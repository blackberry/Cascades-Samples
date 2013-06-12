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
#ifndef QUOTESAPP_H
#define QUOTESAPP_H

#include <bb/cascades/Application>
#include <QObject>

using namespace bb::cascades;

namespace bb
{
    namespace cascades
    {
        class LocaleHandler;
    }
}

class QTranslator;

/**
 * QuotesApp Description:
 *
 * QuotesApp is a sample application that shows you how to deal with dynamic data in
 * a list. It displays a list with some of our favorite sayings from clever
 * people when it comes to programming and technology. The list of quotes
 * is stored in an SQL database, but you will also be able to add your own quotes or
 * delete the ones you don't agree with (or edit them to better fit you purpose).
 *
 * You will learn how to:
 * - Set up a ListView with predefined item visuals
 * - Load data with the SqlDataAccess
 * - Sorting of with the help of a GroupDataModel
 * - Adding, updating and deleting items from a list
 */
class QuotesApp: public QObject
{
Q_OBJECT

public:

    // This is our constructor that sets up the recipe.
    QuotesApp();
    ~QuotesApp();

    /**
     * This is the application start-up function that
     * initiates the database helper object and load the QML scene
     */
    void onStart();

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
     * This function loads the QML and sets the application scene.
     *
     * @return True if all set up was successful, otherwise false is returned.
     */
    bool loadQMLScene();
};

#endif // ifndef QUOTESAPP_H
