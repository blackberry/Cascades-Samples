/*
* Copyright (c) 2012, 2013 BlackBerry Limited.
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

#include "PushNotificationService.hpp"
#include "PushUtility.hpp"
#include <QDebug>

#define	INITIAL_WAIT_TIME_MILLISECONDS	1000  	// 1 second
#define MAX_WAIT_TIME					128000	// 2 minutes

using namespace bb::network;

PushNotificationService::PushNotificationService(QObject *parent)
    : QObject(parent)
    , m_pushService(0)
	, m_timer(new QTimer(this))
    , m_retryTime(INITIAL_WAIT_TIME_MILLISECONDS)
{
	 checkConnectResult(QObject::connect(m_timer, SIGNAL(timeout()), this, SLOT(onConnectionClosed())));
}

void PushNotificationService::createSession()
{
    // Initialize the PushService if it has not been already
    initializePushService();
    m_pushService->createSession();

}

void PushNotificationService::createChannel()
{
    m_pushService->createChannel(m_configurationService.configuration().ppgUrl());
}

void PushNotificationService::initializePushService()
{
    const Configuration config = m_configurationService.configuration();

    if (!m_pushService || (m_previousApplicationId != config.providerApplicationId())) {

        // If a PushService instance has never been created or if the app id has changed, then create a new PushService instance
        // Important note: App ids would not change in a real application, but this sample application allows this.
        // To allow the app id change, we delete the previously existing PushService instance.
        if (m_pushService) {

        	// Disconnect the signals
        	checkConnectResult(QObject::disconnect(m_pushService, SIGNAL(createSessionCompleted(const bb::network::PushStatus&)),
					this, SIGNAL(createSessionCompleted(const bb::network::PushStatus&))));
        	checkConnectResult(QObject::disconnect(m_pushService, SIGNAL(createChannelCompleted(const bb::network::PushStatus&, const QString)),
					this, SIGNAL(createChannelCompleted(const bb::network::PushStatus&, const QString))));
        	checkConnectResult(QObject::disconnect(m_pushService, SIGNAL(destroyChannelCompleted(const bb::network::PushStatus&)),
					this, SIGNAL(destroyChannelCompleted(const bb::network::PushStatus&))));
        	checkConnectResult(QObject::disconnect(m_pushService, SIGNAL(registerToLaunchCompleted(const bb::network::PushStatus&)),
					this, SIGNAL(registerToLaunchCompleted(const bb::network::PushStatus&))));
        	checkConnectResult(QObject::disconnect(m_pushService, SIGNAL(unregisterFromLaunchCompleted(const bb::network::PushStatus&)),
					this, SIGNAL(unregisterFromLaunchCompleted(const bb::network::PushStatus&))));
        	checkConnectResult(QObject::disconnect(m_pushService, SIGNAL(simChanged()),
					this, SIGNAL(simChanged())));
        	checkConnectResult(QObject::disconnect(m_pushService, SIGNAL(pushTransportReady(bb::network::PushCommand::Type)),
					this, SIGNAL(pushTransportReady(bb::network::PushCommand::Type))));
        	checkConnectResult(QObject::disconnect(m_pushService, SIGNAL(connectionClosed()),
	                this, SLOT(onConnectionClosed())));
        	checkConnectResult(QObject::disconnect(&m_registerService, SIGNAL(piRegistrationCompleted(int, const QString)),
					this, SIGNAL(piRegistrationCompleted(int, const QString))));
        	checkConnectResult(QObject::disconnect(&m_unregisterService, SIGNAL(piDeregistrationCompleted(int, const QString)),
					this, SIGNAL(piDeregistrationCompleted(int, const QString))));

            delete m_pushService;
            m_pushService = NULL;
        }

        m_previousApplicationId = config.providerApplicationId();

        m_pushService = new PushService(config.providerApplicationId(), INVOKE_TARGET_KEY_PUSH, this);

        //Connect the signals
        checkConnectResult(QObject::connect(m_pushService, SIGNAL(createSessionCompleted(const bb::network::PushStatus&)),
                this, SIGNAL(createSessionCompleted(const bb::network::PushStatus&))));
        checkConnectResult(QObject::connect(m_pushService, SIGNAL(createChannelCompleted(const bb::network::PushStatus&, const QString)),
                this, SIGNAL(createChannelCompleted(const bb::network::PushStatus&, const QString))));
        checkConnectResult(QObject::connect(m_pushService, SIGNAL(destroyChannelCompleted(const bb::network::PushStatus&)),
                this, SIGNAL(destroyChannelCompleted(const bb::network::PushStatus&))));
        checkConnectResult(QObject::connect(m_pushService, SIGNAL(registerToLaunchCompleted(const bb::network::PushStatus&)),
                this, SIGNAL(registerToLaunchCompleted(const bb::network::PushStatus&))));
        checkConnectResult(QObject::connect(m_pushService, SIGNAL(unregisterFromLaunchCompleted(const bb::network::PushStatus&)),
                this, SIGNAL(unregisterFromLaunchCompleted(const bb::network::PushStatus&))));
        checkConnectResult(QObject::connect(m_pushService, SIGNAL(simChanged()),
                this, SIGNAL(simChanged())));
        checkConnectResult(QObject::connect(m_pushService, SIGNAL(pushTransportReady(bb::network::PushCommand::Type)),
                this, SIGNAL(pushTransportReady(bb::network::PushCommand::Type))));
        checkConnectResult(QObject::connect(m_pushService, SIGNAL(connectionClosed()),
                this, SLOT(onConnectionClosed())));
        checkConnectResult(QObject::connect(&m_registerService, SIGNAL(piRegistrationCompleted(int, const QString)),
                this, SIGNAL(piRegistrationCompleted(int, const QString))));
        checkConnectResult(QObject::connect(&m_unregisterService, SIGNAL(piDeregistrationCompleted(int, const QString)),
                this, SIGNAL(piDeregistrationCompleted(int, const QString))));
    }
}

void PushNotificationService::onConnectionClosed()
{
	if (!m_timer->isActive()){
		emit noPushServiceConnection();
		m_timer->start(m_retryTime);
		return;
	}

	// Try to reconnect to the Push Agent
	if (!m_pushService->reconnect()){

		// Reconnect was unsuccessful.
		// Check to see if the max wait time has been reached.
		// If it has, then just stop and notify the user that reconnecting to the Push Agent was unsuccessful.
		// Otherwise, double the amount of time to wait before retrying, and then try again.
		if (m_retryTime >= MAX_WAIT_TIME){
			qWarning() << "Max number of retry attempts reached. Could not re-establish connection to Push Agent.";
			emit pushAgentConnectionStatusChanged(false);
			m_timer->stop();
		} else {
			qDebug() << "No connection to Push Agent";
			m_retryTime = m_retryTime * 2;
			m_timer->setInterval(m_retryTime);
		}
	} else {
		// Reconnect was successful.
		// Notify the user and call createSession.
		qDebug() << "Connection to Push Agent re-established!";
		m_timer->stop();
		m_retryTime = INITIAL_WAIT_TIME_MILLISECONDS;
		emit pushAgentConnectionStatusChanged(true);
		createSession();
	}

}

void PushNotificationService::registerToLaunch()
{
    m_pushService->registerToLaunch();
}

void PushNotificationService::unregisterFromLaunch()
{
    m_pushService->unregisterFromLaunch();
}

User PushNotificationService::getCurrentlyRegisteredUser()
{
    return m_unregisterService.getCurrentlyRegisteredUser();
}

void PushNotificationService::subscribeToPushInitiator(const User &user, const QString &token)
{
    m_registerService.subscribeToPushInitiator(user,token);
}

void PushNotificationService::unsubscribeFromPushInitiator(const User &user)
{
    m_unregisterService.unsubscribeFromPushInitiator(user);
}

void PushNotificationService::destroyChannel()
{
    m_pushService->destroyChannel();
}

void PushNotificationService::acceptPush(const QString &payloadId)
{
    m_pushService->acceptPush(payloadId);
}

void PushNotificationService::rejectPush(const QString &payloadId)
{
    m_pushService->rejectPush(payloadId);
}

void PushNotificationService::handleSimChange(PushHandler *pushHandler)
{
	if (pushHandler == NULL){
		qDebug() << "handleSimChange: Could not handle sim change. PushHandler was null";
	}

    const Configuration config = m_configurationService.configuration();
    const User user = getCurrentlyRegisteredUser();

	// Delete all pushes (since we might be dealing with a new user
	// and they should not see the old user's pushes)
    pushHandler->removeAll();

	// Also, remove the push history
    pushHandler->removeAllPushHistory();

    if (!config.pushInitiatorUrl().isEmpty() && !user.userId().isEmpty() && !user.password().isEmpty()) {
        m_simChangeUnregisterService.unsubscribeFromPushInitiator(user);

        // Remove the user regardless of whether the unsubscribe is successful or not
        m_simChangeUnregisterService.removeUser();
    }
}

