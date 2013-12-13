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
#ifndef _WEATHERDATASOURCE_H_
#define _WEATHERDATASOURCE_H_

#include "weathererror.h"
#include <bb/data/JsonDataAccess>
#include <bb/data/SqlConnection>
#include <bb/system/SystemDialog>

#include <QtNetwork/QNetworkReply>

using namespace bb::data;

class WeatherError;

/* WeatherModel Description:
 *
 * The weather model class is used by the weather history application to handle
 * all weather data. It connects to a "weather service" that is simulated
 * by requesting a file from the Internet, the file reply is passed to the
 * JsonDataAccess class which is used to set up the model.
 */
class WeatherDataSource: public QObject
{
    Q_OBJECT

    /**
     * The last reported error code.
     */
    Q_PROPERTY(WeatherError::Type errorCode READ errorCode NOTIFY errorCodeChanged FINAL)

public:
    /**
     * This is our constructor. The parent object is set,
     * reply is initialized, and SSL error signal handling is setup.
     *
     * @param parent The parent object, if not set, @c 0 is used
     */
    WeatherDataSource(QObject *parent = 0);

    /**
     * The last error code reported when retrieving data.
     */
    WeatherError::Type errorCode();

    /**
     * Call this function to request weather data for a specific city.
     *
     * @param city The name of the city.
     * @param date The date used to determine offset for the request.
     */
    Q_INVOKABLE void requestData(const QString city, const QString date);


    /**
     * Call this function refresh the data in the database so that the
     * first item is showing guessed weather for tomorrow.
     *
     * @param city The name of the city.
     */
    Q_INVOKABLE void refresh(const QString city);

    /**
     * Returns the current global revision of the database plus one.
     *
     * @return An integer with next revision.
     */
    Q_INVOKABLE int incrementRevision();

signals:

    /**
     * Signal emitted when the network error status changes.
     */
    void errorCodeChanged(WeatherError::Type newError);

    /**
     * Signal emitted when new weather data has been added to the data base.
     */
    void weatherChanged(int revision);

    /**
     * Signal emitted when a request for data was made but no date is available.
     */
    void noMoreWeather();

private slots:
    /**
     * This Slot function is called when the network request to the
     * "weather service" is complete.
     */
    void onHttpFinished();

    /**
     * This Slot function is connected to the mAccessManager sslErrors signal. This function
     * allows us to see what errors we get when connecting to the address given by server url.
     *
     * @param reply The network reply
     * @param errors SSL Error List
     */
    void onSslErrors(QNetworkReply * reply, const QList<QSslError> & errors);

    /**
     * Function used to connect to the SqlConnectors reply signal, this is
     * when data base queries have been executed.
     *
     * @param DataAccessReply reply The Database reply
     */

    void onSqlConnectorReply(const bb::data::DataAccessReply& reply);

private:
    /**
     * Since we are not working against a real server that can deliver data
     * to us, we do some modifications of the list delivered from the network.
     *
     * @param weatherData The complete data set that we will use to create a subset representing
     *        the "real" server data.
     * @param date The start date from which data is supposed to be offset from (if 0 today is used)
     */
    QVariantList modifyDataForPrototyping(QVariantList weatherData, uint date);


    /**
     * This Helper function used to store the data received from the network.
     *
     * @param weatherData The QVariantList with weather data that is to be added to the database.
     */
    void loadNetworkReplyDataIntoDataBase(QVariantList weatherData);

    /**
     * Sets the error code and emits a signal that the error code has changed
     *
     * @parameter error the new error
     */
    void setErrorCode(WeatherError::Type error);

    /**
     * Slot called for by the dialog that you get with SSL-errors
     */
    Q_SLOT void onDialogFinished(bb::system::SystemUiResult::Type type);

    // The network parameters; used for accessing a file from the Internet
    QNetworkAccessManager mAccessManager;
    QNetworkReply *mReply;

    // Data base connector.
    SqlConnection *mSqlConnector;

    // The last accessed data revision.
    int mRevision;

    // The last reported error code.
    WeatherError::Type mErrorCode;

    // String constant for the address used to look up weather data
    static const char* const mWeatherAdress;
};

#endif // ifndef _WEATHERDATASOURCE_H_
