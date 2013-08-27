/*!
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

#ifndef PUSHNOTIFICATIONSERVICE_HPP
#define PUSHNOTIFICATIONSERVICE_HPP

#include "service/PushHandler.hpp"
#include "service/RegisterService.hpp"
#include "service/UnregisterService.hpp"

#include <bb/network/PushService>
#include <bb/network/PushStatus>

#include <QObject>
#include <QVariantList>
#include <QTimer>

using namespace pushcollector;

/*!
 * Offers services related to the registering of a user to receive pushes, the
 * handling / processing of pushes, and the unregistering of a user from receiving pushes,
 * and the handling of a SIM card change.
 */
class PushNotificationService :  public QObject
{
    Q_OBJECT

public:
    PushNotificationService(QObject *parent = 0);

    void createSession();
    void createChannel();
    void registerToLaunch();
    void unregisterFromLaunch();
    User getCurrentlyRegisteredUser();
    void subscribeToPushInitiator(const User &user, const QString &token);
    void unsubscribeFromPushInitiator(const User &user);
    void destroyChannel();
    void initializePushService();
    void acceptPush(const QString &payloadId);
    void rejectPush(const QString &payloadId);
    void handleSimChange(PushHandler *pushHandler);

Q_SIGNALS:
    void createSessionCompleted(const bb::network::PushStatus &status);
    void createChannelCompleted(const bb::network::PushStatus &status, const QString &token);
    void destroyChannelCompleted(const bb::network::PushStatus &status);
    void registerToLaunchCompleted(const bb::network::PushStatus &status);
    void unregisterFromLaunchCompleted(const bb::network::PushStatus &status);
    void simChanged();
    void pushTransportReady(bb::network::PushCommand::Type command);
    void piRegistrationCompleted(int code, const QString &description);
    void piDeregistrationCompleted(int code, const QString &description);
    void noPushServiceConnection();
    void pushAgentConnectionStatusChanged(bool isConnected);

public Q_SLOTS:
    void onConnectionClosed();

private:
    ConfigurationService m_configurationService;
    bb::network::PushService *m_pushService;
    QString m_previousApplicationId;
    RegisterService m_registerService;
    UnregisterService m_unregisterService;

    // use a separate UnregisterService for simChange
    // this one quietly suppresses the result of the unregister action
    UnregisterService m_simChangeUnregisterService;

    QTimer *m_timer;

    // number of milliseconds to wait before retrying to reconnect
    // to the Push Agent
    int m_retryTime;
};

#endif
