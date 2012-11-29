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

#include "registrationhandler.h"
#include <bb/platform/bbm/Context>
#include <bb/platform/bbm/RegistrationState>

#include "global.h"
#include "bucketlistapp.h"

namespace bucketbbm
{

    RegistrationHandler::RegistrationHandler(QObject *parent) :
            QObject(parent)
    {
        // Attempt to register the application with the following UUID.
        // This UUID is for the BBM SP.  It’s an identifier that uniquely identifies the app to the infrastructure.
        // This is used mainly when communicating between two applications using BBM.
        // This UUID is only used when side loading the app.  Once it’s submitted to App World,
        // AW supplies the UUID.  So it should be it will need to be updated once a reak id is given.
        // Meanwhile you can define your own UUID here.  You can generate one here: http://www.guidgenerator.com/
        mUuid = QString::fromUtf8("b540c0af-c71b-4f04-bdf3-a77463d29f7c"); //original
    }

    void RegistrationHandler::registerApp()
    {
        mContext = new bb::platform::bbm::Context(QUuid(mUuid));
        Global::instance()->setContext(mContext);

        // Connect the BBM SP registration signals to our application's slot.
        QObject::connect(mContext,
                SIGNAL(registrationStateUpdated(bb::platform::bbm::RegistrationState::Type)), this,
                SLOT(registrationStatus(bb::platform::bbm::RegistrationState::Type)));

        // Request registration and updates will be handled in the registrationStatus function.
        mContext->requestRegisterApplication();
    }

    void RegistrationHandler::registrationStatus(bb::platform::bbm::RegistrationState::Type state)
    {
        qDebug() << "Registration Status received";

        switch (state) {
            case bb::platform::bbm::RegistrationState::Unregistered:
            case bb::platform::bbm::RegistrationState::Pending:
                break;
            case bb::platform::bbm::RegistrationState::Allowed:

                // Success! Update the registered property and emit the changed signal
                // so that the application can take the appropriate action.
                mRegistered = true;
                emit registeredChanged(mRegistered);

                break;

            // We count all the below as failures to connect the application to
            // the BBM social platform.
            case bb::platform::bbm::RegistrationState::Unknown:
                qDebug() << ("Unknown failure");
                this->registrationFailed();
                break;
            case bb::platform::bbm::RegistrationState::BlockedByUser:
                qDebug() << ("Access failed: blocked by user");
                this->registrationFailed();
                break;

            case bb::platform::bbm::RegistrationState::BlockedByRIM:
                qDebug() << ("Access failed: blocked by RIM");
                this->registrationFailed();
                break;

            case bb::platform::bbm::RegistrationState::NoDataConnection:
                qDebug() << ("Access failed: no data coverage");
                this->registrationFailed();
                break;

            case bb::platform::bbm::RegistrationState::UnexpectedError:
                qDebug() << ("Access failed: unexpected error");
                this->registrationFailed();
                break;

            case bb::platform::bbm::RegistrationState::InvalidUuid:
                qDebug() << ("Access failed: invalid UUID");
                this->registrationFailed();
                break;

            case bb::platform::bbm::RegistrationState::TemporaryError:
                qDebug() << ("Access failed: temporary error");
                this->registrationFailed();
                break;

            case bb::platform::bbm::RegistrationState::CancelledByUser:
                qDebug() << ("Access cancelled by user");
                this->registrationFailed();
                break;

            case bb::platform::bbm::RegistrationState::MaxDownloadsReached:
                qDebug() << ("Access failed: max downloads reached");
                this->registrationFailed();
                break;

            case bb::platform::bbm::RegistrationState::MaxAppsReached:
                qDebug() << ("Access failed: max apps reached");
                this->registrationFailed();
                break;

            case bb::platform::bbm::RegistrationState::Expired:
                qDebug() << ("Access failed: access expired");
                this->registrationFailed();
                break;

            default:
                qDebug() << ("Access failed: other errors");
                this->registrationFailed();
                break;
        }
    }

    void RegistrationHandler::registrationFailed()
    {
        // A generic error message is provided here.
        // You could provide a different error for each failure code to instruct the
        // user on how to continue.
        setStatusText( "BBM registration failed. Registration is required connect with BlackBerry Messenger. Please restart the application to try again.");

        mRegistered = false;
        emit registeredChanged(mRegistered);
    }

    void RegistrationHandler::setStatusText(const QString statusText)
    {
        if (mStatusText.compare(statusText) != 0) {
            mStatusText = statusText;
            emit statusTextChanged(mStatusText);
        }
    }

    QString RegistrationHandler::statusText()
    {
        return mStatusText;
    }

    bool RegistrationHandler::registered()
    {
        return mRegistered;
    }
}
