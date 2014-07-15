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
#include "weatherguesserapp.h"
#include "appsettings.h"

#include "common/loadmodeldecorator.h"
#include "common/pulltorefresh.h"
#include "common/sqlheaderdataqueryex.h"
#include "common/weathererror.h"

#include "data/weatherdatasource.h"
#include "data/citydatasource.h"

#include <bb/cascades/LocaleHandler>
#include <bb/cascades/AbstractPane>
#include <bb/cascades/QmlDocument>

using namespace bb::cascades;

WeatherGuesserApp::WeatherGuesserApp(QObject *parent) : QObject(parent)
{
    // Register QML types, so they can be used in QML.
    qmlRegisterType<SqlHeaderDataQueryEx>("bb.cascades.datamanager", 1, 2, "SqlHeaderDataQueryEx");
    qmlRegisterType<PullToRefresh>("com.weather", 1, 0, "PullToRefresh");
    qmlRegisterType<CityDataSource>("com.weather", 1, 0, "CityDataSource");
    qmlRegisterType<LoadModelDecorator>("com.weather", 1, 0, "LoadModelDecorator");
    qmlRegisterType<WeatherDataSource>("com.weather", 1, 0, "WeatherDataSource");
    qmlRegisterUncreatableType<WeatherError>("com.weather", 1, 0, "WeatherError", "Uncreatable type");

    // Prepare localization. Connect to the LocaleHandlers systemLanguaged change signal, this will
    // tell the application when it is time to load a new set of language strings.
    mTranslator = new QTranslator(this);
    mLocaleHandler = new LocaleHandler(this);
    onSystemLanguageChanged();
    bool connectResult = connect(mLocaleHandler, SIGNAL(systemLanguageChanged()), SLOT(onSystemLanguageChanged()));
    Q_ASSERT(connectResult);
    Q_UNUSED(connectResult);

    // Create a QMLDocument and load it, using build patterns.
    QmlDocument *qmlDocument = QmlDocument::create("asset:///main.qml").parent(this);

    if (!qmlDocument->hasErrors()) {
        // Make the settings object available to QML
        qmlDocument->setContextProperty("_appSettings", new AppSettings(this));

        // The application navigationPane is created from QML.
        AbstractPane *appPane = qmlDocument->createRootObject<AbstractPane>();

        if (appPane) {
            // Set the main application scene to NavigationPane.
            Application::instance()->setScene(appPane);
        }
    }
}

WeatherGuesserApp::~WeatherGuesserApp()
{
}

void WeatherGuesserApp::onSystemLanguageChanged()
{
    // Remove the old translator to make room for the new translation.
    QCoreApplication::instance()->removeTranslator(mTranslator);

    // Initiate, load and install the application translation files.
    QString localeString = QLocale().name();
    QString fileName = QString("weatherguesser_%1").arg(localeString);
    if (mTranslator->load(fileName, "app/native/qm")) {
        QCoreApplication::instance()->installTranslator(mTranslator);
    }
}
