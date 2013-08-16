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

#include "StatusMessage.hpp"

#include <QDebug>

namespace pushcollector
{

StatusMessage::StatusMessage()
		: m_code(-1)
{
}

StatusMessage::StatusMessage(const QVariantMap &map)
{
	setStatus(map["status"].toString());
	setMessage(map["message"].toString());
	setCode(map["code"].toInt());
}

StatusMessage::StatusMessage(const QString &status, const QString &message, int code)
{
	setStatus(status);
	setMessage(message);
	setCode(code);
}

StatusMessage::~StatusMessage()
{
}

QString StatusMessage::status() const
{
    return m_status;
}

QString StatusMessage::message() const
{
    return m_message;
}

int StatusMessage::code() const
{
    return m_code;
}


void StatusMessage::setStatus(const QString &status)
{
    m_status = status;
}

void StatusMessage::setMessage(const QString &message)
{
    m_message = message;
}

void StatusMessage::setCode(int code)
{
	m_code = code;
}


QVariantMap StatusMessage::toMap() const
{
    QVariantMap map;
    map["status"] = status();
    map["message"] = message();
    map["code"] = code();

    return map;
}

} // namespace pushcollector


