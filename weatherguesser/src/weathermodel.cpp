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
    "https://raw.github.com/blackberry/Cascades-Samples/master/weatherguesser/assets/models/json/";

WeatherModel::WeatherModel(QObject *parent) :
    GroupDataModel(parent), mReply(0)
{
  // Connect to the sslErrors signal to the onSslErrors() function. This will help us see what errors
  // we get when connecting to the address given by mWeatherAdress.
  connect(&mAccessManager,
      SIGNAL(sslErrors ( QNetworkReply * , const QList<QSslError> & )), this,
      SLOT(onSslErrors ( QNetworkReply * , const QList<QSslError> & )));
}

void WeatherModel::httpFinished()
{
  JsonDataAccess jda;
  QVariantList weatherData;

  if (mReply->error() == QNetworkReply::NoError)
  {
    // Load the data using the reply QIODevice.

    weatherData = jda.load(mReply).value<QVariantList>();

  }
  else
  {
    // If there was an error loading the file via the network, we fall back to using
    // a local file instead. This could, for example, be a previously downloaded file.
    // In our case, it is a pre-packaged file in the assets folder.
    QString urlString = mReply->url().toString();
    urlString.remove(mWeatherAdress);
    QString cityJson = "app/native/assets/models/json/" + urlString;

    weatherData = jda.load(cityJson).value<QVariantList>();
  }

  if (jda.hasError())
  {
    bb::data::DataAccessError error = jda.error();
    qDebug() << "JSON loading error: " << error.errorType() << ": "
        << error.errorMessage();
    return;
  }

  loadWeather(weatherData);

  // The reply is not needed now so we call deleteLater() function since we are in a slot.
  mReply->deleteLater();
}

void WeatherModel::loadWeather(QVariantList weatherData)
{
  this->clear();

  QDate date = QDate::currentDate();
  int counter = 0;

  // Convert to QDate so sorting is by date.
  // Set the year to this year for the prediction data.
  for (int i = 0; i < weatherData.size(); ++i)
  {
    QVariantMap itemMap = weatherData.at(i).toMap();
    QDate itemDate = QDate::fromString(itemMap["date"].toString(), "yyyy M d");
    itemDate.setYMD(date.year(), itemDate.month(), itemDate.day());
    itemMap["date"] = QVariant(itemDate);

    weatherData.replace(i, itemMap);

    if (itemDate < date)
    {
      counter++;
    }
  }

  // Remove all data points previous to today's date from the prediction data.
  for (int i = 0; i < counter; i++)
  {
    weatherData.removeFirst();
  }

  // Insert the data into this model.
  setSortingKeys(QStringList() << "date");
  insertList(weatherData);
  setGrouping(ItemGrouping::ByFullValue);
}

void WeatherModel::onSslErrors(QNetworkReply * reply,
    const QList<QSslError> & errors)
{
  // Ignore all SSL errors to be able to load from JSON file from the secure address.
  // It might be a good idea to display an error message indicating that security may be compromised.
  //
  // The errors we get are:
  // "SSL error: The issuer certificate of a locally looked up certificate could not be found"
  // "SSL error: The root CA certificate is not trusted for this purpose"
  // Seems to be a problem with how the server is set up and a known QT issue QTBUG-23625

  reply->ignoreSslErrors(errors);
}

void WeatherModel::setCity(QString city)
{
  if (mCity.compare(city) != 0)
  {

    // Remove all the old data.
    this->clear();

    // Set up a request for loading data from github, we encode the string so that
    // spaces and websafe characters can be handled.
    QString encodedCity = QUrl(city).toEncoded();
    QString path = mWeatherAdress + encodedCity + ".json";
    mReply = mAccessManager.get(QNetworkRequest(QUrl(path)));

    // Connect to the reply finished signal to httpFinsihed() Slot function.
    connect(mReply, SIGNAL(finished()), this, SLOT(httpFinished()));

    mCity = city;
    emit cityChanged(city);
  }
}

QString WeatherModel::city()
{
  return mCity;
}

