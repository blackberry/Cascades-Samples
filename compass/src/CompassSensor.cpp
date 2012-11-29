/*
 * Copyright (c) 2011, 2012 Research In Motion Limited.
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

#include "CompassSensor.hpp"

#include <bb/cascades/OrientationSupport>

#include <QDebug>

using namespace bb::cascades;

//! [0]
CompassSensor::CompassSensor(QObject *parent)
    : QObject(parent)
    , m_azimuth(0)
{
    // We'd like to lock to the initial orientation
    OrientationSupport::instance()->setSupportedDisplayOrientation(SupportedDisplayOrientation::CurrentLocked);

    // At first we have to connect to the sensor backend...
    if (!m_compassSensor.connectToBackend()) {
        qWarning() << "Cannot connect to compass sensor backend!";
    }

    // ... and then add a filter that will process the read data
    m_compassSensor.addFilter(this);

    // Do not report duplicated values
    m_compassSensor.setSkipDuplicates(true);

    // Start gathering the data
    m_compassSensor.start();
}
//! [0]

qreal CompassSensor::azimuth() const
{
    return m_azimuth;
}

//! [1]
bool CompassSensor::filter(QCompassReading *reading)
{
    // Store the previous azimuth
    const qreal oldAzimuth = m_azimuth;

    m_azimuth = reading->azimuth();

    // Emit changed signal if azimuth has changed
    if (oldAzimuth != m_azimuth)
        emit azimuthChanged();

    // Do no further processing of the sensor data
    return false;
}
//! [1]
