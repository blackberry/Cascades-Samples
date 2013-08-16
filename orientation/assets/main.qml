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

import bb.cascades 1.2
import Utils 1.0

Page {
    Container {
        layout: DockLayout {}

        background: Color.Black

        //! [0]
        // Trigger the animation whenever the orientation has changed
        onCreationCompleted: _sensor.orientationChanged.connect(animation.play)
        //! [0]

        //! [1]
        Label {
            horizontalAlignment: HorizontalAlignment.Center
            verticalAlignment: VerticalAlignment.Center

            // Use a different text depending on current orientation
            text: _sensor.orientation == OrientationSensor.TopUp ? qsTr ("Top Up") :
                  _sensor.orientation == OrientationSensor.TopDown ? qsTr ("Top Down") :
                  _sensor.orientation == OrientationSensor.LeftUp ? qsTr ("Left Up") :
                  _sensor.orientation == OrientationSensor.RightUp ? qsTr ("Right Up") :
                  _sensor.orientation == OrientationSensor.FaceUp ? qsTr ("Face Up") :
                  _sensor.orientation == OrientationSensor.FaceDown ? qsTr ("Face Down") : ""

            textStyle {
                base: SystemDefaults.TextStyles.BigText
                color: Color.White
                fontWeight: FontWeight.Bold
            }

            scaleX: 0
            scaleY: 0

            animations: ScaleTransition {
                id: animation

                fromX: 0
                fromY: 0
                toX: 1.5
                toY: 1.5
            }
        }
        //! [1]
    }
}
