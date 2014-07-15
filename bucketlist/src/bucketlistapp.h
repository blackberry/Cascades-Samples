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

#ifndef _BUCKETLISTAPP_H_
#define _BUCKETLISTAPP_H_

#include <bb/cascades/Application>
#include <bb/system/InvokeManager>

namespace bb
{
    namespace cascades
    {
        class LocaleHandler;
    }
}

class QTranslator;

/**
 * BucketListApp Description:
 *
 * One hundred things to do before you kick the bucket. A relatively basic
 * list based application, which illustrates the usage of the multi select
 * handler. Moreover the application loads and saves data from a JSON file.
 * In addition to this the application illustrate how one can connect to the
 * BBM Social platform and update the users status message.
 *
 * You will learn:
 * - How to use the multiSelectHandler in order to select several list entries at once.
 * - How to one adds ActionSets and contextActions to a list items.
 * - How to load and save data from a JSON file.
 * - How to connect to the BBM Social Platform.
 * - How to register an application to listen for invocations.
 *
 */
class BucketListApp: public QObject
{
Q_OBJECT

/**
 * Property used for .buk items received via the invocation framework, see BucketListPage.qml
 * for further details.
 */
Q_PROPERTY(QString bucketItemTitle READ bucketItemTitle NOTIFY incomingBucketItem)

signals:
    /**
     * When the application receives a .buk file this signal is emitted, its captured
     * in QML and a UI is shown for adding the incoming item to the list.
     *
     */
    void incomingBucketItem();

public slots:
    /**
     * Function used to attach to the InvokeManagers invoked signal,
     * this is where the incoming .buk data is parsed and a new item is
     * added to the model.
     *
     * @param invoke - The incoming InvokeRequest
     */
    void handleInvoke(const bb::system::InvokeRequest& invoke);

public:
    // Constructor that sets up the application.
    BucketListApp(QObject *parent = 0);

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

    bb::system::InvokeManager *mInvokeManager;

    QString bucketItemTitle() const;

    QString mBucketItemTitle;

};

#endif // ifndef _BUCKETLISTAPP_H_
