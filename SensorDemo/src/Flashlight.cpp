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

#include "Flashlight.hpp"

#include <QDebug>

Flashlight::Flashlight(QObject * parent)
    : QObject(parent)
    , m_enabled(false)
{
    // Acquire access to the camera hardware
    const camera_error_t error = camera_open(CAMERA_UNIT_REAR, CAMERA_MODE_PREAD | CAMERA_MODE_PWRITE, &m_camera);
    if (CAMERA_EOK != error) {
        qDebug() << "Failed to open camera";
    }
}

Flashlight::~Flashlight()
{
    // Release access to the camera hardware
    camera_close(m_camera);
}

void Flashlight::setEnabled(bool enabled)
{
    if (m_enabled == enabled)
        return;

    m_enabled = enabled;

    // Switch the flash light on/off depending on the value of the 'enabled' property
    const camera_videolightmode_t mode = (m_enabled ? CAMERA_VIDEOLIGHT_ON : CAMERA_VIDEOLIGHT_OFF);
    const camera_error_t error = camera_config_videolight(m_camera, mode);
    if (CAMERA_EOK != error) {
        qDebug() << "Failed to toggle the front camera video light";
    }

    emit enabledChanged(m_enabled);
}

bool Flashlight::enabled() const
{
    return m_enabled;
}
