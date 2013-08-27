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

#ifndef MESSAGEDAO_HPP
#define MESSAGEDAO_HPP

#include "Global.hpp"
#include <QString>

#define QSETTINGS_MESSAGE_GROUP "message"
#define STATUS_KEY "status"
#define MESSAGE_KEY "message"
#define CODE_KEY "code"

#define DEFAULT_STATUS ""
#define DEFAULT_MESSAGE ""
#define DEFAULT_CODE -1

#include "../vo/StatusMessage.hpp"

namespace pushcollector
{

/*!
 * DAO related to the reading and writing of messages to the persistent store
 */
class PUSHCOLLECTOR_EXPORT MessageDAO
{
public:
	MessageDAO();
    ~MessageDAO();

    void save(const StatusMessage &statusMsg);

    void remove();

    StatusMessage& message();

    QString fileName() const;

    bool hasMessage() const;

private:
    StatusMessage m_statusMsg;
};

} // namespace pushcollector
#endif /* MESSAGEDAO_HPP */
