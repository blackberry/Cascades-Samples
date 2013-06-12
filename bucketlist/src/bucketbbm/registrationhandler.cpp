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
            QObject(parent), mProgress(BbmRegistrationProgress::NotStarted), mContext(0), mRegistered(false)
    {
        // Attempt to register the application with the following UUID.
        // This UUID is for the BBM SP. It is an identifier that uniquely identifies the
        // app to the infrastructure.
        // This is used mainly when communicating between two applications using BBM.
        // This UUID is only used when side loading the app. Once it is submitted to App World,
        // AW supplies the UUID.  So it should be it will need to be updated once a real id is given.
        // Meanwhile you can define your own UUID here.  You can generate one here: http://www.guidgenerator.com/
        mUuid = QString::fromUtf8("1FBE8708-486A-4D46-AB0C-81FAEF3BDD03"); //original
    }

    void RegistrationHandler::registerApp()
    {
        mContext = new bb::platform::bbm::Context(QUuid(mUuid));
        Global::instance()->setContext(mContext);

        // Connect the BBM SP registration signals to our application's slot.
        QObject::connect(mContext,
                SIGNAL(registrationStateUpdated(bb::platform::bbm::RegistrationState::Type)), this,
                SLOT(registrationStatus(bb::platform::bbm::RegistrationState::Type)));

        mProgress = BbmRegistrationProgress::Started;
        registrationStatus(mContext->registrationState());
    }

    void RegistrationHandler::registrationStatus(bb::platform::bbm::RegistrationState::Type state)
    {
        // Based on the state, decide whether we need to register. If we already
        // registered successfully once (i.e. on a previous application run), then
        // we will not call requestRegisterApplication() again.
        qDebug() << "Received a BBM Social Platform registration access state=" << state;
        switch (mProgress) {
            case BbmRegistrationProgress::Pending:
                if (state != RegistrationState::Pending) {
                    registrationFinished();
                    return;
                }
                // Otherwise, ignore since registration is still in progress.
                break;

            case BbmRegistrationProgress::Started:
                if (mContext->isAccessAllowed()) {
                    // Access is allowed, the application is registered.
                    registrationFinished();
                    return;
                }
                if (mContext->registrationState() == RegistrationState::Unknown) {
                    // Status is not yet known. Wait for an event that will deliver the
                    // status.
                    qDebug() << "BBM Social Platform access state is UNKNOWN; waiting "
                            "for the initial status";
                    return;
                }
                // Start registration.
                if (mContext->requestRegisterApplication()) {
                    // Registration started. The user will see a dialog informing them
                    // that your application is connecting to BBM.
                    mProgress = BbmRegistrationProgress::Pending;
                    qDebug() << "BBM Social Platform registration started";
                    qDebug() << "Verify you are using a valid UUID";
                    return;
                }
                // Could not start registration. No dialogs were shown.
                qDebug() << "BBM Social Platform registration could not be started";
                registrationFinished();
                break;

            case BbmRegistrationProgress::Finished:
                if (mContext->isAccessAllowed() != mRegistered) {
                    // Access to the BBM Social Platform has changed.
                    registrationFinished();
                }
                break;

            default:
                qDebug() << "Ignoring BBM Social Platform access state=" << state
                        << "when progress=" << mProgress;
                break;
        }
    }

    void RegistrationHandler::registrationFinished()
    {
        // Finish registration and use the state to decide which message to show
        // the user.
        mProgress = BbmRegistrationProgress::Finished;
        switch (mContext->registrationState()) {
            case RegistrationState::Allowed:
                qDebug() << "Application connected to BBM. Press Continue.";
                break;

            case RegistrationState::BlockedByRIM:
                qDebug() << "application from connecting to BBM.";

                break;

            case RegistrationState::BlockedByUser:
                qDebug()
                        << "Disconnected. Go to Settings -> Security and  Privacy -> Application Permissions and  connect this application to BBM.";

                break;

            case RegistrationState::InvalidUuid:
                // You should be resolving this error at development time.
                qDebug() << "Invalid UUID. Report this error to the vendor.";
                break;

            case RegistrationState::MaxAppsReached:
                qDebug()
                        << "Too many applications are connected to BBM.Uninstall one or more applications and try again.";
                break;

            case RegistrationState::Expired:
            case RegistrationState::MaxDownloadsReached:
                qDebug()
                        << "Cannot connect to BBM. Download this application from AppWorld to keep using it.";
                break;

            case RegistrationState::NoDataConnection:
                qDebug() << "Check your Internet connection and try again.";

                break;

            case RegistrationState::Pending:
                // The user will never see this. The BBM Social Platform already
                // displays a "Connecting" dialog.
                qDebug() << "Connecting to BBM. Please wait.";
                break;

            case RegistrationState::Unknown:
                qDebug() << "Determining the status. Please wait.";
                break;

            case RegistrationState::Unregistered:
            case RegistrationState::UnexpectedError:
            case RegistrationState::TemporaryError:
            case RegistrationState::CancelledByUser:
            default:

                break;
        }

        if (mContext->isAccessAllowed()) {
            mRegistered = true;
        } else {
            mRegistered = false;
        }

        qDebug() << "Finished BBM Social Platform registration=" << mRegistered;

        emit registeredChanged(mRegistered);
    }

    void RegistrationHandler::registrationFailed()
    {
        // A generic error message is provided here.
        // You could provide a different error for each failure code to instruct the
        // user on how to continue.
        setStatusText(
                tr("BBM registration failed. Registration is required connect with BlackBerry Messenger. Please restart the application to try again."));

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
