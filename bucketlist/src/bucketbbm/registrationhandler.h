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

#ifndef REGISTRATIONHANDLER_H_
#define REGISTRATIONHANDLER_H_

#include <bb/platform/bbm/Context>
#include <bb/platform/bbm/RegistrationState>
#include <QObject>

using namespace bb::platform::bbm;

namespace bb
{
    namespace platform
    {
        namespace bbm
        {
            class Context;
        }
    }
}

namespace bucketbbm
{
    /**
     * RegistrationHandler Description:
     * 
     * RegistrationHandler is class that handles the registration of 
     * an application for usage with the BBM social platform.
     */
    class RegistrationHandler: public QObject
    {
    Q_OBJECT

    /**
     *  Property that holds the connection status text.
     */
    Q_PROPERTY(QString statusText READ statusText WRITE setStatusText NOTIFY statusTextChanged)

    /**
     *  Property that holds the registered status .
     */
    Q_PROPERTY(bool registered READ registered NOTIFY registeredChanged)

    public:
        RegistrationHandler(QObject *parent = 0);

        // Enumerates the possible registration progress states.
        struct BbmRegistrationProgress
        {
            enum Type
            {
                /**
                 * Registration has not started and has never been attempted since
                 * the application started.
                 */
                NotStarted = 0,
                // Registration has started.
                Started,
                // Registration is in progress.
                Pending,
                /**
                 * Registration is done. Use isRegistered() or
                 * Context::isAccessAllowed() to check if the application is
                 * registered successfully.
                 */
                Finished
            };
        };

        /**
         * A call to this function will attempt to register the application
         * with BBM using a specified application id (uuid).
         */
        void registerApp();

        void registrationFinished();

        /**
         * Sets the status text property.
         *
         * @param newStatusText the new string
         */
        void setStatusText(const QString newStatusText);

        /**
         * The current registration process status text, returns an empty string
         * if the application has not been registered.
         *
         * @return A status text
         */
        QString statusText();

        /**
         * The application registration property, true if successfully registered otherwise
         * false. If the application is registered it is possible to update the user status
         * text.
         *
         * @return true if application is registered with BBM otherwise false.
         */
        bool registered();

    signals:

        /**
         * Emitted when the status of the registration changes, typically this
         * happens when something fails during the registration of the application
         * to BBM.
         *
         * @param statusTextChanged The status text
         */
        void statusTextChanged(QString statusTextChanged);

        /**
         * Emitted when  registered changes.
         *
         * @param registered The new registration status.
         */
        void registeredChanged(bool registered);

    private slots:

        /**
         * Function used when connecting to the registrationStateUpdated signal
         * from the BBM Context.
         *
         * @param RegistrationState An enum with information on the registration status
         */
        void registrationStatus(bb::platform::bbm::RegistrationState::Type state);

    private:

        /**
         * Helper function used to update the status text associated with a failed
         * registration attempt.
         */
        void registrationFailed();

        Context* mContext;
        QString mUuid;

        QString mStatusText;
        bool mRegistered;

        /**
         * Registration progress. Use this to check if you have already attempted
         * registration, if registration has finished, or it's still in progress.
         */
        BbmRegistrationProgress::Type mProgress;
    };
}

#endif /* REGISTRATIONHANDLER_H_ */
