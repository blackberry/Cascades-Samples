/* Copyright (c) 2013 Research In Motion Limited.
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

// This is the file used for the SceneCover shown when the application
// is running in minimized mode on the running apps page.

// In the SceneCover the content property keyword has to be explicitly added
Container {
    Container {        
        layout: DockLayout {
        }
        background: Color.Black
        ImageView {
            imageSource: "asset:///images/application-cover.png"
            scalingMethod: ScalingMethod.AspectFit
        }

        // A title is put on a dark background, it is pixel-aligned in order
        // to get it to match the checkered pattern in the background image.
        Container {
            bottomPadding: 31
            horizontalAlignment: HorizontalAlignment.Center
            verticalAlignment: VerticalAlignment.Bottom

            Container {
                preferredWidth: 84
                preferredHeight: 42
                background: Color.create("#121212")
                layout: DockLayout {
                }
                Label {
                    horizontalAlignment: HorizontalAlignment.Center
                    verticalAlignment: VerticalAlignment.Center
                    text: "QML"
                    textStyle.color: Color.create("#ebebeb")
                    textStyle.fontSize: FontSize.PointValue
                    textStyle.fontSizeValue: 6
                }
            }
        }
    }
}
