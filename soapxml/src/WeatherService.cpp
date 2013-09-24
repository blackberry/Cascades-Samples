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

#include "WeatherService.hpp"

/**
 * WeatherService
 *
 * In this class you will learn the following:
 * -- Interface with a SOAP C++ library
 */

/**
 * WeatherService::WeatherService(QObject* parent)
 *
 * Initialize member variables and connect the SOAP response signal to our onServiceResponse() slot
 */
//! [0]
WeatherService::WeatherService(QObject* parent)
    : QObject(parent)
    , m_succeeded(false)
    , m_active(false)
{
    bool ok = connect(&m_soap, SIGNAL(responseReady()), SLOT(onServiceResponse()));
    Q_ASSERT(ok);
    Q_UNUSED(ok);
}
//! [0]

/**
 * WeatherService::temperature()
 *
 * Return QString, the currently set temperature for a selected city
 */
QString WeatherService::temperature() const
{
    return m_temperature;
}

/**
 * WeatherService::description()
 *
 * Return QString, the currently set description
 */
QString WeatherService::description() const
{
    return m_description;
}

/**
 * WeatherService::succeeded()
 *
 * Return bool, whether the SOAP query was successful or not
 */
bool WeatherService::succeeded() const
{
    return m_succeeded;
}

/**
 * WeatherService::error()
 *
 * Return QString, the error if the SOAP query wasn't successful
 */
QString WeatherService::error() const
{
    return m_error;
}

bool WeatherService::active() const
{
    return m_active;
}

/**
 * WeatherService::requestWeatherInformation(const QString &zipCode)
 *
 * Construct and submit a SOAP request for a specific location
 */
//! [1]
void WeatherService::requestWeatherInformation(const QString &zipCode)
{
    if (m_active)
        return;

    m_active = true;
    emit activeChanged();

    m_succeeded = false;

    m_soap.setAction("http://ws.cdyne.com/WeatherWS/GetCityWeatherByZIP");
    m_soap.setHost("wsf.cdyne.com");

    QtSoapMessage request;
    request.setMethod(QtSoapQName("GetCityWeatherByZIP", "http://ws.cdyne.com/WeatherWS/"));
    request.addMethodArgument("ZIP", "http://ws.cdyne.com/WeatherWS/", zipCode);

    // Submit the method request to the web service.
    m_soap.submitRequest(request, "/WeatherWS/Weather.asmx");
}
//! [1]

void WeatherService::reset()
{
    m_temperature.clear();
    m_description.clear();

    emit temperatureChanged();
    emit descriptionChanged();
}

/**
 * WeatherService::onServiceResponse()
 *
 * SLOT
 * Called when the SOAP interface returns a response
 * Retrieves the SOAP result, sets member variables and
 * emits a signal indicating the request is complete
 */
//! [2]
void WeatherService::onServiceResponse()
{
    // Get the response, check for error.
    const QtSoapMessage& response = m_soap.getResponse();
    if (response.isFault()) {
        m_error = tr("Query failed: %1").arg(response.faultString().value().toString());
        emit statusChanged();

        m_active = false;
        emit activeChanged();

        emit complete();
        return;
    }

    // Extract the return value from this method response, check for
    // errors.
    const QtSoapType& responseValue = response.returnValue();
    if (!responseValue.isValid()) {
        m_error = tr("Query failed: invalid return value");
        emit statusChanged();

        m_active = false;
        emit activeChanged();

        emit complete();
        return;
    }

    if (QString::compare("true", responseValue["Success"].toString(), Qt::CaseInsensitive) == 0) {
        m_succeeded = true;
        m_error.clear();

        const int fahrenheit = responseValue["Temperature"].toInt();
        const int celsius = (fahrenheit - 32)*(5.0 / 9.0);
        m_temperature = QString::fromLatin1("%1 F / %2%3C").arg(fahrenheit).arg(celsius).arg(QChar(0x00B0));
        m_description = responseValue["Description"].toString();

        emit temperatureChanged();
        emit descriptionChanged();
    } else {
        m_succeeded = false;
        m_error = responseValue["ResponseText"].toString();
    }

    emit statusChanged();

    m_active = false;
    emit activeChanged();

    emit complete();
}
//! [2]
