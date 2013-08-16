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

#include "MessageDAO.hpp"

#include <QSettings>
#include <QStringList>

namespace pushcollector
{

MessageDAO::MessageDAO()
{
}

MessageDAO::~MessageDAO()
{

}

void MessageDAO::save(const StatusMessage &statusMsg)
{
    QSettings settings;
    settings.beginGroup(QSETTINGS_MESSAGE_GROUP);
    settings.setValue(STATUS_KEY, statusMsg.status());
    settings.setValue(MESSAGE_KEY, statusMsg.message());
    settings.setValue(CODE_KEY, statusMsg.code());
    settings.endGroup();
}

void MessageDAO::remove()
{
    QSettings settings;
    settings.remove(QSETTINGS_MESSAGE_GROUP);
}

StatusMessage& MessageDAO::message()
{
    QSettings settings;

    settings.beginGroup(QSETTINGS_MESSAGE_GROUP);
    m_statusMsg.setStatus(settings.value(STATUS_KEY, DEFAULT_STATUS).toString());
    m_statusMsg.setMessage(settings.value(MESSAGE_KEY, DEFAULT_MESSAGE).toString());
    m_statusMsg.setCode(settings.value(CODE_KEY, DEFAULT_CODE).toInt());
    settings.endGroup();

    return m_statusMsg;
}

QString MessageDAO::fileName() const
{
	QSettings settings;
	return settings.fileName();
}

bool MessageDAO::hasMessage() const
{
    int keysCount = 0;

    QSettings settings;
    settings.beginGroup(QSETTINGS_MESSAGE_GROUP);
    keysCount = settings.allKeys().count();
    settings.endGroup();

    return keysCount > 0;
}

} // namespace pushcollector
