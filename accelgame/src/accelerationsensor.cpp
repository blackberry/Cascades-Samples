/*
 * Copyright (c) 2011, 2012, 2013  BlackBerry Limited.
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

#include "accelerationsensor.hpp"

//! [0]
AccelerationSensor::AccelerationSensor(QObject *parent)
    : QObject(parent)
    , m_x(0)
    , m_y(0)
    , m_z(0)
{
    // At first we have to connect to the sensor backend...
    if (!m_accelerationSensor.connectToBackend())
        qWarning() << "Cannot connect to acceleration sensor backend!";

    // ... and then add a filter that will process the read data
    m_accelerationSensor.addFilter(this);

    // Use only the gravity information
    m_accelerationSensor.setAccelerationMode(QAccelerometer::Gravity);

    // Automatically map the sensor information according to current device orientation
    m_accelerationSensor.setAxesOrientationMode(QAccelerometer::AutomaticOrientation);
}
//! [0]

qreal AccelerationSensor::x() const
{
    return m_x;
}


qreal AccelerationSensor::y() const
{
    return m_y;
}


qreal AccelerationSensor::z() const
{
    return m_z;
}

void AccelerationSensor::start()
{
    m_accelerationSensor.start();
}

void AccelerationSensor::stop()
{
    m_accelerationSensor.stop();
}

//! [1]
bool AccelerationSensor::filter(QAccelerometerReading *reading)
{
    // Store the previous values...
    const qreal oldX = m_x;
    const qreal oldY = m_y;
    const qreal oldZ = m_z;

    // ... update the property values with current sensor values
    m_x = reading->x();
    m_y = reading->y();
    m_z = reading->z();

    // ... and emit changed signals
    if (oldX != m_x)
        emit xChanged();
    if (oldY != m_y)
        emit yChanged();
    if (oldZ != m_z)
        emit zChanged();

    // Do no further processing of the sensor data
    return false;
}
//! [1]
