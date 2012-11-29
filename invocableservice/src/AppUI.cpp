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

#include "AppUI.hpp"
#include "Service.hpp"

#include <bb/cascades/AbstractPane>
#include <bb/cascades/Application>
#include <bb/cascades/QmlDocument>
#include <bb/system/InvokeRequest>
#include <bb/system/InvokeTargetReply>

#include <iostream>

using namespace std;
using namespace bb::cascades;

using bb::system::InvokeRequest;
using bb::system::InvokeTargetReply;

//! [0]
AppUI::AppUI(QObject *parent)
    : QObject(parent)
{
    qDebug() << "*AppUI called in AppUI.cpp";

    // Launch the service
    m_service = new Service(this);

    // Disable sleep by default
    m_sleep = tr("NotSleep");

    // Load the UI description from main.qml
    QmlDocument *qml = QmlDocument::create("asset:///main.qml").parent(this);
    qml->setContextProperty("_app", this);

    // Create the application scene
    AbstractPane *root = qml->createRootObject<AbstractPane>();
    Application::instance()->setScene(root);
}
//! [0]

//! [2]
void AppUI::sendSoundInvokeRequest()
{
    qDebug() << "*sendSoundInvokeRequest";
    cout << "*sendSoundInvokeRequest" << endl;

    // Created request for sound invocation
    InvokeRequest request;
    request.setTarget("blackberry.sample.service.sound");
    request.setAction("test.action.PLAY_SOUND");
    request.setData(m_sleep.toAscii());
    cout << "send request" << endl;

    // Start the invocation
    InvokeTargetReply *targetReply = m_invokeManager.invoke(request);
    if (targetReply == 0) {
        cout << "***request failed" << endl;
    }
}
//! [2]


//! [3]
void AppUI::sendLEDInvokeRequest()
{
    qDebug() << "*sendLEDInvokeRequest called in AppUI.cpp";
    cout << "*sendLEDInvokeRequest called in AppUI.cpp" << endl;

    // Created request for LED invocation
    InvokeRequest request;
    request.setTarget("blackberry.sample.service.led");
    request.setAction("test.action.FLASH_LED");
    request.setData(m_sleep.toAscii());
    cout << "send request" << endl;

    // Start the invocation
    InvokeTargetReply *targetReply = m_invokeManager.invoke(request);
    if (targetReply == 0) {
        cout << "***request failed" << endl;
    }
}
//! [3]

//! [4]
void AppUI::sendVibrateInvokeRequest()
{
    qDebug() << "*sendVibrateInvokeRequest called in AppUI.cpp";
    cout << "*sendVibrateInvokeRequest called in AppUI.cpp" << endl;

    // Created request for vibration invocation
    InvokeRequest request;
    request.setTarget("blackberry.sample.service.vibrate");
    request.setAction("test.action.VIBRATE");
    request.setData(m_sleep.toAscii());
    cout << "send request" << endl;

    // Start the invocation
    InvokeTargetReply *targetReply = m_invokeManager.invoke(request);
    if (targetReply == 0) {
        cout << "***request failed" << endl;
    }
}
//! [4]

void AppUI::killServiceRequest()
{
    qDebug() << "*exitRequest called in AppUI.cpp";
    cout << "*exitRequest called in AppUI.cpp" << endl;

    // Created request for vibration invocation
    InvokeRequest request;
    request.setTarget("blackberry.sample.service.exit");
    request.setAction("test.action.EXIT");
    request.setData(m_sleep.toAscii());
    cout << "send request" << endl;

    // Start the invocation
    InvokeTargetReply *targetReply = m_invokeManager.invoke(request);
    if (targetReply == 0) {
        cout << "***request failed" << endl;
    }
}

//! [5]
void AppUI::handleCheckedChange(bool newCheckedState)
{
    qDebug() << "*handleCheckedChange";

    // Toggle the sleep state on checkbox state change
    if (newCheckedState) {
        qDebug() << "**checked";
        m_sleep = "Sleep";
    } else {
        m_sleep = "NotSleep";
    }
}
//! [5]

