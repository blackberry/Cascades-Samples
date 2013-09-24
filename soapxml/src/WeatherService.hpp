/* Copyright (c) 2012, 2013  BlackBerry Limited.
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

#ifndef WEATHERSERVICE_HPP
#define WEATHERSERVICE_HPP

#include "qtsoap/qtsoap.h"

#include <QtCore/QObject>

class WeatherService : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString temperature READ temperature NOTIFY temperatureChanged)
    Q_PROPERTY(QString description READ description NOTIFY descriptionChanged)
    Q_PROPERTY(bool succeeded READ succeeded NOTIFY statusChanged)
    Q_PROPERTY(QString error READ error NOTIFY statusChanged)
    Q_PROPERTY(bool active READ active NOTIFY activeChanged)

public:
    WeatherService(QObject* parent = 0);

    /*
     * Retrieves the temperature from the previous request
     */
    QString temperature() const;

    /*
     * Retrieves the description from the previous request
     */
    QString description() const;

    /*
     * Returns whether the previous request succeeded or failed
     */
    bool succeeded() const;

    /*
     * Retrieves the error string from the previous request if one occurred
     * Check getSucceeded() first
     */
    QString error() const;

    /*
     * Returns whether there is currently a lookup running.
     */
    bool active() const;

public Q_SLOTS:
    /*
     * Retrieves the weather information for the specified zip code (US only)
     */
    void requestWeatherInformation(const QString &zipCode);

    /*
     * Clears the internal state of the weather service object.
     */
    void reset();

Q_SIGNALS:
    /*
     * The complete signal is emitted when the request to the webservice is done
     */
    void complete();

    /*
     * The change notification signals of the properties
     */
    void temperatureChanged();
    void descriptionChanged();
    void statusChanged();
    void activeChanged();

private Q_SLOTS:
    /*
     * Handler for SOAP request response
     */
    void onServiceResponse();

private:
    QString m_temperature;       // temperature in F and C
    QString m_description;       // description of the weather
    QString m_error;             // error string if one occurred
    bool m_succeeded;            // flag to determine whether request succeeded
    bool m_active;               // flag to determine whether a request is running
    QtSoapHttpTransport m_soap;  // soap transport
};

#endif
