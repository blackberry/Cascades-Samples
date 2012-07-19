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
        class QmlDocument;
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
    ~WeatherGuesserApp();

public slots:

    /**
     * This function updates the city in the application settings.
     *
     * @param city This is the new home weather city.
     */
    void onUpdateHomeCity(QString city);

private:

    /**
     * Creates model that is used to present a list of cities for different continents.
     */
    void createCitiesModel();

    /**
     * Creates a model that is used to presenting weather guessing data.
     */
    void createWeatherModel();

    /**
     * Initializes the favorite model, used for showing the cities marked as
     * favorites in the city data base.
     */
    void createFavoritesModel();

    /**
     * Initializes the home model, used for showing the home city weather report.
     */
    void createHomeModel();

    QmlDocument *mQmlDocument;
};

#endif // ifndef WEATHERGUESSERAPP_H
