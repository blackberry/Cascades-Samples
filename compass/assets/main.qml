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

import bb.cascades 1.0

Page {
    Container {
        layout: DockLayout { }

        //! [0]
        CompassView {
            rotationZ: -_compass.azimuth

            // Disable implicit animations to avoid ugly "jumps" when switching from 0 degrees to 360 degrees and vice versa
            attachedObjects: ImplicitAnimationController {
                propertyName: "rotationZ"
                enabled: false
            }
        }
        //! [0]

        ImageView {
            horizontalAlignment: HorizontalAlignment.Fill
            verticalAlignment: VerticalAlignment.Fill

            imageSource: "asset:///images/COMPASS-CIRCLE-BG.png"
        }

        Container {
            horizontalAlignment: HorizontalAlignment.Center
            verticalAlignment: VerticalAlignment.Top

            topPadding: 170

            //! [1]
            Label {
                text: Math.round(_compass.azimuth) + "°"
                textStyle {
                    base: SystemDefaults.TextStyles.BigText
                    color: Color.White
                    fontWeight: FontWeight.Bold
                }
            }
            //! [1]
        }
    }
}
