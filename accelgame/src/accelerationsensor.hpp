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

#ifndef ACCELERATIONSENSOR_HPP
#define ACCELERATIONSENSOR_HPP

#include <QObject>
#include <QtSensors/QAccelerometerFilter>

QTM_USE_NAMESPACE

/**
 * The AccelerationSensor class uses the QAccelerometer class from the QtSensors
 * module to retrieve the current x/y/z values from the device.
 * It provides the values via properties, so that they could be read from
 * within the UI.
 */
//! [0]
class AccelerationSensor : public QObject, public QAccelerometerFilter
{
    Q_OBJECT

    // The properties to access the x/yz/ values of the acceleration sensor
    Q_PROPERTY(qreal x READ x NOTIFY xChanged)
    Q_PROPERTY(qreal y READ y NOTIFY yChanged)
    Q_PROPERTY(qreal z READ z NOTIFY zChanged)

public:
    AccelerationSensor(QObject *parent = 0);

    // The accessor methods for the value properties
    qreal x() const;
    qreal y() const;
    qreal z() const;

public Q_SLOTS:
    // Start gathering sensor values
    void start();

    // Stop gathering sensor values
    void stop();

Q_SIGNALS:
    // The change notification signals of the value properties
    void xChanged();
    void yChanged();
    void zChanged();

protected:
    /**
     * Called by the QAccelerometer whenever new values are available.
     */
    bool filter(QAccelerometerReading *reading);

private:
    // The acceleration sensor
    QAccelerometer m_accelerationSensor;

    // The value properties
    qreal m_x;
    qreal m_y;
    qreal m_z;
};
//! [0]

#endif
