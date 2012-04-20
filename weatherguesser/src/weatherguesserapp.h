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
#ifndef WEATHERGUESSERAPP_H
#define WEATHERGUESSERAPP_H

#include <bb/cascades/Application>

using namespace bb::cascades;

namespace bb
{
    namespace cascades
    {
        class NavigationPane;
        class Page;
    }
}

/**
 * WeatherGuesserApp
 *
 * Weather guessing has been a human activity that has gone on for
 * thousands of years. Nowadays there are even television programs
 * devoted to this phenomenon. The main objective of this app, apart
 * from producing more or less accurate weather forecasts, is to
 * show how one go about setting up an application using tabbed and
 * navigation panes. It also shows how to load data using both SQL
 * and nonlocal JSON feeds.
 *
 * You will learn:
 * - How to use the NavigationPane and TabbedPane.
 * - How to populate a list using a GroupDataModel and JSON data
 * - How to populate a list using a GroupDataModel and SQL, both asynchronously and synchronously.
 */
class WeatherGuesserApp: public QObject
{
Q_OBJECT

public:
    WeatherGuesserApp();

public slots:
    /**
     * This function navigates to the continent cites page which presents a list
     * with a few selected cities from that continent.
     *
     * @param continent The continent for which cities should be shown.
     */
    void onShowContinentCities(QString continent);

    /**
     * This function navigates to the weather page, which presents a prediction
     * of the weather for the rest of the year for a specific city.
     *
     * @param city The city that the weather forecast will be shown.
     */
    void onShowWeather(QString city);

    /**
     * This function updates the city on the Home City weather tab.
     *
     * @param city This is the new home weather city.
     */
    void onUpdateHomeCity(QString city);

private:

    /**
     * Creates a page that is used to present a list of cities for different continents.
     */
    void createCitiesPage();

    /**
     * Creates a page that is used to present weather guessing data.
     */
    void createWeatherPage();

    /**
     * Initializes the favorite page, connects to signals and creates a model
     * for showing the cities marked as favorites in the city data base.
     */
    void setUpFavoritesPage();

    /**
     * Initializes the favorite page, connects to signals and creates a model
     * for showing the home weather.
     */
    void setUpHomePage();

    NavigationPane *mNavigation;
    Page *mContinentCitiesPage;
    Page *mWeatherPage;
    Page *mHomeCityPage;
};

#endif // ifndef WEATHERGUESSERAPP_H
