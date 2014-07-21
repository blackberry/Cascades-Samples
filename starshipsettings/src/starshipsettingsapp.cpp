/* Copyright (c) 2012, 2013, 2014 BlackBerry Limited.
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
#include "starshipsettingsapp.h"

#include <bb/cascades/AbstractPane>
#include <bb/cascades/LocaleHandler>
#include <bb/cascades/QmlDocument>
#include "appsettings.h"

using namespace bb::cascades;

StarshipSettingsApp::StarshipSettingsApp(QObject *parent) : QObject(parent)
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

    // Then we load the application.
    QmlDocument *qml = QmlDocument::create("asset:///main.qml").parent(this);

    if (!qml->hasErrors()) {

        // Make the application settings object available to QML, all application wide
        // settings are managed via properties in this object.
        qml->setContextProperty("appSettings", new AppSettings(this));

        AbstractPane *appPane = qml->createRootObject<AbstractPane>();

        if (appPane) {
            Application::instance()->setScene(appPane);
        }
    }
}

StarshipSettingsApp::~StarshipSettingsApp()
{
}

void StarshipSettingsApp::onSystemLanguageChanged()
{
    // Remove the old translator to make room for the new translation.
    QCoreApplication::instance()->removeTranslator(mTranslator);

    // Initiate, load and install the application translation files.
    QString localeString = QLocale().name();
    QString fileName = QString("starshipsettings_%1").arg(localeString);
    if (mTranslator->load(fileName, "app/native/qm")) {
        QCoreApplication::instance()->installTranslator(mTranslator);
    }
    qDebug() << "language " << localeString;
}
