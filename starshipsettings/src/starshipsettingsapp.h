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
#ifndef STARSHIPSETTINGSAPP_H
#define STARSHIPSETTINGSAPP_H

#include <bb/cascades/Application>

namespace bb
{
    namespace cascades
    {
        class LocaleHandler;
    }
}

class QTranslator;
/**
 * StarshipSettings Description:
 *
 * This sample application illustrates how to keep your applications current state
 * via QSettings.
 *
 * Feature summary
 * - setting up and communicating with QSettings
 */

class StarshipSettingsApp: public QObject
{
    Q_OBJECT

public:
    // This is our constructor that sets up the recipe.
    StarshipSettingsApp(QObject *parent = 0);
    ~StarshipSettingsApp();

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
};

#endif // ifndef STARSHIPSETTINGSAPP_H
