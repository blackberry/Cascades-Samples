/* Copyright (c) 2012, 2013  BlackBerry Limited.
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

import bb.cascades 1.3
import bb.device 1.0

Container {
    preferredWidth: ui.du(55.3)
    preferredHeight: ui.du(35.3)

    layout: DockLayout {}

    //! [0]
    ImageView {
        id: batteryOutline

        horizontalAlignment: HorizontalAlignment.Center
        verticalAlignment: VerticalAlignment.Center

        imageSource: _battery.chargingState == BatteryChargingState.Unknown ? "asset:///images/battery_plugged_error.png" :
                     _battery.chargingState == BatteryChargingState.NotCharging ? "asset:///images/battery_plugged_bad.png" :
                     _battery.chargingState == BatteryChargingState.Charging ? "asset:///images/battery_plugged.png" :
                     _battery.chargingState == BatteryChargingState.Discharging ? "asset:///images/battery.png" :
                     _battery.chargingState == BatteryChargingState.Full ? "asset:///images/battery_plugged.png" : ""
    }
    //! [0]

    //! [1]
    ImageView {
        id: loadingLevel

        horizontalAlignment: HorizontalAlignment.Left
        verticalAlignment: VerticalAlignment.Center

        translationX: 75
        preferredWidth: _battery.level * ui.du(38.8) / ui.du(11.1)

        imageSource: _battery.level <= 10 ? "asset:///images/fill_red.png" :
                     _battery.level <= 20 ? "asset:///images/fill_yellow.png" :
                     _battery.level < 100 ? "asset:///images/fill_grey.png" : "asset:///images/fill_green.png"
    }
    //! [1]

    ImageView {
        id: stateIndicator

        horizontalAlignment: HorizontalAlignment.Center
        verticalAlignment: VerticalAlignment.Center

        imageSource: _battery.chargingState == BatteryChargingState.Unknown ? "asset:///images/exclamation.png" :
                     _battery.chargingState == BatteryChargingState.NotCharging ? "asset:///images/exlamation.png" :
                     _battery.chargingState == BatteryChargingState.Charging ? "asset:///images/flash.png" :
                     _battery.chargingState == BatteryChargingState.Discharging ? "" :
                     _battery.chargingState == BatteryChargingState.Full ? "" : ""
    }
}
