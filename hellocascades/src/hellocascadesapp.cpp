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
#include "hellocascadesapp.h"

#include <bb/cascades/AbstractPane>
#include <bb/cascades/LocaleHandler>
#include <bb/cascades/QmlDocument>

using namespace bb::cascades;

HelloCascadesApp::HelloCascadesApp(QObject *parent) :
        QObject(parent)
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

    // Obtain a QMLDocument and load it into the QML variable, using build patterns.
    QmlDocument *qml = QmlDocument::create("asset:///hellocascades.qml").parent(this);

    // If the QML document is valid, we process it.
    if (!qml->hasErrors()) {

        // Create the application Pane from QMLDocument, in this case it is a single Page.
        AbstractPane *appPane = qml->createRootObject<AbstractPane>();

        if (appPane) {
            // Set the main scene for the application to the Page.
            Application::instance()->setScene(appPane);
        }
    }
}

void HelloCascadesApp::onSystemLanguageChanged()
{
    // Remove the old translator to make room for the new translation.
    QCoreApplication::instance()->removeTranslator(mTranslator);

    // Initiate, load and install the application translation files.
    // In order for strings in QML to be updated while the app is running
    // Retranslate.onLanguageChanged has to be added to the string (see hellocasacdes.qml).
    QString localeString = QLocale().name();
    QString fileName = QString("hellocascades_%1").arg(localeString);
    if (mTranslator->load(fileName, "app/native/qm")) {
        QCoreApplication::instance()->installTranslator(mTranslator);
    }
}
