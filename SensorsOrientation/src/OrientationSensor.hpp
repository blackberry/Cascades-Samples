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

#ifndef ORIENTATIONSENSOR_HPP
#define ORIENTATIONSENSOR_HPP

#include <QObject>
#include <QtSensors/QOrientationFilter>

QTM_USE_NAMESPACE

/**
 * The OrientationSensor class uses the QOrientationSensor class from the QtSensors
 * module to retrieve the current orientation/direction of the device.
 */
//! [0]
class OrientationSensor : public QObject, QOrientationFilter
{
    Q_OBJECT

    // The property to access the orientation value of the sensor
    Q_PROPERTY(Orientation orientation READ orientation NOTIFY orientationChanged)

public:
    enum Orientation {
        Undefined = 0,
        TopUp,
        TopDown,
        LeftUp,
        RightUp,
        FaceUp,
        FaceDown
    };
    Q_ENUMS(Orientation);

    OrientationSensor(QObject *parent = 0);

    // The accessor method for the orientation property
    Orientation orientation() const;

Q_SIGNALS:
    // The change notification signal of the orientation property
    void orientationChanged();

protected:
    /**
     * This method is reimplemented from the QOrientationFilter interface and is
     * called by the QOrientationFilter whenever new values are available.
     */
    bool filter(QOrientationReading *reading);

private:
    // The orientation sensor
    QOrientationSensor m_sensor;

    // The orientation value
    Orientation m_orientation;
};
//! [0]

#endif
