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
#include "weatherguesserapp.h"
#include "citymodel.h"
#include "weathermodel.h"

#include <bb/cascades/LocaleHandler>
#include <bb/cascades/TabbedPane>
#include <bb/cascades/QmlDocument>

using namespace bb::cascades;

WeatherGuesserApp::WeatherGuesserApp()
{
    // Set up the application organization and name (used by QSettings
    // when saving values to the persistent store)
    // In this application, the home page is kept in the settings.
    QCoreApplication::setOrganizationName("Example");
    QCoreApplication::setApplicationName("Weather Guesser");

    // Prepare localization.Connect to the LocaleHandlers systemLanguaged change signal, this will
    // tell the application when it is time to load a new set of language strings.
    mTranslator = new QTranslator(this);
    mLocaleHandler = new LocaleHandler(this);
    connect(mLocaleHandler, SIGNAL(systemLanguageChanged()), SLOT(onSystemLanguageChanged()));
    onSystemLanguageChanged();

    // Create a QMLDocument and load it, using build patterns.
    mQmlDocument = QmlDocument::create("asset:///main.qml");
    mQmlDocument->setParent(this);

    // Create the cities and weather model (these are pages which is not part of the
    // NavigationPane, so the application will need handle navigating to them).
    createCitiesModel();
    createWeatherModel();

    // Setup the favorite page, setup the home page, load the models, and connect to the appropriate signals.
    createFavoritesModel();
    createHomeModel();

    if (!mQmlDocument->hasErrors()) {

        // The application navigationPane is created from QML.
        TabbedPane *tabs = mQmlDocument->createRootObject<TabbedPane>();

        if (tabs) {
            // Set the main application scene to NavigationPane.
            Application::instance()->setScene(tabs);
        }
    }
}

WeatherGuesserApp::~WeatherGuesserApp()
{

}

void WeatherGuesserApp::createCitiesModel()
{
    // Create and bind the cityModel, so that it can be accessed in the ContinentCitiesPage.QML
    // via the _cityModel context property
    CityModel *cityModel = new CityModel(QStringList() << "name", "continents_connection", this);
    mQmlDocument->setContextProperty("_cityModel", cityModel);
}

void WeatherGuesserApp::createWeatherModel()
{
    // Create a WeatherModel that will load a weather forecast based on its
    // city property (see WeatherPage.qml and FavoritePage.qml).
    WeatherModel *weatherModel = new WeatherModel(this);
    mQmlDocument->setContextProperty("_weatherModel", weatherModel);
}

void WeatherGuesserApp::createFavoritesModel()
{
    // Create a CityModel that will load the favorite cities which are presented
    // in a list from FavoritePage.qml.
    CityModel *favoriteModel = new CityModel(QStringList() << "name", "favorites_connection", this);
    mQmlDocument->setContextProperty("_favoriteModel", favoriteModel);

    // Load the initial the favorites list
    favoriteModel->loadFavoriteCities();
}

void WeatherGuesserApp::createHomeModel()
{
    // The Home page is a special case for the WeatherModel and is set to be used
    // on the first tab in main.qml (see also WeatherItem.qml).
    WeatherModel *homeModel = new WeatherModel(this);
    mQmlDocument->setContextProperty("_homeModel", homeModel);

    // Connect to the homeModel cityChanged signal in order to update the application
    // settings for the home city (so it will be set on the next time the app launches)
    connect(homeModel, SIGNAL(cityChanged(QString)), this, SLOT(onUpdateHomeCity(QString)));

    // Begin loading weather data for the home page, if no hometown is stored in
    // the application settings, London is loaded as the hometown
    QSettings settings;
    QString homeTown = "London";
    if (!settings.value("homeCity").isNull()) {
        homeTown = settings.value("homeCity").toString();
    }

    homeModel->setCity(homeTown);
}

void WeatherGuesserApp::onUpdateHomeCity(QString city)
{
    // Store the hometown in the application settings and set the hometown city property
    QSettings settings;
    settings.setValue("homeCity", QVariant(city));
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
