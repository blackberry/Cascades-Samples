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
#ifndef ROTATIONSENSOR_HPP
#define ROTATIONSENSOR_HPP

#include <QObject>
#include <QtSensors/QRotationSensor>

QTM_USE_NAMESPACE

/**
 * The RotationSensor class uses the QRotationSensor class from the QtSensors
 * module to retrieve the current x and y value from the rotation sensor of the device.
 * It calculates the rotation angle and provides it as a property, so that it can be used inside the UI.
 */
//! [0]
class RotationSensor : public QObject, public QRotationFilter
{
    Q_OBJECT

    // The property to access the rotation angle
    Q_PROPERTY(qreal rotation READ rotation NOTIFY rotationChanged)

public:
    RotationSensor(QObject *parent = 0);

    // The accessor methods for the properties
    qreal rotation() const;

Q_SIGNALS:
    // The change notification signals of the properties
    void rotationChanged();

protected:
    /**
     * This method is reimplemented from the QRotationFilter interface and is
     * called by the QRotationSensor whenever new values are available.
     */
    bool filter(QRotationReading *reading);

private:
    // The rotation sensor
    QRotationSensor m_rotationSensor;

    // The rotation property
    qreal m_rotation;
};
//! [0]

#endif
