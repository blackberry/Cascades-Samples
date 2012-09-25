/****************************************************************************
 **
 ** Portions Copyright (C) 2012 Research In Motion Limited.
 ** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
 ** All rights reserved.
 ** Contact: Research In Motion Ltd. (http://www.rim.com/company/contact/)
 ** Contact: Nokia Corporation (qt-info@nokia.com)
 **
 ** This file is part of the examples of the BB10 Platform and is derived
 ** from a similar file that is part of the Qt Toolkit.
 **
 ** You may use this file under the terms of the BSD license as follows:
 **
 ** "Redistribution and use in source and binary forms, with or without
 ** modification, are permitted provided that the following conditions are
 ** met:
 **   * Redistributions of source code must retain the above copyright
 **     notice, this list of conditions and the following disclaimer.
 **   * Redistributions in binary form must reproduce the above copyright
 **     notice, this list of conditions and the following disclaimer in
 **     the documentation and/or other materials provided with the
 **     distribution.
 **   * Neither the name of Research In Motion, nor the name of Nokia
 **     Corporation and its Subsidiary(-ies), nor the names of its
 **     contributors may be used to endorse or promote products
 **     derived from this software without specific prior written
 **     permission.
 **
 ** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 ** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 ** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 ** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 ** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 ** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 ** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 ** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 ** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 ** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 ** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
 **
 ****************************************************************************/

#include "TossGame.hpp"

#include <QtCore/QDebug>

//! [0]
TossGame::TossGame(QObject *parent)
    : QObject(parent)
    , m_gesture(Rock)
    , m_active(false)
    , m_close(false)
{
    // At first we have to connect to the sensor backend...
    if (!m_sensor.connectToBackend()) {
        qWarning() << "Cannot connect to proximity sensor backend!";
    }

    // ... and then add a filter that will process the reported data
    m_sensor.addFilter(this);
}
//! [0]

//! [1]
bool TossGame::filter(QProximityReading *reading)
{
    if (m_close != reading->close()) { // Only react if the state changed
        m_close = reading->close();

        if (!m_close) { // If the user moves the device away from the body
            m_gesture = static_cast<Gesture>(qrand() % 3);
            emit gestureChanged();
        }
    }

    // Do no further processing of the sensor data
    return false;
}
//! [1]

TossGame::Gesture TossGame::gesture() const
{
    return m_gesture;
}

bool TossGame::active() const
{
    return m_active;
}

void TossGame::setActive(bool active)
{
    if (m_active == active)
        return;

    m_active = active;
    emit activeChanged();

    // Start or stop the gathering of data
    if (m_active)
        m_sensor.start();
    else
        m_sensor.stop();
}
