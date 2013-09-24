/* Copyright (c) 2012, 2013  BlackBerry Limited.
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


#ifndef PROFILEEDITOR_HPP
#define PROFILEEDITOR_HPP

#include <bb/platform/bbm/UserProfile>

#include <QtCore/QObject>
#include <QtCore/QPointer>

//! [0]
class ProfileEditor : public QObject
{
    Q_OBJECT

public:
    ProfileEditor(bb::platform::bbm::UserProfile* userProfile, QObject *parent = 0);

public Q_SLOTS:
    void savePersonalMessage(const QString &personalMessage);
    void saveStatus(bool busy, const QString &statusMessage);
    void saveDisplayPicture(const QString &filePath);

private:
    QPointer<bb::platform::bbm::UserProfile> m_userProfile;
};
//! [0]

#endif
