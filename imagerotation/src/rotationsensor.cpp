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
#include "rotationsensor.hpp"

#include <bb/cascades/OrientationSupport>

#include <QDebug>

using namespace bb::cascades;

//! [0]
RotationSensor::RotationSensor(QObject *parent)
    : QObject(parent)
    , m_rotation(0)
{
    // We'd like to lock to the initial orientation
    OrientationSupport::instance()->setSupportedDisplayOrientation(SupportedDisplayOrientation::CurrentLocked);

    // At first we have to connect to the sensor backend...
    if (!m_rotationSensor.connectToBackend()) {
        qWarning() << "Cannot connect to rotation sensor backend!";
    }

    // ... and then add a filter that will process the read data
    m_rotationSensor.addFilter(this);

    // Start gathering the data
    m_rotationSensor.start();
}
//! [0]

qreal RotationSensor::rotation() const
{
    return m_rotation;
}

//! [1]
bool RotationSensor::filter(QRotationReading *reading)
{
    const qreal x = reading->x();
    const qreal y = reading->y();

    if (x > 0 && x < 90) {
        if (y > 0) { // top-left quadrant
            m_rotation = (x - 90);
        } else { // top-right quadrant
            m_rotation = (90 - x);
        }
    } else { // bottom-left quadrant
        if (y > 0) {
            m_rotation = (-90 + x);
        } else { // bottom right quadrant
            m_rotation = (90 - x);
        }
    }

    emit rotationChanged();

    // Do no further processing of the sensor data
    return false;
}
//! [1]
