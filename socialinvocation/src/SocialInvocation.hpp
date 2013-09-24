/* Copyright (c) 2013 BlackBerry Limited.
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
#ifndef SOCIALINVOCATION_HPP
#define SOCIALINVOCATION_HPP

#include <QObject>
#include <bb/system/InvokeManager>

namespace bb {
namespace system {
class InvokeManager;
}
}

//! [0]
class SocialInvocation : public QObject
{
    Q_OBJECT

public:
    // Creates a new SocialInvocation object with the given @p parent object
    SocialInvocation(QObject *parent = 0);

    // Converts the passed QString to an UTF-8 encoded QByteArray
    Q_INVOKABLE QByteArray encodeQString(const QString& toEncode) const;

public Q_SLOTS:
    // This method is called to invoke another application with the current configuration
    void invoke(const QString &target, const QString &action,
                const QString &mimetype, const QString &uri);

    void invokeFoursquareVenueCard (const QString &venue);
    void childCardDone(const bb::system::CardDoneMessage &doneMessage);
    void onSSO();

private:
    // The central object to manage invocations
    bb::system::InvokeManager* m_invokeManager;
};
//! [0]

#endif
