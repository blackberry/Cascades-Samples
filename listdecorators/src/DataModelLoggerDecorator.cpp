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
#include "DataModelLoggerDecorator.hpp"
#include <bb/cascades/DataModel>
#include <QDebug>

//! [0]
// Custom IndexMapper class for logging purposes.
class IndexMapperLogger: public bb::cascades::DataModel::IndexMapper {

public:
    explicit IndexMapperLogger(
            QSharedPointer<bb::cascades::DataModel::IndexMapper> indexMapper,
            DataModelLoggerDecorator *model);
    virtual ~IndexMapperLogger();

    virtual bool newIndexPath(QVariantList *pOutIndexPath,
            int *pOutReplacementIndex, const QVariantList &oldIndexPath) const;

private:
    bb::cascades::DataModel *model() const;

    // Holds the original IndexMapper class.
    QSharedPointer<bb::cascades::DataModel::IndexMapper> m_indexMapper;
    DataModelLoggerDecorator *m_model;
};
//! [0]
//! [1]
IndexMapperLogger::IndexMapperLogger(
        QSharedPointer<bb::cascades::DataModel::IndexMapper> indexMapper,
        DataModelLoggerDecorator *model)
    : m_indexMapper(indexMapper)
    , m_model(model)
{
}

IndexMapperLogger::~IndexMapperLogger() {
}

bb::cascades::DataModel *IndexMapperLogger::model() const {
    return m_model->model();
}

bool IndexMapperLogger::newIndexPath(QVariantList *pOutIndexPath,
        int *pOutReplacementIndex, const QVariantList &oldIndexPath) const {
    // Store result from original IndexMapper. Log output based on result and return the original
    // value.
    bool result = m_indexMapper->newIndexPath(pOutIndexPath,
            pOutReplacementIndex, oldIndexPath);

    if (!result) {
        qDebug() << "DEL: " << oldIndexPath;
    } else if (oldIndexPath != *pOutIndexPath) {
        qDebug() << "MOV: " << oldIndexPath << " -> " << *pOutIndexPath;
    } else if (m_model->logAllCallbacks()) {
        qDebug() << "NOP: " << oldIndexPath;
    }

    return result;
}
//! [1]
///////////////////////////////////////////////////////////////////////////////
// DataModelLogger implementation
//! [2]
DataModelLoggerDecorator::DataModelLoggerDecorator(QObject *parent)
    : bb::cascades::datamanager::DataModelDecorator(parent)
    , m_logBegin(true)
    , m_logData(true)
    , m_logAllCallbacks(true)
{
}
//! [2]
DataModelLoggerDecorator::~DataModelLoggerDecorator() {
}

//! [3]
// Introduce log messages, invoke parent method and return it's value.
int DataModelLoggerDecorator::childCount(const QVariantList &indexPath) {
    logStart("childCount()", indexPath);
    int result = DataModelDecorator::childCount(indexPath);
    logEnd("childCount()", indexPath, QString::number(result));
    return result;
}
//! [3]
// Introduce log messages, invoke parent method and return it's value.
bool DataModelLoggerDecorator::hasChildren(const QVariantList &indexPath) {
    logStart("hasChildren()", indexPath);
    bool result = bb::cascades::datamanager::DataModelDecorator::hasChildren(
            indexPath);
    logEnd("hasChildren()", indexPath, QString(result ? "true" : "false"));
    return result;
}

// Introduce log messages, invoke parent method and return it's value.
QString DataModelLoggerDecorator::itemType(const QVariantList &indexPath) {
    logStart("itemType()", indexPath);
    QString result = bb::cascades::datamanager::DataModelDecorator::itemType(
            indexPath);
    logEnd("itemType()", indexPath, result);
    return result;
}

// Introduce log messages, invoke parent method and return it's value.
QVariant DataModelLoggerDecorator::data(const QVariantList &indexPath) {
    logStart("data()", indexPath);
    QVariant result = bb::cascades::datamanager::DataModelDecorator::data(
            indexPath);
    if (m_logData) {
        // Used for storing the data as string for logging purposes.
        QString dataStr;
        QDebug str(&dataStr);
        str << result;
        logEnd("data()", indexPath, dataStr);
    } else {
        logEnd("data()", indexPath,
                QString(
                        result.isNull() || !result.isValid() ?
                                "NULL" : result.typeName()));
    }

    return result;
}

void DataModelLoggerDecorator::load() {
    if (QMetaObject::invokeMethod(model(), "load", Qt::AutoConnection)) {
        QObject::connect(model(), SIGNAL(loaded()), this, SIGNAL(loaded()));
    }
}

bool DataModelLoggerDecorator::logData() {
    return m_logData;
}

bool DataModelLoggerDecorator::logBegin() {
    return m_logBegin;
}

void DataModelLoggerDecorator::setLogData(bool enable) {
    m_logData = enable;
}

void DataModelLoggerDecorator::setLogBegin(bool enable) {
    m_logBegin = enable;
}

bool DataModelLoggerDecorator::logAllCallbacks() {
    return m_logAllCallbacks;
}

void DataModelLoggerDecorator::setLogAllCallbacks(bool enable) {
    m_logAllCallbacks = enable;
}

void DataModelLoggerDecorator::onItemAdded(QVariantList indexPath) {
    qDebug() << "onItemAdded()->" << indexPath;
    bb::cascades::datamanager::DataModelDecorator::onItemAdded(indexPath);
}

void DataModelLoggerDecorator::onItemUpdated(QVariantList indexPath) {
    qDebug() << "onItemUpdated()->" << indexPath;
    bb::cascades::datamanager::DataModelDecorator::onItemUpdated(indexPath);
}

void DataModelLoggerDecorator::onItemRemoved(QVariantList indexPath) {
    qDebug() << "onItemRemoved()->" << indexPath;
    bb::cascades::datamanager::DataModelDecorator::onItemRemoved(indexPath);
}

void DataModelLoggerDecorator::onItemsChanged(
        bb::cascades::DataModelChangeType::Type eChangeType,
        QSharedPointer<bb::cascades::DataModel::IndexMapper> indexMapper) {
    qDebug() << "onItemsChanged()->" << this->model()->objectName();
    // Creating our log instrumented custom IndexMapper class.
    QSharedPointer<IndexMapperLogger> mapperLogger(
            new IndexMapperLogger(indexMapper, this));
    bb::cascades::datamanager::DataModelDecorator::onItemsChanged(eChangeType,
            mapperLogger);
}

void DataModelLoggerDecorator::logStart(const QString &method,
        const QVariantList &list) {
    m_timer.start();
    if (m_logBegin) {
        qDebug() << method << "->" << list << "+";
    }
}

void DataModelLoggerDecorator::logEnd(const QString &method,
        const QVariantList &list, const QString &result) {
    if (m_logBegin) {
        qDebug() << method << "->" << list << "::" << result << " - "
                << m_timer.elapsed() << "ms";
    }
}

