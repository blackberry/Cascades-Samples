/* Copyright (c) 2012 Research In Motion Limited.
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

#ifndef PROFILE_HPP
#define PROFILE_HPP

#include <bb/cascades/Image>
#include <bb/platform/bbm/UserProfile>

#include <QtCore/QObject>

class ProfileEditor;
namespace bb {
namespace platform {
namespace bbm {
class Context;
}
}
}
//! [0]
class Profile : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool busy READ busy NOTIFY profileChanged)
    Q_PROPERTY(QString displayName READ displayName NOTIFY profileChanged)
    Q_PROPERTY(QString statusMessage READ statusMessage NOTIFY profileChanged)
    Q_PROPERTY(QString personalMessage READ personalMessage NOTIFY profileChanged)
    Q_PROPERTY(QString ppid READ ppid NOTIFY profileChanged)
    Q_PROPERTY(QString appVersion READ appVersion NOTIFY profileChanged)
    Q_PROPERTY(QString handle READ handle NOTIFY profileChanged)
    Q_PROPERTY(QString platformVersion READ platformVersion NOTIFY profileChanged)
    Q_PROPERTY(QVariant displayPicture READ displayPicture NOTIFY profileChanged)

    Q_PROPERTY(ProfileEditor* editor READ editor CONSTANT)

public:
    Profile(bb::platform::bbm::Context &context, QObject *parent = 0);

public Q_SLOTS:
    void show();
    void requestDisplayPicture();

Q_SIGNALS:
    void profileChanged();

private:
    bool busy() const;
    QString displayName() const;
    QString statusMessage() const;
    QString personalMessage() const;
    QString ppid() const;
    QString appVersion() const;
    QString handle() const;
    QString platformVersion() const;
    QVariant displayPicture() const;
    ProfileEditor* editor() const;

    bb::platform::bbm::UserProfile* m_userProfile;
    bb::cascades::Image m_displayPicture;
    ProfileEditor* m_profileEditor;
    bb::platform::bbm::Context* m_context;
};
//! [0]

#endif
