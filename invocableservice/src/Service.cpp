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

#include "Service.hpp"

#include <bb/cascades/Application>
#include <bb/system/InvokeManager>
#include <bb/device/Led>
#include <bb/device/LedColor>
#include <bb/device/VibrationController>
#include <bb/multimedia/SystemSound>

#include <QtCore/QByteArray>
#include <QtCore/QDebug>

#include <iostream>
#include <unistd.h>

using namespace bb::device;
using namespace bb::multimedia;
using namespace std;

using bb::system::InvokeManager;
using bb::system::InvokeRequest;

//! [0]
Service::Service(QObject *parent)
    : QObject(parent)
    , m_invokeManager(new InvokeManager(this))
    // Create strings to identify the invoke target id requested
    , m_soundString(QLatin1String("blackberry.sample.service.sound"))
    , m_ledString(QLatin1String("blackberry.sample.service.led"))
    , m_vibrateString(QLatin1String("blackberry.sample.service.vibrate"))
    , m_killString(QLatin1String("blackberry.sample.service.exit"))
    // Default sleep delay
    , m_delay(5)
{
    // Wait to receive signal of invokeRequest from invokeManager, and call checkTargetAction when signal received
    connect(m_invokeManager, SIGNAL(invoked(const bb::system::InvokeRequest&)), this, SLOT(checkTargetAction(const bb::system::InvokeRequest&)));
}
//! [0]

//! [1]
void Service::checkTargetAction(const bb::system::InvokeRequest& request)
{
    qDebug() << "*checkTarget called in Service.cpp, action is " << request.target();
    cout << "*checkTarget called in Service.cpp, action is " << request.target().toStdString() << endl;

    // get target attribute from request
    const QString reqTarget = request.target();

    if (!reqTarget.startsWith("blackberry.sample.service")) {
        cout << "invalid entry target: " << reqTarget.toStdString() << endl;
        return;
    }

    // get data attribute from request
    const QString sleepData(request.data());
    bool sleep = false;

    // debug: print array sleepData
    qDebug() << "**sleepData: " << sleepData;
    cout << endl << "**sleepData: " << sleepData.toStdString() << endl;

    if ("Sleep" == sleepData) {
        sleep = true;
    }

    // Invoke functions according to requested target
    if (reqTarget == m_soundString) {
        qDebug() << "**playSound service invoked";
        cout << "**play sound service invoked" << endl;

        // Trigger the sound
        playSound(sleep);
    } else if (reqTarget == m_ledString) {
        qDebug() << "**flashLED service invoked";
        cout << "**flashLED service invoked" << endl;

        // Trigger the LED
        flashLED(sleep);
    } else if (reqTarget == m_vibrateString) {
        qDebug() << "**vibrate service invoked";
        cout << "**vibrate service invoked" << endl;

        // Trigger the vibrate
        vibrate(sleep);
    }  else if (reqTarget == m_killString) {
        qDebug() << "**kill service invoked";
        cout << "**kill service invoked" << endl;

        bb::cascades::Application::exit(0);
    }
}
//! [1]

//! [2]
void Service::playSound(bool isSleep)
{
    qDebug() << "*playSound called in Service.cpp";
    cout << "*playSound called in Service.cpp" << endl;

    // Delay execution if requested
    if(isSleep) {
        cout << "**isSleep TRUE" << endl;
        sleep(m_delay);
    }

    // Play system sound
    SystemSound::play(SystemSound::CameraShutterEvent);
}
//! [2]

//! [3]
void Service::flashLED(bool isSleep)
{
    qDebug() << "*flashLED called in Service.cpp";
    cout << "*flashLED called in Service.cpp" << endl;

    // Delay execution if requested
    if(isSleep) {
        cout << "**isSleep TRUE" << endl;
        sleep(m_delay);
    }

    // Flash LED
    m_led.flash();
}
//! [3]

//! [4]
void Service::vibrate(bool isSleep)
{
    qDebug() << "*vibrate called in Service.cpp";
    cout << "*vibrate called in Service.cpp" << endl;

    VibrationController vc;

    // Delay execution if requested
    if (isSleep) {
        cout << "**isSleep TRUE" << endl;
        sleep(m_delay);
    }

    // Vibrate phone with 80 intensity for 500 milliseconds
    vc.start(80,500);
}
//! [4]
