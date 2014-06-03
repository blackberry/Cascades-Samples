/*
 * Copyright (c) 2011-2013 BlackBerry Limited.
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

#include <bb/cascades/Application>

#include <QLocale>
#include <QTranslator>
#include <bb/platform/identity/IdentityService>
#include <bb/platform/identity/IdentityServiceProvider>
#include <bb/platform/identity/IdentityServiceResult>
#include <bb/platform/identity/IdentityServicePropertyResult>
#include <bb/platform/identity/BlackBerryId>
#include <bb/platform/identity/BlackBerryIdPropertyType>
#include <bb/platform/identity/BlackBerryProfile>
#include <bb/platform/identity/BlackBerryProfilePropertyType>
#include <bb/platform/identity/BlackBerryProfileGetDataFlag>
#include <bb/platform/identity/BlackBerryProfileSetDataFlag>
#include <bb/platform/identity/BlackBerryProfileCreateDataFlag>

#include "applicationui.hpp"

#include <Qt/qdeclarativedebug.h>

using namespace bb::cascades;
using namespace bb::platform::identity;


Q_DECL_EXPORT int main(int argc, char **argv)
{
    Application app(argc, argv);
//! [0]
    // Register Identity Service classes for use in QML
    qmlRegisterType<IdentityService>("bb.IDS_QML_Sample", 1, 0, "IdentityService");
    qmlRegisterType<IdentityServiceProvider>("bb.IDS_QML_Sample", 1, 0, "IdentityServiceProvider");

    // Register Provider classes for use in QML
    qmlRegisterType<BlackBerryId>("bb.IDS_QML_Sample", 1, 0, "BlackBerryId");
    qmlRegisterType<BlackBerryProfile>("bb.IDS_QML_Sample", 1, 0, "BlackBerryProfile");

    // Register enums for use in QML
    qmlRegisterUncreatableType<bb::platform::identity::IdentityServiceResult>("bb.IDS_QML_Sample", 1, 0, "IdentityServiceResult", "");
    qmlRegisterUncreatableType<bb::platform::identity::IdentityServicePropertyResult>("bb.IDS_QML_Sample", 1, 0, "IdentityServicePropertyResult", "");

    qmlRegisterUncreatableType<bb::platform::identity::BlackBerryIdPropertyType>("bb.IDS_QML_Sample", 1, 0, "BlackBerryIdPropertyType", "");

    qmlRegisterUncreatableType<bb::platform::identity::BlackBerryProfilePropertyType>("bb.IDS_QML_Sample", 1, 0, "BlackBerryProfilePropertyType", "");
    qmlRegisterUncreatableType<bb::platform::identity::BlackBerryProfileGetDataFlag>("bb.IDS_QML_Sample", 1, 0, "BlackBerryProfileGetDataFlag", "");
    qmlRegisterUncreatableType<bb::platform::identity::BlackBerryProfileSetDataFlag>("bb.IDS_QML_Sample", 1, 0, "BlackBerryProfileSetDataFlag", "");
    qmlRegisterUncreatableType<bb::platform::identity::BlackBerryProfileCreateDataFlag>("bb.IDS_QML_Sample", 1, 0, "BlackBerryProfileCreateDataFlag", "");
    qmlRegisterUncreatableType<bb::platform::identity::IdentityServiceResult>("bb.IDS_QML_Sample", 1, 0, "IdentityServiceResult", "");
//! [0]
    // Create the Application UI object, this is where the main.qml file
    // is loaded and the application scene is set.
    ApplicationUI appui;

    // Enter the application main event loop.
    return Application::exec();
}
