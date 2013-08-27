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

#ifndef COMMANDMESSAGE_HPP
#define COMMANDMESSAGE_HPP

#include "Global.hpp"
#include "User.hpp"

#include <QString>
#include <QVariantMap>

namespace pushcollector
{

/*!
 * Value object relating to a command message sent by the UI through the invocation framework
 */
class PUSHCOLLECTOR_EXPORT CommandMessage
{
public:
	CommandMessage();
    CommandMessage(const QVariantMap &map);
    CommandMessage(const QString &command, bool shouldRegisterToLaunch = false, bool shouldUnregisterFromLaunch = false, bool configSaveAction = false);
    virtual ~CommandMessage();

    QString command() const;
    bool shouldRegisterToLaunch() const;
    bool shouldUnregisterFromLaunch() const;
    bool configSaveAction() const;

    void setCommand(const QString &command);
    void setShouldRegisterToLaunch(bool shouldRegisterToLaunch);
    void setShouldUnregisterFromLaunch(bool shouldUnregisterFromLaunch);
    void setConfigSaveAction(bool configSaveAction);

    QVariantMap toMap() const;

private:
    // The time of the push using a 12-hour clock (e.g. 2:38p, e.g. 11:22a)
    QString m_command;

    // Whether or not to register to launch
    bool m_shouldRegisterToLaunch;

    // Whether or not to unregister from launch
    bool m_shouldUnregisterFromLaunch;

    bool m_configSaveAction;

};

} // namespace pushcollector


#endif /* COMMANDMESSAGE_HPP */
