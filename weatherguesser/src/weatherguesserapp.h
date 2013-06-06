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

namespace bb
{
    namespace cascades
    {
        class LocaleHandler;
        class QmlDocument;
    }
}

class QTranslator;

/**
 * WeatherGuesserApp Description:
 *
 * Weather guessing has been a human activity that has gone on for
 * thousands of years. Nowadays there are even television programs
 * devoted to this phenomenon. The main objective of this app, apart
 * from producing more or less accurate weather forecasts, is to
 * show how to go about setting up an application using tabbed and
 * navigation panes. It also shows how to load data using both SQL
 * and non-local JSON feeds.
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
    // This is our constructor that sets up the recipe.
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
     * System Language function, which will be attached to the signal emitted when
     * the system language change (for example if it is changed in the settings menu);
     */
    Q_SLOT void onSystemLanguageChanged();

    // Qt translator object used for loading translations.
    QTranslator* mTranslator;

    // The Locale handler used to query and listens for changes to system locales.
    bb::cascades::LocaleHandler* mLocaleHandler;

    /**
     * This function creates the model that is used to present a list of cities for different continents.
     */
    void createCitiesModel();

    /**
     * This function creates the model that is used to presenting weather guessing data.
     */
    void createWeatherModel();

    /**
     * This function initializes the favorite model which is used for showing the cities marked as
     * favorites in the city database.
     */
    void createFavoritesModel();

    /**
     * This function initializes the home model which is used for showing the home city weather report.
     */
    void createHomeModel();

    bb::cascades::QmlDocument *mQmlDocument;
};

#endif // ifndef WEATHERGUESSERAPP_H
