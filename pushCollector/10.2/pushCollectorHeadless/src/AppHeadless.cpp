/*!
* Copyright (c) 2012, 2013 Research In Motion Limited.
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

#include "AppHeadless.hpp"
#include <bb/network/PushErrorCode>
#include <bb/platform/Notification>

using namespace bb::network;
using namespace bb::platform;
using namespace bb::system;

AppHeadless::AppHeadless()
		: m_invokeManager(new InvokeManager(this))
		, m_jsonDA(new bb::data::JsonDataAccess(this))
		, m_pushNotificationService(new PushNotificationService(this))
		, m_pushHandler(new PushHandler(this))
		, m_shouldRegisterToLaunch(false)
		, m_shouldUnregisterFromLaunch(false)
{
    // connect the push notification service signals and slots
	checkConnectResult(QObject::connect(m_pushNotificationService, SIGNAL(createSessionCompleted(const bb::network::PushStatus&)),
			this, SLOT(onCreateSessionCompleted(const bb::network::PushStatus&))));
	checkConnectResult(QObject::connect(m_pushNotificationService, SIGNAL(createChannelCompleted(const bb::network::PushStatus&, const QString)),
		this, SLOT(onCreateChannelCompleted(const bb::network::PushStatus&, const QString))));
	checkConnectResult(QObject::connect(m_pushNotificationService, SIGNAL(destroyChannelCompleted(const bb::network::PushStatus&)),
			this, SLOT(onDestroyChannelCompleted(const bb::network::PushStatus&))));
	checkConnectResult(QObject::connect(m_pushNotificationService, SIGNAL(registerToLaunchCompleted(const bb::network::PushStatus&)),
			this, SLOT(onRegisterToLaunchCompleted(const bb::network::PushStatus&))));
	checkConnectResult(QObject::connect(m_pushNotificationService, SIGNAL(unregisterFromLaunchCompleted(const bb::network::PushStatus&)),
			this, SLOT(onUnregisterFromLaunchCompleted(const bb::network::PushStatus&))));
	checkConnectResult(QObject::connect(m_pushNotificationService, SIGNAL(piRegistrationCompleted(int, const QString)),
			this, SLOT(onPIRegistrationCompleted(int, const QString))));
	checkConnectResult(QObject::connect(m_pushNotificationService, SIGNAL(piDeregistrationCompleted(int, const QString)),
			this, SLOT(onPIDeregistrationCompleted(int, const QString))));
	checkConnectResult(QObject::connect(m_pushNotificationService, SIGNAL(simChanged()),
			this, SLOT(onSimChanged())));
	checkConnectResult(QObject::connect(m_pushNotificationService, SIGNAL(pushTransportReady(bb::network::PushCommand::Type)),
			this, SLOT(onPushTransportReady(bb::network::PushCommand::Type))));
	checkConnectResult(QObject::connect(m_pushNotificationService, SIGNAL(noPushServiceConnection()),
			this, SLOT(onNoPushServiceConnection())));
	checkConnectResult(QObject::connect(m_pushNotificationService, SIGNAL(pushAgentConnectionStatusChanged(bool)),
			this, SLOT(onPushAgentConnectionStatusChanged(bool))));

	// connect the invoke manager signals and slots
	checkConnectResult(connect(m_invokeManager, SIGNAL(invoked(const bb::system::InvokeRequest&)), this, SLOT(onInvoked(const bb::system::InvokeRequest&))));
	initializePushSession();
}

AppHeadless::~AppHeadless()
{
}

void AppHeadless::onInvoked(const InvokeRequest& request)
{
	 if (request.action().compare(BB_PUSH_INVOCATION_ACTION) == 0) {
		 if (m_configurationService.hasConfiguration()) {
			 // The underlying PushService instance might not have been
			 // initialized when an invoke first comes in
			 // Make sure that we initialize it here if it hasn't been already
			 // It requires an application ID (for consumer applications) so we have to check
			 // that configuration settings have already been stored
			 m_pushNotificationService->initializePushService();

			qDebug() << "Received push action";

			// Received an incoming push
			// Extract it from the invoke request and then process it
			PushPayload payload(request);
			if (payload.isValid()) {
				pushNotificationHandler(payload);
			}
		 }
	} else if (request.action().compare(BB_PUSH_COLLECTOR_COMMAND_ACTION) == 0){
		qDebug() << "Received command action" << QString::fromUtf8(request.data());
		QVariantList commandData = m_jsonDA->loadFromBuffer(request.data()).toList();
		commandMessageHandler(commandData);
	}
}

void AppHeadless::sendStatusToUI(const QString &status, const QString &message, int code)
{
	StatusMessage statusMsg(status, message, code);
	m_messageWriter.write(statusMsg);
}

void AppHeadless::commandMessageHandler(const QVariantList &commandData)
{
	if (commandData.size() == 0){
		qDebug() << "command data was empty";
		return;
	}

	CommandMessage commandMsg(commandData.at(0).toMap());

	if (commandMsg.command().compare(PUSH_COLLECTOR_CREATE_SESSION) == 0){
		m_shouldRegisterToLaunch = commandMsg.shouldRegisterToLaunch();
		m_shouldUnregisterFromLaunch = commandMsg.shouldUnregisterFromLaunch();

		m_pushNotificationService->createSession();
	} else if (commandMsg.command().compare(PUSH_COLLECTOR_CREATE_CHANNEL) == 0){
		if (commandData.size() == 2){
			m_user = User(commandData.at(1).toMap());
		}

        m_pushNotificationService->createChannel();
	} else if (commandMsg.command().compare(PUSH_COLLECTOR_DESTROY_CHANNEL) == 0){
		if (commandData.size() == 2){
			m_user = User(commandData.at(1).toMap());
		}
		m_pushNotificationService->destroyChannel();
	}
}

void AppHeadless::initializePushSession()
{
    // If the app already has config info saved then create the session
    if (m_configurationService.hasConfiguration()) {
    	Configuration config = m_configurationService.configuration();

        m_shouldRegisterToLaunch = config.launchApplicationOnPush();
        m_shouldUnregisterFromLaunch = !config.launchApplicationOnPush();
        m_pushNotificationService->createSession();

    } else {
    	// there are no config settings. Tell the UI to show the config screen
    	sendStatusToUI(PUSH_COLLECTOR_LAUNCH_CONFIG);
    }
}

void AppHeadless::onNoPushServiceConnection()
{
    sendStatusToUI(PUSH_COLLECTOR_NO_PUSH_SERVICE_CONNECTION);
}

void AppHeadless::onPushAgentConnectionStatusChanged(bool isConnected)
{
	QString message;
	if (isConnected){
		message = tr("Push Service successfully reconnected to the Push Agent.");
	} else{
		message = tr("Error: Push Service could not reconnect to the Push Agent.");
	}

	sendStatusToUI(PUSH_COLLECTOR_SHOW_TOAST, message);
}

void AppHeadless::onCreateSessionCompleted(const bb::network::PushStatus &status)
{

	QString message;
    if (status.code() == PushErrorCode::NoError) {
        if (m_shouldRegisterToLaunch) {
            m_pushNotificationService->registerToLaunch();
        } else if (m_shouldUnregisterFromLaunch) {
            m_pushNotificationService->unregisterFromLaunch();
        }
    }

    sendStatusToUI(PUSH_COLLECTOR_CREATE_SESSION_COMPLETE, message, status.code());
}

void AppHeadless::onCreateChannelCompleted(const bb::network::PushStatus &status, const QString &token)
{
    qDebug()<< "creatChannelComplete status: " << status.code();
    qDebug()<< "createChannelComplete token: " << token;

    // Typically in your own application you wouldn't want to display this error to your users
    QString message = tr("Create channel failed with error code: %0").arg(status.code());

    Configuration config = m_configurationService.configuration();

    switch(status.code()){
    case  PushErrorCode::NoError:

        if (!config.pushInitiatorUrl().isEmpty()) {
            // Now, attempt to subscribe to the Push Initiator
            message = tr("Subscribing to Push Initiator...");
            sendStatusToUI(PUSH_COLLECTOR_REGISTER_WITH_PUSH_INITIATOR, message, status.code());

            // This is very important: the token returned in the create channel success event is what
            // the Push Initiator should use when initiating a push to the BlackBerry Push Service.
            // This token must be communicated back to the Push Initiator's server-side application.
            m_pushNotificationService->subscribeToPushInitiator(m_user, token);
            return;
        } else {
        	message = tr("Register succeeded.");
        }
        break;
    case  PushErrorCode::TransportFailure:
    	message = tr("Create channel failed as the push transport is unavailable. "
                  "Verify your mobile network and/or Wi-Fi are turned on. "
                  "If they are on, you will be notified when the push transport is available again.");
        break;
    case PushErrorCode::SubscriptionContentNotAvailable:
    	message = tr("Create channel failed as the PPG is currently returning a server error. "
                  "You will be notified when the PPG is available again.");
        break;
    }

    sendStatusToUI(PUSH_COLLECTOR_CREATE_CHANNEL_COMPLETE, message, status.code());
}

void AppHeadless::onPIRegistrationCompleted(int code, const QString &description)
{
	QString message;
    if (code == 200) {
        message = tr("Register succeeded.");
    } else {
        // Typically in your own application you wouldn't want to display this error to your users
    	message = tr("Subscribe to the Push Initiator failed with error code: %0. Reason: %1").arg(QString::number(code), description);
    }

    sendStatusToUI(PUSH_COLLECTOR_REGISTER_WITH_PUSH_INITIATOR_COMPLETE, message, code);
}

void AppHeadless::onDestroyChannelCompleted(const bb::network::PushStatus &status)
{
    qDebug() << "onDestroyChannelCompleted: " << status.code();

    // Typically in your own application you wouldn't want to display this error to your users
    QString message = tr("Destroy channel failed with error code: %0").arg(status.code());

    Configuration config = m_configurationService.configuration();

    switch(status.code()){
    case  PushErrorCode::NoError:
         if (!config.pushInitiatorUrl().isEmpty()) {
             // The Push Service SDK will be used to unsubscribe to the Push Initiator's server-side application since a
             // Push Initiator URL was specified
             message = tr("Unsubscribing from Push Initiator...");
             sendStatusToUI(PUSH_COLLECTOR_DEREGISTER_WITH_PUSH_INITIATOR, message, status.code());

             // Now, attempt to unsubscribe from the Push Initiator
             m_pushNotificationService->unsubscribeFromPushInitiator(m_user);
            return;
        } else {
        	message = tr("Unregister succeeded.");
        }
        break;
    case  PushErrorCode::TransportFailure:
    	message = tr("Destroy channel failed as the push transport is unavailable. "
                  "Verify your mobile network and/or Wi-Fi are turned on. "
                  "If they are on, you will be notified when the push transport is available again.");
        break;
    case PushErrorCode::SubscriptionContentNotAvailable:
    	message = tr("Destroy channel failed as the PPG is currently returning a server error. "
                  "You will be notified when the PPG is available again.");
        break;
    }

    sendStatusToUI(PUSH_COLLECTOR_DESTROY_CHANNEL_COMPLETE, message, status.code());
}

void AppHeadless::onPIDeregistrationCompleted(int code, const QString &description)
{
	QString message;
    if (code == 200) {
    	message = tr("Unregister succeeded.");
    } else {
        // Typically in your own application you wouldn't want to display this error to your users
    	message = tr("Unsubscribe from the Push Initiator failed with error code: %0. Reason: %1").arg(QString::number(code), description);
    }

    sendStatusToUI(PUSH_COLLECTOR_DEREGISTER_WITH_PUSH_INITIATOR_COMPLETE, message, code);
}

void AppHeadless::onRegisterToLaunchCompleted(const bb::network::PushStatus &status)
{
    if (status.code() != PushErrorCode::NoError) {
        // Typically in your own application you wouldn't want to display this error to your users
    	QString message(tr("Register to launch failed with error code: %0.").arg(status.code()));
    	sendStatusToUI(PUSH_COLLECTOR_REGISTER_TO_LAUNCH_COMPLETE, message, status.code());
    }
}

void AppHeadless::onUnregisterFromLaunchCompleted(const bb::network::PushStatus &status)
{
    if (status.code() != PushErrorCode::NoError) {
        // Typically in your own application you wouldn't want to display this error to your users
    	QString message(tr("Unregister from launch failed with error code: %0.").arg(status.code()));
    	sendStatusToUI(PUSH_COLLECTOR_UNREGISTER_FROM_LAUNCH_COMPLETE, message, status.code());
    }
}

void AppHeadless::onPushTransportReady(bb::network::PushCommand::Type command)
{
    QString message = "The push transport is now available. Please try ";
    if (command == PushCommand::CreateChannel){
        message += "registering ";
    } else {
        message += "unregistering ";
    }

    message += "again.";

    sendStatusToUI(PUSH_COLLECTOR_SHOW_DIALOG, message);
}

void AppHeadless::onSimChanged()
{
    // Remove the currently registered user (if there is one)
    // and unsubscribe the user from the Push Initiator since
    // switching SIMs might indicate we are dealing with
    // a different user. Also, remove all pushes and push
	// history from the database.
    m_pushNotificationService->handleSimChange(m_pushHandler);

    sendStatusToUI(PUSH_COLLECTOR_SIM_CHANGE);
}

void AppHeadless::pushNotificationHandler(bb::network::PushPayload &pushPayload)
{
    // Check for a duplicate push
    PushHistoryItem pushHistoryItem(pushPayload.id());

    if (m_pushHandler->checkForDuplicate(pushHistoryItem)) {
        // A duplicate was found, stop processing. Silently discard this push from the user
        qWarning() << QString("Duplicate push was found with ID: %0.").arg(pushPayload.id());

        return;
    }

    // Convert from PushPayload to Push so that it can be stored in the database
    Push push(pushPayload);

    // Save the push and set the sequence number (ID) of the push
    push.setSeqNum(m_pushHandler->save(push));

    // Create a notification for the push that will be added to the BlackBerry Hub
    Notification *notification = new Notification(NOTIFICATION_PREFIX + QString::number(push.seqNum()),this);
    notification->setTitle("Push Collector");
    notification->setBody(QString("New %0 push received").arg(push.fileExtension()));

    // Add an invoke request to the notification
    // This invoke will contain the seqnum of the push.
    // When the notification in the BlackBerry Hub is selected, this seqnum will be used to lookup the push in
    // the database and display it
    InvokeRequest invokeRequest;
    invokeRequest.setTarget(INVOKE_TARGET_KEY_OPEN);
    invokeRequest.setAction(BB_OPEN_INVOCATION_ACTION);
    invokeRequest.setMimeType("text/plain");
    invokeRequest.setData(QByteArray::number(push.seqNum()));
    notification->setInvokeRequest(invokeRequest);

    // Add the notification for the push to the BlackBerry Hub
    // Calling this method will add a "splat" to the application icon, indicating that a new push has been received
    notification->notify();

    // tell the UI to refresh the model so that it will display the the newly received push in the list
    sendStatusToUI(PUSH_COLLECTOR_REFRESH_MODEL);

    // If an acknowledgement of the push is required (that is, the push was sent as a confirmed push
    // - which is equivalent terminology to the push being sent with application level reliability),
    // then you must either accept the push or reject the push
    if (pushPayload.isAckRequired()) {
        // In our sample, we always accept the push, but situations might arise where an application
        // might want to reject the push (for example, after looking at the headers that came with the push
        // or the data of the push, we might decide that the push received did not match what we expected
        // and so we might want to reject it)
        m_pushNotificationService->acceptPush(pushPayload.id());
    }
}
