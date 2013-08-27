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

#include "TrackManager.hpp"

#include <QtCore/QDir>
#include <QtCore/QDirIterator>

using namespace bb::cascades;

/**
 * A helper method to return the directory path where
 * all the recorded tracks are stored.
 */
static QString trackStorageLocation()
{
    return QString("data/tracks/");
}

/**
 * A helper method to initialize the directory where
 * all the recorded tracks are stored.
 */
static void initializeTrackStorage()
{
    QDir trackStorage(trackStorageLocation());
    if (!trackStorage.exists())
        QDir::root().mkpath(trackStorage.absolutePath());
}

TrackManager::TrackManager(QObject *parent)
    : QObject(parent)
    , m_model(new GroupDataModel(this))
    , m_trackCounter(0)
{
    m_model->setGrouping(ItemGrouping::None);

    initializeTrackStorage();

    updateTrackInformation();
}

//! [0]
QUrl TrackManager::nextTrackUrl() const
{
    // The next track will have the current track number incremented by one
    const int nextTrack = m_trackCounter + 1;

    // Return an URL in the form "app/native/tracks/track001.m4a"
    return QUrl(QString::fromLatin1("%1/track%2.m4a").arg(QDir(trackStorageLocation()).absolutePath())
                                                     .arg(nextTrack, 3, 10, QLatin1Char('0')));
}
//! [0]

//! [1]
void TrackManager::clearAllTracks()
{
    // Iterate over all files in the track directory and delete them
    QDirIterator it(trackStorageLocation(), QDir::Files | QDir::NoDotAndDotDot);
    while (it.hasNext()) {
        it.next();

        QFile::remove(it.fileInfo().absoluteFilePath());
    }

    updateTrackInformation();
}
//! [1]

void TrackManager::update()
{
    updateTrackInformation();
}

GroupDataModel* TrackManager::model() const
{
    return m_model;
}

bool TrackManager::hasRecordedTracks() const
{
    // If the model is empty, we have no recorded tracks
    return (m_model->childCount(QVariantList()) != 0);
}

//! [2]
void TrackManager::updateTrackInformation()
{
    const bool oldHasRecordedTracks = hasRecordedTracks();

    // Clear the content of the tracks model
    m_model->clear();

    // Reset the internal track counter
    m_trackCounter = 0;

    // Iterate over the track storage directory and fill the model
    QDirIterator it(trackStorageLocation(), QDir::Files | QDir::NoDotAndDotDot);
    while (it.hasNext()) {
        it.next();

        QVariantMap entry;
        entry["name"] = it.fileName(); // used as title in the ListView
        entry["url"] = it.fileInfo().absoluteFilePath(); // used by the MediaPlayer

        m_model->insert(entry);

        m_trackCounter++;
    }

    // Emit change notification signal if the hasRecordedTracks property has changed
    if (oldHasRecordedTracks != hasRecordedTracks())
        emit hasRecordedTracksChanged();
}
//! [2]
