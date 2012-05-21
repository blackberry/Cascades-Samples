/* Copyright (c) 2012 Research In Motion Limited.
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
#include "weathermodel.h"

#include <bb/data/JsonDataAccess>
#include <QtNetwork/QNetworkAccessManager>

using namespace bb::cascades;

const char* const WeatherModel::mWeatherAdress =
        "https://github.com/blackberry/Cascades-Samples/raw/master/weatherguesser/assets/models/json/";

WeatherModel::WeatherModel(QObject *parent) :
        GroupDataModel(parent), mReply(0)
{
}

void WeatherModel::onUpdateWeatherCity(QString city)
{
    // Remove all the old data.
    this->clear();

    // Set up a request for loading data from github, we encode the string so that
    // spaces and foreign characters can be handled.
    QString encodedCity = QUrl(city).toEncoded();
    QString path = mWeatherAdress + encodedCity + ".json";
    mReply = mAccessManager.get(QNetworkRequest(QUrl(path)));

    // Connect to the reply finished signal.
    connect(mReply, SIGNAL(finished()), this, SLOT(httpFinished()));
}

void WeatherModel::httpFinished()
{

    if (mReply->error() == QNetworkReply::NoError) {
        // Load the data using the reply QIODevice
        JsonDataAccess jda(mReply);
        loadWeather(&jda);
    } else {
        // If there was an error loading the file via the network we fall back to using
        // a local file instead. This could for example be a previously downloaded file
        // in our case it is a prepackaged file in the assets folder.
        QString urlString = mReply->url().toString();
        urlString.remove(mWeatherAdress);
        QString cityJson = "app/native/assets/models/json/" + urlString;

        JsonDataAccess jda(cityJson);
        loadWeather(&jda);
    }

    // The reply is not needed now we use deleteLater since we are in a slot.
    mReply->deleteLater();
}

void WeatherModel::loadWeather(JsonDataAccess *jda)
{
    this->clear();
    QVariantList weatherData = jda->load().value<QVariantList>();

    if (jda->hasError()) {
        bb::data::DataAccessError* error = jda->error();
        qDebug() << "JSON loading error: " << error->errorType() << ": " << error->errorMessage();
        return;
    }

    QDate date = QDate::currentDate();
    int counter = 0;

    // Convert to QDate for sorting to work on date and set the year to be this year for the prediction data.
    for (int i = 0; i < weatherData.size(); ++i) {
        QVariantMap itemMap = weatherData.at(i).toMap();
        QDate itemDate = QDate::fromString(itemMap["date"].toString(), "yyyy M d");
        itemDate.setYMD(date.year(), itemDate.month(), itemDate.day());
        itemMap["date"] = QVariant(itemDate);

        weatherData.replace(i, itemMap);

        if (itemDate < date) {
            counter++;
        }
    }

    // Remove all data points previous to todays date from the prediction data.
    for (int i = 0; i < counter; i++) {
        weatherData.removeFirst();
    }

    // Finally insert the data into this model.
    this->setSortingKeys(QStringList() << "date");
    this->insert(weatherData);
    this->setGrouping(ItemGrouping::ByFullValue);
}
