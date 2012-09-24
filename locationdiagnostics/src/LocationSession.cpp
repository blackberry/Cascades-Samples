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

#include "LocationSession.hpp"

#include <bb/multimedia/SystemSound>

#include <QtCore/QVariant>

using namespace bb::multimedia;

//! [0]
LocationSession::LocationSession(QObject* parent, bool satInfo)
    : QObject(parent)
    , m_soundEnabled(false)
    , m_positionSource(QGeoPositionInfoSource::createDefaultSource(this))
{
    if (m_positionSource) {
        connect(m_positionSource, SIGNAL(positionUpdated(const QGeoPositionInfo &)), this, SLOT(positionUpdated(const QGeoPositionInfo &)));
        connect(m_positionSource, SIGNAL(updateTimeout()), this, SLOT(positionUpdateTimeout()));

        log(tr("Initialized QGeoPositionInfoSource"));
    } else {
        log(tr("Failed to initialized QGeoPositionInfoSource"));
    }

    if (satInfo) {
        m_satelliteSource = QGeoSatelliteInfoSource::createDefaultSource(this);

        if (m_satelliteSource) {
            connect(m_satelliteSource, SIGNAL(satellitesInUseUpdated(const QList<QGeoSatelliteInfo> &)), this, SLOT(satellitesInUseUpdated(const QList<QGeoSatelliteInfo> &)));
            connect(m_satelliteSource, SIGNAL(satellitesInViewUpdated(const QList<QGeoSatelliteInfo> &)), this, SLOT(satellitesInViewUpdated(const QList<QGeoSatelliteInfo> &)));

            log(tr("Initialized QGeoSatelliteInfoSource"));
        } else {
            log(tr("Failed to initialized QGeoSatelliteInfoSource"));
        }
    }

    m_latitude = m_longitude = m_altitude = m_direction = m_horizontalAccuracy = m_verticalAccuracy = m_magneticVariation = tr("--");
    m_time = tr("-/-/- -:-");
    m_groundSpeed = tr("-- km/h");
    m_verticalSpeed = tr("-- km/h");
}
//! [0]

//! [1]
void LocationSession::startUpdates()
{
    if (m_positionSource) {
        m_positionSource->startUpdates();
        log(tr("Updates started"));
    }
}
//! [1]

//! [2]
void LocationSession::requestUpdate()
{
    if (m_positionSource) {
        m_positionSource->requestUpdate(120000);
        log(tr("Update requested"));
    }
}
//! [2]

//! [3]
void LocationSession::stopUpdates()
{
    if (m_positionSource) {
        m_positionSource->stopUpdates();
        log(tr("Updates stopped"));
    }

    deleteLater();
}
//! [3]

//! [4]
void LocationSession::resetSession(const QString &type)
{
    if (m_soundEnabled)
        SystemSound::play(SystemSound::GeneralNotification);

    if (m_positionSource) {
        m_positionSource->stopUpdates();
        m_positionSource->setProperty("reset", type);
        m_positionSource->startUpdates();
        log(tr("%1 RESET").arg(type));
    }
}
//! [4]

//! [5]
void LocationSession::positionUpdated(const QGeoPositionInfo& pos)
{
    if (m_soundEnabled)
        SystemSound::play(SystemSound::GeneralNotification);

    m_latitude = QString::number(pos.coordinate().latitude());
    m_longitude = QString::number(pos.coordinate().longitude());
    m_altitude = QString::number(pos.coordinate().altitude());
    m_time = pos.timestamp().toString();
    m_direction = QString::number(pos.attribute(QGeoPositionInfo::Direction));
    m_groundSpeed = QString::number(pos.attribute(QGeoPositionInfo::GroundSpeed));
    m_verticalSpeed = QString::number(pos.attribute(QGeoPositionInfo::VerticalSpeed));
    m_horizontalAccuracy = QString::number(pos.attribute(QGeoPositionInfo::HorizontalAccuracy));
    m_verticalAccuracy = QString::number(pos.attribute(QGeoPositionInfo::VerticalAccuracy));
    m_magneticVariation = QString::number(pos.attribute(QGeoPositionInfo::MagneticVariation));

    emit dataChanged();

    log(tr("update"));
}
//! [5]

QGeoPositionInfoSource* LocationSession::positionSource() const
{
    return m_positionSource;
}

void LocationSession::positionUpdateTimeout()
{
    log(tr("positionUpdateTimeout() received"));
}

void LocationSession::satellitesInUseUpdated(const QList<QGeoSatelliteInfo>&)
{
    log(tr("satellitesInUseUpdated received"));
}

void LocationSession::setSoundEnabled(bool enabled)
{
    m_soundEnabled = enabled;
}

void LocationSession::satellitesInViewUpdated(const QList<QGeoSatelliteInfo>&)
{
    log(tr("satellitesInViewUpdated received"));
}

void LocationSession::log(const QString &msg)
{
    m_log += msg + QLatin1String("\n");
    emit logChanged();
}

QString LocationSession::latitude() const
{
    return m_latitude;
}

QString LocationSession::longitude() const
{
    return m_longitude;
}

QString LocationSession::altitude() const
{
    return m_altitude;
}

QString LocationSession::time() const
{
    return m_time;
}

QString LocationSession::direction() const
{
    return m_direction;
}

QString LocationSession::groundSpeed() const
{
    return m_groundSpeed;
}

QString LocationSession::verticalSpeed() const
{
    return m_verticalSpeed;
}

QString LocationSession::horizontalAccuracy() const
{
    return m_horizontalAccuracy;
}

QString LocationSession::verticalAccuracy() const
{
    return m_verticalAccuracy;
}

QString LocationSession::magneticVariation() const
{
    return m_magneticVariation;
}

QString LocationSession::log() const
{
    return m_log;
}
