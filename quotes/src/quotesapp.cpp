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
#include "quotesapp.h"
#include "customsqldatasource.h"

#include <bb/cascades/AbstractPane>
#include <bb/cascades/LocaleHandler>
#include <bb/cascades/QmlDocument>

using namespace bb::cascades;

QuotesApp::QuotesApp()
{
}

QuotesApp::~QuotesApp()
{
}

void QuotesApp::onStart()
{
    DataSource::registerQmlTypes();

    if (!loadQMLScene()) {
        qWarning("Failed to load QML scene.");
    }
}

bool QuotesApp::loadQMLScene()
{
    // Prepare localization.Connect to the LocaleHandlers systemLanguaged change signal, this will
    // tell the application when it is time to load a new set of language strings.
    mTranslator = new QTranslator(this);
    mLocaleHandler = new LocaleHandler(this);
    connect(mLocaleHandler, SIGNAL(systemLanguageChanged()), SLOT(onSystemLanguageChanged()));
    onSystemLanguageChanged();

    // Register the SQL data source, so that it can be set up in QML
    qmlRegisterType < CustomSqlDataSource > ("com.quotes.data", 1, 0, "CustomSqlDataSource");

    // Create a QML object and load it, using build patterns.
    QmlDocument *qmlDocument = QmlDocument::create("asset:///main.qml");

    if (!qmlDocument->hasErrors()) {

        // The root Container is created from QML.
        AbstractPane* navigationPane = qmlDocument->createRootObject<AbstractPane>();

        // Finally the main scene for the application is set to this Control.
        Application::instance()->setScene(navigationPane);
        return true;

    }

    return false;
}

void QuotesApp::onSystemLanguageChanged()
{
    // Remove the old translator to make room for the new translation.
    QCoreApplication::instance()->removeTranslator(mTranslator);

    // Initiate, load and install the application translation files.
    QString localeString = QLocale().name();
    QString fileName = QString("quotes_%1").arg(localeString);
    if (mTranslator->load(fileName, "app/native/qm")) {
        QCoreApplication::instance()->installTranslator(mTranslator);
    }
}
