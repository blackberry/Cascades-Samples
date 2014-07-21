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
#ifndef RUNDGANGAPP_HPP_
#define RUNDGANGAPP_HPP_

#include <QObject>
#include <bb/cascades/Application>
#include <bb/cascades/LocaleHandler>

namespace bb
{
    namespace cascdes
    {
        class Application;
        class LocaleHandler;
    }
}

class QTranslator;

/**
 * RundGangApp Description:
 *
 * Rundgang means feedback in Swedish, well to be completely honest its
 * the kind of feedback that appears when the soundguy is not doing his
 * job. But feedback is the main topic of this app, it shows how to set up
 * an email with a list of addresses and attach different kinds of files
 * to it. In addition it also illustrates application structure for a QML
 * based app.
 *
 * You will learn how to:
 * - Launch the email card with pre-popopulted data.
 * - Collect data from, an AudioRecoder and Camera (photo and QR code).
 * - Create an application menu that can be accessed anywhere.
 * - Use data sources to populate  GroupDataModels.
 * - One way to handle un-granted permissions.
 * - How a QML based application can be structured with C++ helper objects.
 */
class RundGangApp: public QObject
{
    Q_OBJECT

public:
    /**
     * Constructor; sets up the application
     * @param app A pointer to the application object.
     */
    RundGangApp(QObject *parent = 0);
    virtual ~RundGangApp(){}

private slots:
    /**
     * System Language function, which will be attached to the signal emitted when
     * the system language change (for example if it is changed in the settings menu);
     */
    void onSystemLanguageChanged();

private:
    /**
     * Function used to create and set the SceneCover of the application,
     * the cover is shown when the app is running in minimized mode.
     */
    void addApplicationCover();

    // Qt translator object used for loading translations.
    QTranslator* mTranslator;

    // The Locale handler used to query and listens for changes to system locales.
    bb::cascades::LocaleHandler* mLocaleHandler;
};

#endif /* RUNDGANGAPP_HPP_ */
