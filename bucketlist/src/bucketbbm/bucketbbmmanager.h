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

#ifndef BUCKETBBMMANAGER_H_
#define BUCKETBBMMANAGER_H_

#include <QObject>

#include "bucketbbm/registrationhandler.h"
#include "bucketbbm/profilebox.h"
#include "bucketbbm/profilemessage.h"

namespace bucketbbm
{
    /**
     * BucketBBMManager Description:
     *
     * This manager takes care both of registering the application with BBM
     * and updating the status of the BBM profile. It can be used from QML by
     * registering it as a type.
     */
    class BucketBBMManager: public QObject
    {
    Q_OBJECT

    /**
     *  Property that holds the application registered status text.
     */
    Q_PROPERTY(QString registerStatusText READ registerStatusText NOTIFY registerStatusTextChanged)

    /**
     *  Property that holds the registered status, true if the application is registered with BBM.
     */
    Q_PROPERTY(bool appRegistered READ appRegistered NOTIFY appRegisteredChanged)

    public:
        // Constructor that sets up the manager.
        BucketBBMManager(QObject *parent = 0);

        // Destructor that sets up the manager.
        virtual ~BucketBBMManager();

        /**
         * A call to this function will attempt to register the application
         * with BBM using a specified application id (uuid).
         */
        Q_INVOKABLE void registerApp();

        /**
         * This function adds a message to the application profile box and
         * updates the status message of the BBM associated user profile.
         */
        Q_INVOKABLE void updateStatus(const QString message, const QString icon);

        /**
         * The current registration process status text, returns an empty string
         * if the application has not been registered.
         *
         * @return A status text
         */
        QString registerStatusText();

        /**
         * The application registration property, true if successfully registered otherwise
         * false. If the application is registered it is possible to update the user status
         * text.
         *
         * @return true if application is registered with BBM otherwise false.
         */
        bool appRegistered();

    signals:
        /**
         * Emitted when  appRegistered changes.
         *
         * @param appRegistered The new registration status.
         */
        void appRegisteredChanged(bool appRegistered);

        /**
         * Emitted when the status of the registration changes, typically this
         * happens when something fails during the registration of the application
         * to BBM.
         *
         * @param registerStatusText The status text
         */
        void registerStatusTextChanged(QString registerStatusText);

    private slots:
        /**
         * Function that is connected to the registration handler for
         * relaying updates on the registration process.
         *
         * @param registered The current application registration true if registered otherwise false.
         */
        void onRegisteredChanged(bool registered);

        /**
         * Function that is connected to the registration handler for
         * relaying updates on the registration status.
         *
         * @param statusText The current registration status string.
         */
        void onStatusTextChanged(QString statusText);

    private:
        // Objects needed for handling communication with BBM.
        bucketbbm::RegistrationHandler *mRegistrationHandler;
        bucketbbm::ProfileBox *mProfileBox;
        bucketbbm::ProfileMessage *mProfileMessage;

    };
}
#endif /* BUCKETBBMMANAGER_H_ */
