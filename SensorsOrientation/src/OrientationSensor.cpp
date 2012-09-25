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

#include <QDebug>

#include "OrientationSensor.hpp"

//! [0]
OrientationSensor::OrientationSensor(QObject *parent)
    : QObject(parent)
    , m_orientation(Undefined)
{

    // At first we have to connect to the sensor backend...
    if (!m_sensor.connectToBackend()) {
        qWarning() << "Cannot connect to orientation sensor backend!";
    }

    // ... and then add a filter that will process the read data
    m_sensor.addFilter(this);

    // Start gathering the data
    m_sensor.start();
}
//! [0]

OrientationSensor::Orientation OrientationSensor::orientation() const
{
    return m_orientation;
}

//! [1]
bool OrientationSensor::filter(QOrientationReading *reading)
{
    // Store the previous orientation
    const Orientation oldOrientation = m_orientation;

    switch (reading->orientation()) {
    case QOrientationReading::Undefined:
        m_orientation = Undefined;
        break;
    case QOrientationReading::TopUp:
        m_orientation = TopUp;
        break;
    case QOrientationReading::TopDown:
        m_orientation = TopDown;
        break;
    case QOrientationReading::LeftUp:
        m_orientation = LeftUp;
        break;
    case QOrientationReading::RightUp:
        m_orientation = RightUp;
        break;
    case QOrientationReading::FaceUp:
        m_orientation = FaceUp;
        break;
    case QOrientationReading::FaceDown:
        m_orientation = FaceDown;
        break;
    }

    // Emit changed signal if orientation has changed
    if (m_orientation != oldOrientation)
        emit orientationChanged();

    // Do no further processing of the sensor data
    return false;
}
//! [1]
