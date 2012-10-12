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

import bb.cascades 1.0
import QtMobility.sensors 1.2

TabbedPane {
    id: tabPane

    showTabsOnActionBar: true

//! [0]
    onCreationCompleted: {
        OrientationSupport.supportedDisplayOrientation = SupportedDisplayOrientation.All;
        tabPane.activeTab = compassTab;
    }
//! [0]

//! [1]
    Tab {
        id: alarmTab
        title: qsTr("Motion Alarm")
        imageSource: "images/alarm.png"

        Page {
            ControlDelegate {
                source: "motionalarm.qml"
                delegateActive: (tabPane.activeTab == alarmTab)
            }
        }
    }
//! [1]

    Tab {
        id: compassTab
        title: qsTr("Compass")
        imageSource: "images/compass.png"

        Page {
            ControlDelegate {
                source: "compass.qml"
                delegateActive: (tabPane.activeTab == compassTab)
            }
        }
    }

    Tab {
        id: metalFinderTab
        title: qsTr("Metal Detector")
        imageSource: "images/magnet.png"

        Page {
            ControlDelegate {
                source: "metalfinder.qml"
                delegateActive: (tabPane.activeTab == metalFinderTab)
            }
        }
    }

    Tab {
        id: flashlightTab
        title: qsTr("Flashlight")
        imageSource: "images/flashlight.png"

        Page {
            ControlDelegate {
                source: "flashlight.qml"
                delegateActive: (tabPane.activeTab == flashlightTab)
            }
        }
    }

    Tab {
        id: collisionDetectorTab
        title: qsTr("Collision Detector")
        imageSource: "images/crash.png"

        Page {
            ControlDelegate {
                source: "collisiondetector.qml"
                delegateActive: (tabPane.activeTab == collisionDetectorTab)
            }
        }
    }

    Tab {
        id: rotation3DTab
        title: qsTr("Rotation 3D")
        imageSource: "images/rotation3d.png"

        Page {
            ControlDelegate {
                source: "rotation3D.qml"
                delegateActive: (tabPane.activeTab == rotation3DTab)
            }
        }
    }
}
