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

#ifndef LOCATIONSESSION_HPP
#define LOCATIONSESSION_HPP

#include <QtLocationSubset/QGeoPositionInfo>
#include <QtLocationSubset/QGeoPositionInfoSource>
#include <QtLocationSubset/QGeoSatelliteInfo>
#include <QtLocationSubset/QGeoSatelliteInfoSource>

#include <QtCore/QObject>

using namespace QtMobilitySubset;

/**
 * @short A helper class that encapsulates the retrieval of location information.
 */
//! [0]
class LocationSession: public QObject
{
    Q_OBJECT

    // The properties that provide the location information to the UI
    Q_PROPERTY(QString latitude READ latitude NOTIFY dataChanged)
    Q_PROPERTY(QString longitude READ longitude NOTIFY dataChanged)
    Q_PROPERTY(QString altitude READ altitude NOTIFY dataChanged)
    Q_PROPERTY(QString time READ time NOTIFY dataChanged)
    Q_PROPERTY(QString direction READ direction NOTIFY dataChanged)
    Q_PROPERTY(QString groundSpeed READ groundSpeed NOTIFY dataChanged)
    Q_PROPERTY(QString verticalSpeed READ verticalSpeed NOTIFY dataChanged)
    Q_PROPERTY(QString horizontalAccuracy READ horizontalAccuracy NOTIFY dataChanged)
    Q_PROPERTY(QString verticalAccuracy READ verticalAccuracy NOTIFY dataChanged)
    Q_PROPERTY(QString magneticVariation READ magneticVariation NOTIFY dataChanged)
    Q_PROPERTY(QString log READ log NOTIFY logChanged)

public:
    LocationSession(QObject* parent, bool satInfo);

    // This method is called to trigger the continious retrieval of location information
    void startUpdates();

    // This method is called to trigger an one-time retrieval of location information
    void requestUpdate();

    // The accessor method for the internal geo position object
    QGeoPositionInfoSource* positionSource() const;

    // Sets whether a sound should be played on retrieval of new location information
    void setSoundEnabled(bool enabled);

    // This method is called to stop the retrieval of location information
    Q_INVOKABLE void stopUpdates();

    // This method is called to reset the internal retrieval engine
    Q_INVOKABLE void resetSession(const QString &type);

Q_SIGNALS:
    // The change notification signals of the properties
    void dataChanged();
    void logChanged();

private Q_SLOTS:
    // This slot is invoked whenever new location information are retrieved
    void positionUpdated(const QGeoPositionInfo & pos);

    // This slot is invoked whenever a timeout happend while retrieving location information
    void positionUpdateTimeout();

    // This slot is invoked whenever new information about the used satellites are retrieved
    void satellitesInUseUpdated(const QList<QGeoSatelliteInfo> & satellites);

    // This slot is invoked whenever new information about the in-view satellites are retrieved
    void satellitesInViewUpdated(const QList<QGeoSatelliteInfo> & satellites);

private:
    // A helper message to log events
    void log(const QString &msg);

    // The accessor methods of the properties
    QString latitude() const;
    QString longitude() const;
    QString altitude() const;
    QString time() const;
    QString direction() const;
    QString groundSpeed() const;
    QString verticalSpeed() const;
    QString horizontalAccuracy() const;
    QString verticalAccuracy() const;
    QString magneticVariation() const;
    QString log() const;

    // The flag for sound usage
    bool m_soundEnabled;

    // The central object to retrieve location information
    QGeoPositionInfoSource *m_positionSource;

    // The central object to retrieve satellite information
    QGeoSatelliteInfoSource *m_satelliteSource;

    // The property values
    QString m_latitude;
    QString m_longitude;
    QString m_altitude;
    QString m_time;
    QString m_direction;
    QString m_groundSpeed;
    QString m_verticalSpeed;
    QString m_horizontalAccuracy;
    QString m_verticalAccuracy;
    QString m_magneticVariation;
    QString m_log;
};
//! [0]

#endif
