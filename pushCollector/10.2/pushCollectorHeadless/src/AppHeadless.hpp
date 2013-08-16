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

#ifndef APPHEADLESS_HPP
#define APPHEADLESS_HPP

#include "PushNotificationService.hpp"
#include "PushUtility.hpp"
#include "service/ConfigurationService.hpp"
#include "service/MessageWriter.hpp"
#include "service/PushHandler.hpp"
#include "vo/CommandMessage.hpp"

#include <bb/data/JsonDataAccess>
#include <bb/system/InvokeManager>

using namespace pushcollector;

class AppHeadless : public QObject {
	Q_OBJECT
public:
	AppHeadless();
	virtual ~AppHeadless();

public Q_SLOTS:
	void onCreateSessionCompleted(const bb::network::PushStatus &status);
    void onCreateChannelCompleted(const bb::network::PushStatus &status, const QString &token);
    void onDestroyChannelCompleted(const bb::network::PushStatus &status);
    void onRegisterToLaunchCompleted(const bb::network::PushStatus &status);
    void onUnregisterFromLaunchCompleted(const bb::network::PushStatus &status);
    void onPIRegistrationCompleted(int code, const QString &description);
    void onPIDeregistrationCompleted(int code, const QString &description);
    void onSimChanged();
    void onPushTransportReady(bb::network::PushCommand::Type command);
	void onInvoked(const bb::system::InvokeRequest& request);
    void onNoPushServiceConnection();
    void onPushAgentConnectionStatusChanged(bool isConnected);

private:

	void initializePushSession();
	void commandMessageHandler(const QVariantList &commandData);
	void pushNotificationHandler(bb::network::PushPayload &pushPayload);
	void sendStatusToUI(const QString &status, const QString &message = "", int code = -1);

	// The manager object to react to invocations
	bb::system::InvokeManager *m_invokeManager;

	// Used to convert data from JSON format to Qt C++ objects
	bb::data::JsonDataAccess *m_jsonDA;

	PushNotificationService *m_pushNotificationService;

	PushHandler *m_pushHandler;

    // The wrapper classes for loading/storing configuration values
    ConfigurationService m_configurationService;

    MessageWriter m_messageWriter;

    bool m_shouldRegisterToLaunch;
    bool m_shouldUnregisterFromLaunch;

    User m_user;
};

#endif /* APPHEADLESS_HPP */
