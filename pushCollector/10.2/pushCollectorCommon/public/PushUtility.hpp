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

#ifndef PUSHUTILITY_HPP
#define PUSHUTILITY_HPP

#include "Global.hpp"

// The commands that the UI will send to the headless part of the app
#define PUSH_COLLECTOR_LAUNCH_CONFIG	 						"launchConfig"
#define PUSH_COLLECTOR_CREATE_SESSION    						"createSession"
#define PUSH_COLLECTOR_CREATE_CHANNEL    						"createChannel"
#define PUSH_COLLECTOR_DESTROY_CHANNEL    						"destroyChannel"
#define PUSH_COLLECTOR_REGISTER_WITH_PUSH_INITIATOR 			"registerWithPI"
#define PUSH_COLLECTOR_DEREGISTER_WITH_PUSH_INITIATOR 			"deregisterWithPI"
#define PUSH_COLLECTOR_SIM_CHANGE					 			"simChange"
#define PUSH_COLLECTOR_NO_PUSH_SERVICE_CONNECTION				"noPushServiceConnection"
#define PUSH_COLLECTOR_CREATE_SESSION_COMPLETE 					"createSessionComplete"
#define PUSH_COLLECTOR_CREATE_CHANNEL_COMPLETE 					"createChannelComplete"
#define PUSH_COLLECTOR_DESTROY_CHANNEL_COMPLETE 				"destroyChannelComplete"
#define PUSH_COLLECTOR_REGISTER_WITH_PUSH_INITIATOR_COMPLETE 	"registerWithPIComplete"
#define PUSH_COLLECTOR_DEREGISTER_WITH_PUSH_INITIATOR_COMPLETE 	"deregisterWithPIComplete"
#define PUSH_COLLECTOR_REGISTER_TO_LAUNCH_COMPLETE 				"registerToLaunchComplete"
#define PUSH_COLLECTOR_UNREGISTER_FROM_LAUNCH_COMPLETE 			"unregisterFromLaunchComplete"
#define PUSH_COLLECTOR_SHOW_DIALOG	 							"showDialog"
#define PUSH_COLLECTOR_SHOW_TOAST	 							"showToast"
#define PUSH_COLLECTOR_REFRESH_MODEL	 						"refreshModel"

// This needs to match the invoke target specified in bar-descriptor.xml
// The Invoke target key for receiving new push notifications
#define INVOKE_TARGET_KEY_PUSH "com.example.pushCollector.invoke.push"

// This needs to match the invoke target specified in bar-descriptor.xml
// The Invoke target key when selecting a notification in the BlackBerry Hub
#define INVOKE_TARGET_KEY_OPEN "com.example.pushCollector.invoke.open"

// The invocation action for items added to BlackBerry Hub
#define BB_OPEN_INVOCATION_ACTION "bb.action.OPEN"

// The invocation action used when sending commands to the headless part of the application
#define BB_PUSH_COLLECTOR_COMMAND_ACTION "bb.action.COMMAND"

// Each item in the BlackBerry Hub has a key. This is the prefix that Push Collector adds to the key
// before adding an item to the BlackBerry Hub
#define NOTIFICATION_PREFIX "com.example.pushCollector_"

namespace pushcollector
{

/*!
 * Verifies that the QObject::connect result is true. If a slot failed to connect
 * to the signal, make sure you know exactly why this has happened. This is not
 * normal, and will cause your app to stop working!!
 */
PUSHCOLLECTOR_EXPORT void checkConnectResult(bool connectResult);

} // namespace pushcollector
#endif /* PUSHUTILITY_HPP */
