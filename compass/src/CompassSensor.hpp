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

#ifndef COMPASSSENSOR_HPP
#define COMPASSSENSOR_HPP

#include <QObject>
#include <QtSensors/QCompassFilter>

QTM_USE_NAMESPACE

/**
 * The CompassSensor class uses the QCompass class from the QtSensors
 * module to retrieve the current azimuth values from the compass sensor of the device.
 */
//! [0]
class CompassSensor : public QObject, public QCompassFilter
{
    Q_OBJECT

    // The property to access the azimuth value of the compass sensor
    Q_PROPERTY(qreal azimuth READ azimuth NOTIFY azimuthChanged)

public:
    CompassSensor(QObject *parent = 0);

    // The accessor method for the azimuth property
    qreal azimuth() const;

Q_SIGNALS:
    // The change notification signal of the azimuth property
    void azimuthChanged();

protected:
    /**
     * This method is reimplemented from the QCompassFilter interface and is
     * called by the QCompass whenever new values are available.
     */
    bool filter(QCompassReading *reading);

private:
    // The compass sensor
    QCompass m_compassSensor;

    // The azimuth value
    qreal m_azimuth;
};
//! [0]

#endif
