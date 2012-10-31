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

#include <QDebug>
#include <QtLocationSubset/QGeoPositionInfo>
#include <QtLocationSubset/QGeoPositionInfoSource>
#include <QtLocationSubset/QGeoSatelliteInfo>
#include <QtLocationSubset/QGeoSatelliteInfoSource>
#include <bb/cascades/maps/MapView>
#include <bb/cascades/maps/MapData.hpp>
#include <bb/platform/geo/GeoLocation.hpp>
#include <bb/cascades/maps/MapLongPressToPinDrop.hpp>



#include <QtCore/QObject>

using namespace QtMobilitySubset;
using namespace bb::cascades::maps;
using namespace bb::platform::geo;

/**
 * @short A helper class that encapsulates the retrieval of location information.
 */
//! [0]
class LocationSession: public QObject
{
    Q_OBJECT

    // The properties that provide the location information to the UI
    Q_PROPERTY(QString method READ method NOTIFY dataChanged)
    Q_PROPERTY(double latitude READ latitude NOTIFY dataChanged)
    Q_PROPERTY(double longitude READ longitude NOTIFY dataChanged)
    Q_PROPERTY(double altitude READ altitude NOTIFY dataChanged)
    Q_PROPERTY(QString time READ time NOTIFY dataChanged)
    Q_PROPERTY(QString direction READ direction NOTIFY dataChanged)
    Q_PROPERTY(QString groundSpeed READ groundSpeed NOTIFY dataChanged)
    Q_PROPERTY(QString verticalSpeed READ verticalSpeed NOTIFY dataChanged)
    Q_PROPERTY(QString horizontalAccuracy READ horizontalAccuracy NOTIFY dataChanged)
    Q_PROPERTY(QString verticalAccuracy READ verticalAccuracy NOTIFY dataChanged)
    Q_PROPERTY(QString magneticVariation READ magneticVariation NOTIFY dataChanged)
    Q_PROPERTY(QString horizontalDilution READ horizontalDilution NOTIFY dataChanged)
    Q_PROPERTY(QString verticalDilution READ verticalDilution NOTIFY dataChanged)
    Q_PROPERTY(QString positionDilution READ positionDilution NOTIFY dataChanged)
    Q_PROPERTY(QString ttff READ ttff NOTIFY dataChanged)
    Q_PROPERTY(QString gpsWeek READ gpsWeek NOTIFY dataChanged)
    Q_PROPERTY(QString gpsTimeOfWeek READ gpsTimeOfWeek NOTIFY dataChanged)
    Q_PROPERTY(bool isPropagated READ isPropagated NOTIFY dataChanged)

    Q_PROPERTY(QString satellitesInUse READ satellitesInUse NOTIFY dataChanged)
    Q_PROPERTY(QString satellitesInView READ satellitesInView NOTIFY dataChanged)

    Q_PROPERTY(QString log READ log NOTIFY logChanged)

public:
    LocationSession(QObject* parent, bool satInfo, MapView *map);

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

    // This mehod is called to expose the MapView created in qml to this.
    Q_INVOKABLE void setMapView(MapView *value);

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

    // This slot is invoked whenever a new pin is created by long pressing on the mapView.
    void onPinCreated(const QString& pinID);

private:
    // A helper method to parse the raw geo information
    void parseRawData();

    // A helper message to log events
    void log(const QString &msg, bool showInUi = true);

    // The accessor methods of the properties
    QString method() const;
    double latitude() const;
    double longitude() const;
    double altitude() const;
    QString time() const;
    QString direction() const;
    QString groundSpeed() const;
    QString verticalSpeed() const;
    QString horizontalAccuracy() const;
    QString verticalAccuracy() const;
    QString magneticVariation() const;
    QString horizontalDilution() const;
    QString verticalDilution() const;
    QString positionDilution() const;
    QString ttff() const;
    QString gpsWeek() const;
    QString gpsTimeOfWeek() const;
    bool isPropagated() const;
    QString satellitesInUse() const;
    QString satellitesInView() const;
    QString log() const;

    // The flag for sound usage
    bool m_soundEnabled;

    // The central object to retrieve location information
    QGeoPositionInfoSource *m_positionSource;

    // The central object to retrieve satellite information
    QGeoSatelliteInfoSource *m_satelliteSource;

    // The property values
    QString m_method;
    double m_latitude;
    double m_longitude;
    double m_altitude;
    QString m_time;
    QString m_direction;
    QString m_groundSpeed;
    QString m_verticalSpeed;
    QString m_horizontalAccuracy;
    QString m_verticalAccuracy;
    QString m_magneticVariation;
    QString m_horizontalDilution;
    QString m_verticalDilution;
    QString m_positionDilution;
    QString m_ttff;
    QString m_gpsWeek;
    QString m_gpsTimeOfWeek;
    bool m_isPropagated;
    QString m_satellitesInUse;
    QString m_satellitesInView;
    QString m_log;

    MapView *mapView;
};
//! [0]

#endif
