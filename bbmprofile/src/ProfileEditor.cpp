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

#include "ProfileEditor.hpp"

#include <QtCore/QFile>

//! [0]
ProfileEditor::ProfileEditor(bb::platform::bbm::UserProfile* userProfile, QObject *parent)
    : QObject(parent)
    , m_userProfile(userProfile)
{
}
//! [0]

//! [1]
void ProfileEditor::saveStatus(bool busy, const QString &statusMessage)
{
    if (!m_userProfile)
        return;

    m_userProfile->requestUpdateStatus((busy? bb::platform::bbm::UserStatus::Busy : bb::platform::bbm::UserStatus::Available), statusMessage);
}
//! [1]

//! [2]
void ProfileEditor::savePersonalMessage(const QString &personalMessage)
{
    if (!m_userProfile)
        return;

    m_userProfile->requestUpdatePersonalMessage(personalMessage);
}
//! [2]

//! [3]
void ProfileEditor::saveDisplayPicture(const QString &filePath)
{
    if (!m_userProfile)
        return;

    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly))
        return;

    const QByteArray imageData = file.readAll();

    bb::platform::bbm::ImageType::Type imageType = bb::platform::bbm::ImageType::Unsupported;
    if (filePath.endsWith(QLatin1String(".jpg"), Qt::CaseInsensitive) ||
        filePath.endsWith(QLatin1String(".jpeg"), Qt::CaseInsensitive))
        imageType = bb::platform::bbm::ImageType::Jpg;
    else if (filePath.endsWith(QLatin1String(".png"), Qt::CaseInsensitive))
        imageType = bb::platform::bbm::ImageType::Png;
    else if (filePath.endsWith(QLatin1String(".gif"), Qt::CaseInsensitive))
        imageType = bb::platform::bbm::ImageType::Gif;
    else if (filePath.endsWith(QLatin1String(".bmp"), Qt::CaseInsensitive))
        imageType = bb::platform::bbm::ImageType::Bmp;

    m_userProfile->requestUpdateDisplayPicture(imageType, imageData);
}
//! [3]
