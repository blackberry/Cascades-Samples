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

#include <bb/cascades/ListView>
#include <bb/cascades/NavigationPane>
#include <bb/cascades/Page>
#include <bb/cascades/QmlDocument>

WeatherGuesserApp::WeatherGuesserApp()
{
    // We set up the application Organization and name, this is used by QSettings
    // when saving values to the persistent store, in this app the home page is kept in the settings.
    QCoreApplication::setOrganizationName("Example");
    QCoreApplication::setApplicationName("Weather Guesser");

    // Here we create a QMLDocument and load it, we are using build patterns.
    QmlDocument *qml = QmlDocument::create().load("main.qml");
    qml->setParent(this);

    if (!qml->hasErrors()) {

        // The application navigationPane is created from QML.
        mNavigation = qml->createRootNode<NavigationPane>();

        if (mNavigation) {
            // Create the cities and weather page, these are pages which is not part of the
            // NavigationPane, the application will handle navigation to these.
            createCitiesPage();
            createWeatherPage();

            // Set up the favorite and home page, load models and connects to the appropriate signals.
            setUpFavoritesPage();
            setUpHomePage();

            // Connect to custom signals that will trigger navigation, defined in QML.
            Page *continents = mNavigation->findChild<Page*>("continents");
            connect(continents, SIGNAL(showContinentCities(QString)), this,
                    SLOT(onShowContinentCities(QString)));

            Page *favorites = mNavigation->findChild<Page*>("favorites");
            connect(favorites, SIGNAL(showWeather(QString)), this, SLOT(onShowWeather(QString)));

            connect(mContinentCitiesPage, SIGNAL(showWeather(QString)), this,
                    SLOT(onShowWeather(QString)));

            // Finally the main scene for the application is set to NavigationPane.
            Application::setScene(mNavigation);
        }
    }
}

void WeatherGuesserApp::createCitiesPage()
{
    QmlDocument *qml = QmlDocument::create().load("ContinentCitiesPage.qml");

    if (!qml->hasErrors()) {

        mContinentCitiesPage = qml->createRootNode<Page>();

        if (mContinentCitiesPage) {

            // Set up a cities model for the page, this model will load different cities depending
            // on which continent is selected.
            ListView *citiesList = mContinentCitiesPage->findChild<ListView*>("citiesList");
            CityModel *cityModel = new CityModel(QStringList() << "name", "continents_connection",
                    this);
            citiesList->setDataModel(cityModel);

            // Connect to the continents page custom signal.
            Page *continents = mNavigation->findChild<Page*>("continents");
            connect(continents, SIGNAL(showContinentCities(QString)), cityModel,
                    SLOT(onChangeContinent(QString)));

            qml->documentContext()->setContextProperty("_navigation", mNavigation);
        }
    }
}

void WeatherGuesserApp::createWeatherPage()
{
    QmlDocument *qml = QmlDocument::create().load("WeatherPage.qml");

    if (!qml->hasErrors()) {

        mWeatherPage = qml->createRootNode<Page>();

        if (mWeatherPage) {

            // Set up a weather model the list, the model will load weather data
            ListView *weatherList = mWeatherPage->findChild<ListView*>("weatherList");
            WeatherModel *weatherModel = new WeatherModel(this);
            weatherList->setDataModel(weatherModel);

            // Connect the weather model to page signals that updates city for which model should be shown.
            connect(mContinentCitiesPage, SIGNAL(showWeather(QString)), weatherModel,
                    SLOT(onUpdateWeatherCity(QString)));

            Page *favorites = mNavigation->findChild<Page*>("favorites");
            connect(favorites, SIGNAL(showWeather(QString)), weatherModel,
                    SLOT(onUpdateWeatherCity(QString)));

            qml->documentContext()->setContextProperty("_navigation", mNavigation);
        }
    }
}

void WeatherGuesserApp::setUpFavoritesPage()
{
    Page *favPage = mNavigation->findChild<Page*>("favorites");

    // Set up a city model to load all cities marked as favorites.
    ListView *favList = favPage->findChild<ListView*>("favoritesList");
    CityModel *favModel = new CityModel(QStringList() << "name", "favorites_connection", this);
    favList->setDataModel(favModel);

    // Connect the favorites model to the signals for adding and removing cities as favorites.
    connect(favList, SIGNAL(removeFavoriteCity(QString, QVariant)), favModel,
            SLOT(onRemoveFavoriteCity(QString, QVariant)));

    ListView *citiesList = mContinentCitiesPage->findChild<ListView*>("citiesList");
    connect(citiesList, SIGNAL(newFavoriteCity(QString)), favModel,
            SLOT(onSetFavoriteCity(QString)));

    // Initial load of favorites list.
    favModel->loadFavoriteCities();
}

void WeatherGuesserApp::setUpHomePage()
{
    mHomeCityPage = mNavigation->findChild<Page*>("homeCityPage");

    // Set a weather model
    ListView *weatherList = mHomeCityPage->findChild<ListView*>("weatherList");
    WeatherModel *homeWeatherModel = new WeatherModel(this);
    weatherList->setDataModel(homeWeatherModel);

    // The home town can be set from two different lists, so we have to connect to
    // their signals
    ListView *favList = mNavigation->findChild<ListView*>("favoritesList");
    connect(favList, SIGNAL(updateHomeCity(QString)), homeWeatherModel,
            SLOT(onUpdateWeatherCity(QString)));

    ListView *citiesList = mContinentCitiesPage->findChild<ListView*>("citiesList");
    connect(citiesList, SIGNAL(updateHomeCity(QString)), homeWeatherModel,
            SLOT(onUpdateWeatherCity(QString)));

    // In addition we connect the application to the same signals so we can store the home town in the app settings.
    connect(favList, SIGNAL(updateHomeCity(QString)), this, SLOT(onUpdateHomeCity(QString)));
    connect(citiesList, SIGNAL(updateHomeCity(QString)), this, SLOT(onUpdateHomeCity(QString)));

    // Start loading weather data for the home page, if no home town is stored in
    // the application settings London is loaded as the home town.
    QSettings settings;
    QString homeTown = "London";
    if (!settings.value("homeCity").isNull()) {
        homeTown = settings.value("homeCity").toString();
    }
    homeWeatherModel->onUpdateWeatherCity(homeTown);
    mHomeCityPage->setProperty("city", QVariant(homeTown));
}

void WeatherGuesserApp::onUpdateHomeCity(QString city)
{
    // Store the home town in the application settings and set the hometown city property.
    QSettings settings;
    settings.setValue("homeCity", QVariant(city));
    mHomeCityPage->setProperty("city", QVariant(city));
}

void WeatherGuesserApp::onShowContinentCities(QString continent)
{
    // Navigate to the continent cities page and update the page property for presenting the continent.
    mNavigation->push(mContinentCitiesPage);
    mContinentCitiesPage->setProperty("continent", QVariant(continent));
}

void WeatherGuesserApp::onShowWeather(QString city)
{
    // Navigate to the weather page and update the page property for presenting the city.
    mNavigation->push(mWeatherPage);
    mWeatherPage->setProperty("city", QVariant(city));
}
