/* Copyright (c) 2013, 2014 BlackBerry Limited.
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
#include "rundgangapp.h"
#include "common/customsqldatasource.h"
#include "common/globalsettings.h"
#include "common/emailcontroller.h"
#include "rundgang/photocontroller.h"
#include "rundgang/audiocontroller.h"

#include <bb/cascades/AbstractPane>
#include <bb/cascades/Application>
#include <bb/cascades/QmlDocument>
#include <bb/cascades/SceneCover>
#include <bb/cascades/ThemeSupport>
#include <bb/cascades/VisualStyle>

#include <bb/cascades/pickers/ContactPicker>
#include <bb/cascades/pickers/ContactSelectionMode>
#include <bb/cascades/pickers/SelectedContactAttribute>

using namespace bb::cascades;
using namespace bb::cascades::pickers;

RundGangApp::RundGangApp(QObject *parent) : QObject(parent)
{
    // Localization: Make the initial call to set up the initial application language and
    // connect to the LocaleHandlers systemLanguaged change signal, this will
    // tell the application when it is time to load a new set of language strings.
    mTranslator = new QTranslator(this);
    mLocaleHandler = new LocaleHandler(this);
    onSystemLanguageChanged();
    bool connectResult = connect(mLocaleHandler, SIGNAL(systemLanguageChanged()), SLOT(onSystemLanguageChanged()));
    Q_ASSERT(connectResult);
    Q_UNUSED(connectResult);

    // Register objects for assisting the Control of the Photo feedback page.
    qmlRegisterType<PhotoController>("com.rundgang", 1, 0, "PhotosController");
    qmlRegisterType<AudioController>("com.rundgang", 1, 0, "AudioController");
    qmlRegisterType<CustomSqlDataSource>("com.rundgang", 1, 0, "CustomSqlDataSource");
    qmlRegisterType<EmailController>("com.rundgang", 1, 0, "EmailController");

    // Registering picker types and Contact Picker helper object,
    // so that it can be used in QML.
    qmlRegisterType<ContactPicker>("bb.cascades.pickers", 1, 0, "ContactPicker");
    qmlRegisterUncreatableType<ContactSelectionMode>("bb.cascades.pickers", 1, 0,
            "ContactSelectionMode", "Non creatable enum type");

    // Create scene document from main.qml asset, the parent is set
    // to ensure the document gets destroyed properly at shut down.
    QmlDocument *qml = QmlDocument::create("asset:///main.qml").parent(this);

    // Create the GlobalSettings object that holds application wide settings.
    GlobalSettings *globalSettings = new GlobalSettings(this);
    qml->setContextProperty("_appSettings", globalSettings);

    // Make the application object accessible from QML.
    qml->setContextProperty("_app", this);

    // Set the stored visual style of the application.
    Application::instance()->themeSupport()->setVisualStyle(globalSettings->visualStyle());

    // Create root object for the UI.
    AbstractPane *root = qml->createRootObject<AbstractPane>();

    // Set created root object as the application scene.
    Application::instance()->setScene(root);

    // Add an application cover, shown when the app is running in minimized mode.
    addApplicationCover();
}


void RundGangApp::addApplicationCover() {
    // Create the Application Cover document from its QML file.
    QmlDocument *qmlCover = QmlDocument::create("asset:///Common/AppCover.qml").parent(this);

    if (!qmlCover->hasErrors()) {
        // Create the QML Container from using the QMLDocument.
        Container *coverContainer = qmlCover->createRootObject<Container>();

        // Create a SceneCover and set the application cover
        SceneCover *sceneCover = SceneCover::create().content(coverContainer);
        Application::instance()->setCover(sceneCover);
    }
}

void RundGangApp::onSystemLanguageChanged()
{
    // As the system language is changed remove the previous translator object.
    QCoreApplication::instance()->removeTranslator(mTranslator);

    // Initiate, load and install the application translation files corresponding
    // to the current locale.
    QString localeString = QLocale().name();
    QString fileName = QString("rundgang_%1").arg(localeString);
    if (mTranslator->load(fileName, "app/native/qm")) {
        QCoreApplication::instance()->installTranslator(mTranslator);
    }
}
