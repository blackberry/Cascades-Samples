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

#ifndef FLASHLIGHT_HPP
#define FLASHLIGHT_HPP

#include <QObject>

#include <camera/camera_api.h>

class Flashlight: public QObject
{
    Q_OBJECT

    // Defines whether the flash light of the camera is enabled or not
    Q_PROPERTY(bool enabled READ enabled WRITE setEnabled NOTIFY enabledChanged)

public:
    Flashlight(QObject *parent = 0);
    ~Flashlight();

    // The accessor methods for the property
    void setEnabled(bool enabled);
    bool enabled() const;

Q_SIGNALS:
    // The change notification signal of the property
    void enabledChanged(bool);

private:
    // The 
    bool m_enabled;
    camera_handle_t m_camera;
};

#endif
