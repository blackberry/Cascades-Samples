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

#ifndef TRACKMANAGER_HPP
#define TRACKMANAGER_HPP

#include <bb/cascades/GroupDataModel>
#include <QtCore/QObject>

/**
 * @short A utility class to manage the recorded tracks.
 *
 * The TrackManager class provides information about the already
 * recorded tracks and about the name of the next track to record.
 */
//! [0]
class TrackManager : public QObject
{
    Q_OBJECT

    // A model that contains a list of all recorded tracks
    Q_PROPERTY(bb::cascades::GroupDataModel* model READ model CONSTANT)

    // A flag that defines whether recorded tracks are available or not
    Q_PROPERTY(bool hasRecordedTracks READ hasRecordedTracks NOTIFY hasRecordedTracksChanged)

public:
    TrackManager(QObject *parent = 0);

    // This method is invoked to get the target URL for the next track to record.
    Q_INVOKABLE QUrl nextTrackUrl() const;

public Q_SLOTS:
    // This method is invoked to clear all recorded tracks
    void clearAllTracks();

    // This method is invoked to update the internal state of the track manager.
    void update();

Q_SIGNALS:
    // The change notification signal of the property
    void hasRecordedTracksChanged();

private:
    // A helper method to update internal data from the track storage location
    void updateTrackInformation();

    // The accessor methods of the properties
    bb::cascades::GroupDataModel* model() const;
    bool hasRecordedTracks() const;

    // The model that contains the list of recorded tracks
    bb::cascades::GroupDataModel *m_model;

    // The internal track counter
    unsigned int m_trackCounter;
};
//! [0]

#endif
