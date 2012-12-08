/*
* Copyright (c) 2012 Research In Motion Limited.
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
#include <QDebug>

using namespace bb::network;

PushNotificationService::PushNotificationService(QObject *parent)
    : QObject(parent)
    , m_pushService(0)
{
}

void PushNotificationService::createSession()
{
    // Initialize the PushService if it has not been already
    initializePushService();

    // Check to see if the PushService has a connection to the Push Agent.
    // This can occur if the application doesn't have sufficient permissions to use Push.
    // For more information on required permissions, please refer to the developer guide.
    if (m_pushService->hasConnection()){
        m_pushService->createSession();
    } else {
        emit noPushServiceConnection();
    }
}

void PushNotificationService::createChannel()
{
    if (m_pushService->hasConnection()){
        m_pushService->createChannel(m_configurationService.configuration().ppgUrl());
    } else {
        emit noPushServiceConnection();
    }
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
			QObject::disconnect(m_pushService, SIGNAL(createSessionCompleted(const bb::network::PushStatus&)),
					this, SIGNAL(createSessionCompleted(const bb::network::PushStatus&)));
			QObject::disconnect(m_pushService, SIGNAL(createChannelCompleted(const bb::network::PushStatus&, const QString)),
					this, SIGNAL(createChannelCompleted(const bb::network::PushStatus&, const QString)));
			QObject::disconnect(m_pushService, SIGNAL(destroyChannelCompleted(const bb::network::PushStatus&)),
					this, SIGNAL(destroyChannelCompleted(const bb::network::PushStatus&)));
			QObject::disconnect(m_pushService, SIGNAL(registerToLaunchCompleted(const bb::network::PushStatus&)),
					this, SIGNAL(registerToLaunchCompleted(const bb::network::PushStatus&)));
			QObject::disconnect(m_pushService, SIGNAL(unregisterFromLaunchCompleted(const bb::network::PushStatus&)),
					this, SIGNAL(unregisterFromLaunchCompleted(const bb::network::PushStatus&)));
			QObject::disconnect(m_pushService, SIGNAL(simChanged()),
					this, SIGNAL(simChanged()));
			QObject::disconnect(m_pushService, SIGNAL(pushTransportReady(bb::network::PushCommand::Type)),
					this, SIGNAL(pushTransportReady(bb::network::PushCommand::Type)));
			QObject::disconnect(&m_registerService, SIGNAL(piRegistrationCompleted(int, const QString)),
					this, SIGNAL(piRegistrationCompleted(int, const QString)));
			QObject::disconnect(&m_unregisterService, SIGNAL(piDeregistrationCompleted(int, const QString)),
					this, SIGNAL(piDeregistrationCompleted(int, const QString)));

            delete m_pushService;
            m_pushService = NULL;
        }

        m_previousApplicationId = config.providerApplicationId();

        m_pushService = new PushService(config.providerApplicationId(), INVOKE_TARGET_KEY_PUSH, this);

        //Connect the signals
        QObject::connect(m_pushService, SIGNAL(createSessionCompleted(const bb::network::PushStatus&)),
                this, SIGNAL(createSessionCompleted(const bb::network::PushStatus&)));
        QObject::connect(m_pushService, SIGNAL(createChannelCompleted(const bb::network::PushStatus&, const QString)),
                this, SIGNAL(createChannelCompleted(const bb::network::PushStatus&, const QString)));
        QObject::connect(m_pushService, SIGNAL(destroyChannelCompleted(const bb::network::PushStatus&)),
                this, SIGNAL(destroyChannelCompleted(const bb::network::PushStatus&)));
        QObject::connect(m_pushService, SIGNAL(registerToLaunchCompleted(const bb::network::PushStatus&)),
                this, SIGNAL(registerToLaunchCompleted(const bb::network::PushStatus&)));
        QObject::connect(m_pushService, SIGNAL(unregisterFromLaunchCompleted(const bb::network::PushStatus&)),
                this, SIGNAL(unregisterFromLaunchCompleted(const bb::network::PushStatus&)));
        QObject::connect(m_pushService, SIGNAL(simChanged()),
                this, SIGNAL(simChanged()));
        QObject::connect(m_pushService, SIGNAL(pushTransportReady(bb::network::PushCommand::Type)),
                this, SIGNAL(pushTransportReady(bb::network::PushCommand::Type)));
        QObject::connect(&m_registerService, SIGNAL(piRegistrationCompleted(int, const QString)),
                this, SIGNAL(piRegistrationCompleted(int, const QString)));
        QObject::connect(&m_unregisterService, SIGNAL(piDeregistrationCompleted(int, const QString)),
                this, SIGNAL(piDeregistrationCompleted(int, const QString)));
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
    if (m_pushService->hasConnection()){
        m_pushService->destroyChannel();
    }  else {
        emit noPushServiceConnection();
    }
}

void PushNotificationService::acceptPush(const QString &payloadId)
{
    m_pushService->acceptPush(payloadId);
}

void PushNotificationService::rejectPush(const QString &payloadId)
{
    m_pushService->rejectPush(payloadId);
}

bool PushNotificationService::checkForDuplicatePush(const PushHistoryItem &pushHistoryItem)
{
    return m_pushHandler.checkForDuplicate(pushHistoryItem);
}

int PushNotificationService::savePush(const Push &push)
{
    return m_pushHandler.save(push);
}

Push PushNotificationService::push(int pushSeqNum)
{
    return m_pushHandler.push(pushSeqNum);
}

QVariantList PushNotificationService::pushes()
{
    return m_pushHandler.pushes();
}

bool PushNotificationService::removePush(int pushSeqNum)
{
    return m_pushHandler.remove(pushSeqNum);
}

bool PushNotificationService::removeAllPushes()
{
    return m_pushHandler.removeAll();
}

bool PushNotificationService::markPushAsRead(int pushSeqNum)
{
    return m_pushHandler.markAsRead(pushSeqNum);
}

bool PushNotificationService::markAllPushesAsRead()
{
    return m_pushHandler.markAllAsRead();
}

void PushNotificationService::handleSimChange()
{
    const Configuration config = m_configurationService.configuration();
    const User user = getCurrentlyRegisteredUser();

    if (!config.pushInitiatorUrl().isEmpty() && !user.userId().isEmpty() && !user.password().isEmpty()) {
        m_simChangeUnregisterService.unsubscribeFromPushInitiator(user);

        // Remove the user regardless of whether the unsubscribe is successful or not
        m_simChangeUnregisterService.removeUser();
    }
}
