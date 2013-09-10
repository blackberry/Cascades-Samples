/*
 * Copyright (c) 2013 BlackBerry Limited.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "SqlQueryUtils.hpp"
#include "NumericRevision.hpp"

#include <bb/UIToolkitSupport>
#include <QVariantMap>
#include <QStringList>
#include <QTime>
#include <QSharedData>
#include <QMutexLocker>
#include <QElapsedTimer>
#include <QStringBuilder>
#include <QTextStream>
#include <QThread>
#include <QUrl>
#include <QtSql/QSqlError>
#include <QtSql/QSqlRecord>
#include <QtSql/QSqlQuery>
#include <QtCore/QDebug>

using namespace bb::cascades::datamanager;

///////////////////////////////////////////////////////////////////////////////
///////////////////////// File scope helper functions//////////////////////////
///////////////////////////////////////////////////////////////////////////////
static QVariant valueLookup(const QVariantMap& bindValues, QString key);
static QVariantList createPositionalQueryAndList(QString& baseSql, const QVariantMap& bindValues);
static void bindParameters(const QVariantList& bindValues, QSqlQuery *sqlQuery);

SqlQueryUtils::SqlQueryUtils() {
}

SqlQueryUtils::~SqlQueryUtils() {
}

/**
 * Return the existing database connection from QT or create and return a new connection.
 */
QSqlDatabase SqlQueryUtils::connection(const QUrl &source, QSqlError *error) {
    // construct sourcePath and connectionName from source URL
    QString sourcePath = bb::UIToolkitSupport::absolutePathFromUrl(source);
    int threadId = (int) QThread::currentThreadId();
    QString connectionName = sourcePath + ":" + QString::number(threadId);

    // get existing connection if any
    QSqlDatabase connection = QSqlDatabase::database(connectionName, false);

    if (!connection.isOpen()) {
        if (sourcePath.isEmpty()) {
            *error = QSqlError("", "missing database name", QSqlError::ConnectionError);
        } else {
            connection = QSqlDatabase::addDatabase("QSQLITE", connectionName);
            connection.setDatabaseName(sourcePath);
            if (!connection.isValid()) {
                // invalid driver
                *error = QSqlError("", "invalid database driver", QSqlError::ConnectionError);
            } else {
                QFileInfo dbFile(sourcePath);
                if (!dbFile.exists()) {
                    *error = QSqlError("", "no existing database found \"" + sourcePath + "\"",
                            QSqlError::ConnectionError);
                } else {
                    bool success = connection.open();
                    if (!success) {
                        *error = QSqlError("", "unable to open existing database \"" + sourcePath + "\"",
                                QSqlError::ConnectionError);
                    } else {
                        // success
                        *error = QSqlError();
                    }
                }
            }
        }
    }
    if (error->type() != QSqlError::NoError) {
        qDebug() << "query error: " << *error;
    }
    return connection;
}

QString SqlQueryUtils::extendQuery(const QString &query, int offset, int limit) {
    return QString("%1 limit %2 offset %3").arg(query).arg(limit).arg(offset);
}

bool SqlQueryUtils::getQueryData(QSqlDatabase &connection, const QString &query, int offset, int limit,
        const QVariantMap &bindValues, const QString &keyColumn, const QString &revisionColumn,
        QList<DataItem> *results, QSqlError *error) {
    QElapsedTimer timer;
    timer.start();
    results->clear();

    QSqlQuery sqlQuery(connection);
    QString fullQuery = extendQuery(query, offset, limit);
    SqlQueryUtils::prepareQuery(fullQuery, bindValues, &sqlQuery);

    bool success = sqlQuery.exec();
    if (!success) {
        *error = sqlQuery.lastError();
        qDebug() << "query error: " << *error;
        return false;
    }
    QList<QString> field;
    // Build the result structure.
    while (sqlQuery.next()) {
        QVariantMap recordMap;
        if (field.isEmpty()) {
            QSqlRecord record = sqlQuery.record();
            for (int i = 0, n = record.count(); i < n; i++) {
                field.append(record.fieldName(i));
            }
        }
        for (int i = 0, n = field.size(); i < n; i++) {
            QVariant value = sqlQuery.value(i);
            // JavaScript/QML doesn't support 64bit integers (everything is converted to double)
            // so convert to string here to make sure we don't run into trouble.
            // In the long term it will be cleaner to move this code into something like a
            // query decorator instead of the core query code.
            if (value.type() == QVariant::LongLong || value.type() == QVariant::ULongLong) {
                value = value.toString();
            }
            recordMap.insert(field.at(i), value);
        }

        // the data item key
        QString keyId;
        if (!keyColumn.isEmpty() && recordMap.contains(keyColumn)) {
            keyId = recordMap.value(keyColumn).toString();
        }

        // the data item revision
        quint64 revision = 0;
        bool revisionOK = false;
        if (!revisionColumn.isEmpty() && recordMap.contains(revisionColumn)) {
            revision = recordMap.value(revisionColumn).toULongLong(&revisionOK);
        }
        DataRevision dataRev = revisionOK ? DataRevision(new NumericRevision(revision)) : DataRevision();

        results->append(DataItem(keyId, dataRev, recordMap));
    }

    qDebug() << "Query executed: " << sqlQuery.executedQuery();
    qDebug() << "Loaded " << results->size() << " items in " << timer.elapsed() << "ms";
    return true;
}

bool SqlQueryUtils::getSingleQueryValue(QSqlDatabase &connection, const QString &query, const QVariantMap &bindValues,
        const QString &resultName, QVariant *resultValue, QSqlError *error) {
    QElapsedTimer timer;
    timer.start();

    QSqlQuery sqlQuery(connection);
    SqlQueryUtils::prepareQuery(query, bindValues, &sqlQuery);

    *resultValue = QVariant();
    bool success = sqlQuery.exec();
    if (success && sqlQuery.next()) {
        QSqlRecord record = sqlQuery.record();
        int recCount = record.count();
        for (int i = 0; i < recCount; ++i) {
            // Verify result column name if its provided by the caller
            // Otherwise grab the first value and return it
            if (resultName.isEmpty() || resultName.compare(record.fieldName(i), Qt::CaseInsensitive) == 0) {
                *resultValue = sqlQuery.value(i);
                break;
            }
        }
    } else {
        *error = sqlQuery.lastError();
        qDebug() << "query error: " << *error;
        return false;
    }

    qDebug() << "Fetched single value=" << *resultValue << " in " << timer.elapsed() << "ms";
    return true;
}

QList<HeaderDataItem> SqlQueryUtils::normalizeHeaderData(const QList<DataItem> &data) {
    QList<HeaderDataItem> headerData;

    if (data.size() == 0) {
        return headerData;
    }

    // examine the first data item
    const DataItem &itemZero = data.at(0);
    QVariantMap itemZeroProperties = itemZero.payload().toMap();
    QStringList itemZeroKeys = itemZeroProperties.keys();

    // get the count key
    QString countKey;
    for (int i = 0, n = itemZeroKeys.size(); i < n; i++) {
        QString keyLower = itemZeroKeys.at(i).toLower();
        if ((keyLower == "count") || keyLower.startsWith("count(")) {
            countKey = itemZeroKeys.at(i);
            break;
        }
    }
    if (countKey.isEmpty()) {
        qDebug() << "DataItem does not contain enough header information (no count) to normalize to HeaderDataItem";
    }

    // try a header key name of "header" if no key identified yet
    QString headerKey;
    if (itemZero.keyId().isEmpty()) {
        if (itemZeroProperties.contains("header")) {
            headerKey = "header";
        }
        else {
            qDebug() << "DataItem does not contain enough header information (no key) to normalize to HeaderDataItem";
        }
    }

    for (int i = 0, n = data.size(); i < n; i++) {
        DataItem dataItem = data.at(i);
        QVariantMap properties = dataItem.payload().toMap();
        if (properties.contains(countKey)) {
            int count = properties.value(countKey).toInt();
            if (count > 0) {
                if (dataItem.keyId().isEmpty() && !headerKey.isEmpty()) {
                    HeaderDataItem headerItem(properties.value(headerKey).toString(), dataItem.revision(), count, dataItem.payload());
                    headerData.append(headerItem);
                }
                else {
                    HeaderDataItem headerItem(dataItem, count);
                    headerData.append(headerItem);
                }
            }
        }
    }
    return headerData;
}

/**
 * Replace named placeholder query with positional placeholder query to get rid of problems with QT code.
 * This fixes:
 * 1. When a query has no parameters but bindValues *is* supplied.
 * 2. When a query uses the same named parameter more than once in query string.
 *
 * Fix should be applied in QT code in QSqlRecordPrivate.
 */
void SqlQueryUtils::prepareQuery(const QString &query, const QVariantMap &bindValues, QSqlQuery *sqlQuery) {
    QString positionalQuery(query);
    QVariantList positionalBindValues = createPositionalQueryAndList(positionalQuery, bindValues);
    if (sqlQuery->prepare(positionalQuery)) {
        if (!positionalBindValues.isEmpty()) {
            bindParameters(positionalBindValues, sqlQuery);
        }
    }
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////// File scope helper functions//////////////////////////
//////////// copied from multimedia to solve issues in QSqlResult /////////////

// deal with keys with or without leading ':'
static QVariant valueLookup(const QVariantMap& bindValues, QString key) {
    if (!bindValues.contains(key) && key.startsWith(':')) {
        key = key.remove(0, 1);
    }
    return bindValues.value(key);
}

static QVariantList createPositionalQueryAndList(QString& baseSql, const QVariantMap& bindValues) {
    QVariantList values;

    if (bindValues.size() > 0) {
        //Note: the below algorithm is based on QSqlResultPrivate::namedToPositionalBinding
        //in an attempt to keep this code in sync with QT's param parsing.

        int length = baseSql.length();
        bool inEscape = false;

        for (int i = 0; i < length;) {
            QChar ch = baseSql.at(i);
            if (ch == QLatin1Char(':') && !inEscape && (i == 0 || baseSql.at(i - 1) != QLatin1Char(':'))
                    && (i + 1 < length && (baseSql.at(i + 1)).isLetterOrNumber())) {

                //fast forward to end of term
                int end = i + 2;
                for (; end < length && baseSql.at(end).isLetterOrNumber(); ++end)
                    ;

                //extract value for this term
                QString oldKey = baseSql.mid(i, end - i);

                //replace term with unique term
                baseSql.replace(i, end - i, '?');
                values.append(valueLookup(bindValues, oldKey));
                //values.append(bindValues.value(oldKey));

                //correct loop variables
                length = baseSql.length();
                i = end + (1 - oldKey.length());
            } else {
                if (ch == QLatin1Char('\'')) {
                    inEscape = !inEscape;
                }
                ++i;
            }
        }
    }
    return values;

}

static void bindParameters(const QVariantList& bindValues, QSqlQuery *sqlQuery) {
    if (!bindValues.isEmpty()) {
        for (int i = bindValues.length() - 1; i >= 0; --i) {
            sqlQuery->bindValue(i, bindValues.at(i));
        }
    }
}

