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

#ifndef STATUSMESSAGE_HPP
#define STATUSMESSAGE_HPP

#include "Global.hpp"

#include <QString>
#include <QVariantMap>

namespace pushcollector
{

/*!
 * Value object relating to a status message sent by the headless part of the application
 */
class PUSHCOLLECTOR_EXPORT StatusMessage
{
public:
	StatusMessage();
	StatusMessage(const QVariantMap &map);
	StatusMessage(const QString &status, const QString &message = "", int code = -1);
    virtual ~StatusMessage();

    QString status() const;
    QString message() const;
    int code() const;

    void setStatus(const QString &status);
    void setMessage(const QString &message);
    void setCode(int code);

    QVariantMap toMap() const;

private:
    // The time of the push using a 12-hour clock (e.g. 2:38p, e.g. 11:22a)
    QString m_status;

    QString m_message;

    // Whether or not to register to launch
    int m_code;
};

} // namespace pushcollector



#endif /* STATUSMESSAGE_HPP */
