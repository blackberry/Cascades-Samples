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

#include <bb/cascades/QListDataModel>
#include <bb/data/JsonDataAccess>
#include <bb/system/SystemDialog>
#include <QtNetwork/QNetworkReply>

using namespace bb::data;
using namespace bb::system;


// The weather model is based on the QListDataModel template, which in turn
// implements the abstract DataModel class.
typedef bb::cascades::QListDataModel<QVariant> WeatherListModel;

/* WeatherModel Description:
 *
 * The weather model class is used by the weather guesser application to handle
 * all weather data. It connects to a mock "weather service" that is simulated
 * by requesting a file from the Internet, the file reply is passed to the
 * JsonDataAccess class which is used to set up the model.
 */
class WeatherModel: public WeatherListModel
{
    Q_OBJECT

    /**
     * The city property determines which weather data file is requested
     * from the remote server.
     */
    Q_PROPERTY(QString city READ city WRITE setCity NOTIFY cityChanged)

public:

    /**
     * This is our constructor. This class inherits from GroupDataModel. The parent object is set,
     * reply is initialized, and ssl error signal handling is setup.
     *
     * @param parent The parent object, if not set, @c 0 is used
     */
    WeatherModel(QObject *parent = 0);

    /**
     * This function sets the city to obtain mock weather data for.
     *
     * @param city The name of the city
     */
    void setCity(QString city);

    /**
     * This function returns the name of the city that the mock weather data is for.
     *
     * @return The name of the current set city for mock weather data
     */
    QString city();

    /**
     * A call to request more data for the current city is made via a
     * call to the requestData function.
     *
     * @param loadDelay optional delay before making the request, this is
     *        useful if simulating the data when there is no Internet connection.
     *        If the data is loaded directly in that case it cause the UI to freeze and
     *        the loading activity indicator at the bottom of the list is not shown.
     */
    Q_INVOKABLE void requestData(int loadDelay = 0);

    signals:

    /**
     * Signal emitted when the city property has changed value.
     */
    void cityChanged(QString city);


private slots:
    /**
     * This Slot function is called when the network request to the
     * "weather service" is complete.
     */
    void httpFinished();

    /**
      * Slot called for by the dialog that you get with SSL-errors
      */
     void onDialogFinished(bb::system::SystemUiResult::Type type);

    /**
     * This Slot function is connected to the mAccessManager sslErrors signal. This function
     * allows us to see what errors we get when connecting to the address given by mWeatherAdress.
     *
     * @param reply The network reply
     * @param errors SSL Error List
     */
    void onSslErrors(QNetworkReply * reply, const QList<QSslError> & errors);

    /**
     * This function is either called directly from requestData if the loadDelay parameter is set to zero (default).
     * if a loadDelay is used to simulate fetching data from the internet a QTimer is connected to the slot.
     */
    void onDelayTimeout();

private:
    /**
     * This Helper function used to set up the model data in the JsonDataAccess class
     *
     * @param weatherData The QVariantList with weather data that is to be added to the model.
     */
    void loadWeather(QVariantList weatherData);

    // The network parameters; used for accessing a file from the Internet
    QNetworkAccessManager mAccessManager;
    QNetworkReply *mReply;

    // String constant for the address used to look up weather data
    static const char* const mWeatherAdress;

    // The current city for obtaining mock weather data
    QString mCity;
};

#endif // ifndef _WEATHERMODEL_H_
