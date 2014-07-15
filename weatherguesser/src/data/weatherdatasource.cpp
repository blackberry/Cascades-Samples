/* Copyright (c) 2013 BlackBerry Limited.
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
#include "weatherdatasource.h"
#include "globalconnection.h"

#include <bb/data/JsonDataAccess>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QDate>

const char* const WeatherDataSource::mWeatherAdress =
        "https://raw.githubusercontent.com/blackberry/Cascades-Samples/master/weatherguesser/assets/models/json/";

WeatherDataSource::WeatherDataSource(QObject *parent) :
        QObject(parent), mReply(0), mRevision(0), mErrorCode(WeatherError::NoError)
{
    // Connect to the sslErrors signal to the onSslErrors() function. This will help us see what errors
    // we get when connecting to the address given by server URL.
    bool connectResult = connect(&mAccessManager, SIGNAL(sslErrors ( QNetworkReply * , const QList<QSslError> & )),
            SLOT(onSslErrors ( QNetworkReply * , const QList<QSslError> & )));
    Q_ASSERT(connectResult);

    // Get the global SQL connection.
    mSqlConnector = GlobalConnection::instance()->sqlConnection();

    connectResult = connect(mSqlConnector, SIGNAL(reply(const bb::data::DataAccessReply&)),
            SLOT(onSqlConnectorReply(const bb::data::DataAccessReply&)));
    Q_ASSERT(connectResult);
}

int WeatherDataSource::incrementRevision()
{
    // First increment the revision stored in the revision table.
    QVariantList dataList;
    dataList << "weather";
    mSqlConnector->executeAndWait(
            "UPDATE revision SET revision_id = revision_id + 1 WHERE table_name=:weather", dataList, 1);

    // Then read the new value and update our internal variable.
    DataAccessReply reply = mSqlConnector->executeAndWait(
            "SELECT revision_id FROM revision WHERE table_name=:weather", dataList, 2);

    QVariantList resultList = reply.result().toList();
    QVariantMap resultMap = resultList.at(0).toMap();
    mRevision = resultMap["revision_id"].toInt();

    return mRevision;
}

QVariantList WeatherDataSource::modifyDataForPrototyping(QVariantList weatherData, uint date)
{
    // The important thing is this function is that we use the data delivered to us
    // in the JSON format. We could read all the data in weatherData and add it to
    // the database that will present it in the list. But instead we will read chunk
    // of it based on the date parameter. This is just to imitate what it might look
    // like if you would make a real server request you would get a chunk of data that
    // you add not all of it like we do here since we do not have a real server
    // (we just access the file remotely on github). So we modify the data and get just
    // part of it for demonstration purposes.
    QDate startDate;
    int chunkSize = 20;

    // If a date has not been set yet we are loading the first batch and start
    // with todays date, otherwise we want the given date + chunksize in the future.
    if (date == 0) {
        startDate = QDate::currentDate().addDays(1);
    } else {
        startDate = QDate::fromJulianDay(date);

        // Add one day to start in the future.
        startDate = startDate.addDays(1);
    }

    int dayOffset = startDate.dayOfYear();

    // Better check so that we don't try and step outside our data, if that's the case wrap around.
    if (dayOffset + chunkSize > 365) {
        dayOffset = 0;
    }

    // Iterate to the day to start copying data.
    QVariantList modifiedData;
    QVariantList::Iterator item = weatherData.begin();
    item += dayOffset;
    for (int i = 0; i <= chunkSize; i++) {
        QVariantMap itemMap = (*item).toMap();
        itemMap["date"] = QVariant(startDate.addDays(i).toJulianDay());

        modifiedData << itemMap;
        ++item;
    }

    return modifiedData;
}

void WeatherDataSource::loadNetworkReplyDataIntoDataBase(QVariantList weatherData)
{
    // Iterate over all the items in the received data.
    QVariantList::Iterator item = weatherData.begin();

    // ValuesTable is a list of lists for adding a batch of data to the database.
    QVariantList valuesTable;

    // Update the data revision since we are adding new data to the data base.
    int revision = incrementRevision();

    while (item != weatherData.end()) {
        QVariantList entryValues;
        QVariantMap itemMap = (*item).toMap();

        // Store the retrieved values for adding to data base.
        entryValues << itemMap["city"] << itemMap["date"] << itemMap["city"] << itemMap["templo"].toInt() <<
                itemMap["temphi"].toInt() << itemMap["tempaverage"].toInt() <<
                itemMap["icon"].toInt() << itemMap["date"] << revision;
        valuesTable << QVariant::fromValue(entryValues);
        ++item;
    }

    // We do insert and replace here to avoid duplicates if data has changed for a specific date.
    QString query ("INSERT OR REPLACE INTO weather (weatherid, city, templo, temphi, tempaverage, icon, date, revision_id) VALUES ((SELECT weatherid FROM weather WHERE city=:city AND date=:date LIMIT 1), :city, :templo, :temphi, :tempaverage, :icon, :date, :revision_id)");
    mSqlConnector->executeBatch(query, valuesTable);
}

void WeatherDataSource::onHttpFinished()
{
    JsonDataAccess jda;
    QVariantList weatherDataFromServer;
    int httpStatus = -1; // controls the final behavior of this function

    if (mReply->error() == QNetworkReply::NoError) {
        // Load the data using the reply QIODevice.
        weatherDataFromServer = jda.load(mReply).value<QVariantList>();

        if (jda.hasError()) {
            bb::data::DataAccessError error = jda.error();
            qDebug() << "JSON loading error:" << error.errorType() << " : " << error.errorMessage();
            httpStatus = -2;
        } else {
            httpStatus = 200;
        }
    } else {
        // An error occurred, try to get the http status code and reason
        QVariant statusCode = mReply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
        QString reason = mReply->attribute(QNetworkRequest::HttpReasonPhraseAttribute).toString();

        if (statusCode.isValid()) {
            httpStatus = statusCode.toInt();
        }

        qDebug() << "Network request to " << mReply->request().url().toString()
                << " failed with http status " << httpStatus << " " << reason;
    }

    // Now behave
    switch (httpStatus) {
        case 200: {
            // We modify our data to get closer to what a server might actually deliver.
            uint date = mReply->request().url().queryItemValue("date").toUInt();
            QVariantList modifiedData = modifyDataForPrototyping(weatherDataFromServer, date);

            loadNetworkReplyDataIntoDataBase(modifiedData);
        }
            break;
        case 404:
            // If we requested index 0 and didn't get an empty array it means the city does not exist and we should show an error
            setErrorCode(WeatherError::InvalidCity);
            break;
        case 503:
            // TODO: perhaps try again a few times and eventually just stop? if we end up stopping and the list is empty, show an alert message. if the list isn't empty just stop fetching
            setErrorCode(WeatherError::ServerBusy);
            break;
        case -2:
            setErrorCode(WeatherError::JsonError);
            break;
        case 500:
        default:
            // The server crapped out, if we don't have any entries let the user know an error occurred, otherwise just stop fetching
            setErrorCode(WeatherError::ServerError);
            break;
    }

    // The reply is not needed now so we call deleteLater() function since we are in a slot.
    mReply->deleteLater();
    mReply = 0;
}

void WeatherDataSource::onSslErrors(QNetworkReply * reply, const QList<QSslError> & errors)
{
    Q_UNUSED(reply);

    foreach (QSslError e, errors) {
        qDebug() << "SSL error: " << e;
    }

    bb::system::SystemDialog *dialog = new bb::system::SystemDialog("OK");

    dialog->setTitle(tr("SSL errors received"));
    dialog->setBody(
            tr("We have received information about a security breach in the protocol. Press \"OK\" to terminate the application"));

    // Connect your functions to handle the predefined signals for the buttons.
    // The slot will check the SystemUiResult to see which button was clicked.

    bool success = connect(dialog, SIGNAL(finished(bb::system::SystemUiResult::Type)),
            SLOT(onDialogFinished(bb::system::SystemUiResult::Type)));

    if (success) {
        // Signal was successfully connected.
        // Now show the dialog box in your UI

        dialog->show();
    } else {
        // Failed to connect to signal.
        dialog->deleteLater();
    }
}

void WeatherDataSource::onDialogFinished(bb::system::SystemUiResult::Type type)
{
    Q_UNUSED(type);
    exit(0);
}

void WeatherDataSource::onSqlConnectorReply(const bb::data::DataAccessReply& reply)
{
    if (reply.hasError()) {
        qWarning() << "WeatherModel: Database error, " << reply.errorType() << " "
                << reply.errorMessage() << " id: " << reply.id();
    } else {
        if (reply.id() == 0) {
            emit weatherChanged(mRevision);
        }
    }
}

WeatherError::Type WeatherDataSource::errorCode()
{
    return mErrorCode;
}

void WeatherDataSource::setErrorCode(WeatherError::Type error)
{
    mErrorCode = error;
    emit errorCodeChanged(mErrorCode);
}

void WeatherDataSource::requestData(const QString city, const QString date)
{
    // Only request data if there is currently no request being done.
    if (mReply == 0) {

        // Reset the error code.
        setErrorCode(WeatherError::NoError);

        // We do not have a "real" server so we access the JSON files stored on github.
        // In reality one would make a request to get a specific portion of data here,
        // we can't do that so we fake it by encoding the date and use that later
        // to read a specific part of the data, we also add the newerItems parameter so we
        // know if items in the past of future should be handled.
        QString encodedCity = QUrl(city).toEncoded();
        QString path = mWeatherAdress + encodedCity + ".json" + "?date=" + date;
        mReply = mAccessManager.get(QNetworkRequest(QUrl(path)));

        // Connect to the reply finished signal to httpFinsihed() Slot function.
        bool connectResult = connect(mReply, SIGNAL(finished()), this, SLOT(onHttpFinished()));
        Q_ASSERT(connectResult);
    }
}

void WeatherDataSource::refresh(const QString city)
{
    QDate tomorrowDate = QDate::currentDate().addDays(1);
    int julianDate = tomorrowDate.toJulianDay();

    // Remove all entries with lower date then tomorrows date.
    QVariantList dataList;
    dataList << city << julianDate;
    mSqlConnector->executeAndWait(
            "DELETE FROM weather WHERE city =:city AND date<:date", dataList, 3);

    requestData(city, QString::number(julianDate));
}
