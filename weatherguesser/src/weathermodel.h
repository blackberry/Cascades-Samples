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
#ifndef _WEATHERMODEL_H_
#define _WEATHERMODEL_H_

#include <bb/cascades/GroupDataModel>
#include <bb/data/JsonDataAccess>
#include <QtNetwork/QNetworkReply>

using namespace bb::data;

/* WeatherModel
 *
 * The weather model class is used by the weather guesser application to handle
 * all weather data. It connects to a mock "weather service" that simulated
 * by requesting a file from the Internet, the reply is passed to a
 * JsonDataAccess, which is then used to set up the model.
 */
class WeatherModel: public bb::cascades::GroupDataModel
{
Q_OBJECT

Q_PROPERTY(QString city READ city WRITE setCity
               NOTIFY cityChanged)

public:
    WeatherModel(QObject *parent = 0);

    void setCity(QString city);

    QString city();

signals:
    void cityChanged(QString city);

private slots:
    /**
     * Slot function that is called when the network request to the
     * "weather service" is complete.
     */
    void httpFinished();

    /**
     * Slot function connected to the mAccessManager sslErrors signal.
     */
    void onSslErrors(QNetworkReply * reply, const QList<QSslError> & errors);

private:
    /**
     * Helper function used to set up the model data in the JsonDataAccess
     *
     * @param weatherData The QVariantList with weather data that is to be added to the model.
     */
    void loadWeather(QVariantList weatherData);

    // The network parameters used for accessing a file from the Internet.
    QNetworkAccessManager mAccessManager;
    QNetworkReply *mReply;

    // String constant for the address used to look up weather data.
    static const char* const mWeatherAdress;

    QString mCity;
};

#endif // ifndef _WEATHERMODEL_H_
