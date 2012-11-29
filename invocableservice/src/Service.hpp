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

#ifndef SERVICE_HPP
#define SERVICE_HPP

#include <bb/device/Led>
#include <bb/system/InvokeRequest>

#include <QtCore/QObject>
#include <QtCore/QString>

using bb::system::InvokeRequest;

namespace bb {
namespace system {
class InvokeManager;
}
}

/*
 * This is the service that will be responding to invocation requests.
 */
class Service : public QObject
{
    Q_OBJECT

public:
    Service(QObject *parent = 0);

private Q_SLOTS:
    // This method is called when the invoke request is received from the invoke manager
    void checkTargetAction(const bb::system::InvokeRequest& request);

private:
    // This method is called to play sound upon invocation request
    void playSound(bool isSleep);

    // This method is called to flash the LED upon invocation request
    void flashLED(bool isSleep);

    // This method is called to vibrate phone upon invocation request
    void vibrate(bool isSleep);

private:
    // The central object to manage invocations
    bb::system::InvokeManager *m_invokeManager;

    // The property values
    const QString m_soundString;
    const QString m_ledString;
    const QString m_vibrateString;
    const QString m_killString;
    bb::device::Led m_led;
    const int m_delay;
};

#endif
