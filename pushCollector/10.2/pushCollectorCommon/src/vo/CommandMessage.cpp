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

#include "CommandMessage.hpp"

#include <QDebug>

namespace pushcollector
{

CommandMessage::CommandMessage()
    : m_shouldRegisterToLaunch(false)
    , m_shouldUnregisterFromLaunch(false)
	, m_configSaveAction(false)
{
}

CommandMessage::CommandMessage(const QVariantMap &map)
{
	setCommand(map["command"].toString());
	setShouldRegisterToLaunch(map["shouldRegisterToLaunch"].toBool());
	setShouldUnregisterFromLaunch(map["shouldUnregisterFromLaunch"].toBool());
	setConfigSaveAction(map["configSaveAction"].toBool());
}

CommandMessage::CommandMessage(const QString &command, bool shouldRegisterToLaunch, bool shouldUnregisterFromLaunch, bool configSaveAction)
{
	setCommand(command);
	setShouldRegisterToLaunch(shouldRegisterToLaunch);
	setShouldUnregisterFromLaunch(shouldUnregisterFromLaunch);
	setConfigSaveAction(configSaveAction);
}

CommandMessage::~CommandMessage()
{
}

QString CommandMessage::command() const
{
    return m_command;
}

bool CommandMessage::shouldRegisterToLaunch() const
{
    return m_shouldRegisterToLaunch;
}

bool CommandMessage::shouldUnregisterFromLaunch() const
{
    return m_shouldUnregisterFromLaunch;
}

bool CommandMessage::configSaveAction() const
{
	return m_configSaveAction;
}

void CommandMessage::setCommand(const QString &command)
{
    m_command = command;
}

void CommandMessage::setShouldRegisterToLaunch(bool shouldRegisterToLaunch)
{
	m_shouldRegisterToLaunch = shouldRegisterToLaunch;
}

void CommandMessage::setShouldUnregisterFromLaunch(bool shouldUnregisterFromLaunch)
{
	m_shouldUnregisterFromLaunch = shouldUnregisterFromLaunch;
}

void CommandMessage::setConfigSaveAction(bool configSaveAction)
{
	m_configSaveAction = configSaveAction;
}

QVariantMap CommandMessage::toMap() const
{
    QVariantMap map;
    map["command"] = command();
    map["shouldRegisterToLaunch"] = shouldRegisterToLaunch();
    map["shouldUnregisterFromLaunch"] = shouldUnregisterFromLaunch();
    map["configSaveAction"] = configSaveAction();

    return map;
}

} // namespace pushcollector
