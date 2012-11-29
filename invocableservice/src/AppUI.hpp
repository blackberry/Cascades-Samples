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

#ifndef APPUI_HPP
#define APPUI_HPP

#include <bb/system/InvokeManager>

#include <QtCore/QObject>
#include <QtCore/QString>

class Service;

class AppUI  : public QObject
{
    Q_OBJECT

public:
    AppUI(QObject *parent = 0);

public Q_SLOTS:
    // This method is called when sound button is clicked
    void sendSoundInvokeRequest();

    // This method is called when LED button is clicked
    void sendLEDInvokeRequest();

    // This method is called when vibrate button is called
    void sendVibrateInvokeRequest();

    // This method is called when service kill button is called
    void killServiceRequest();

    // This method is called when the checkbox state changes
    void handleCheckedChange(bool newCheckedState);

private:
    QString m_sleep;
    Service *m_service;
    bb::system::InvokeManager m_invokeManager;
};

#endif
