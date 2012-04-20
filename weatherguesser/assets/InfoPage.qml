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

// The info page with some information about the application a bit of decorations.
Page {
    content: Container {
        layout: DockLayout {
        }

        ImageView {
            imageSource: "asset:///images/info/background.png"
            layoutProperties: DockLayoutProperties {
                horizontalAlignment: HorizontalAlignment.Fill
                verticalAlignment: VerticalAlignment.Fill
            }
        }

        Container {
            layout: StackLayout {
                leftPadding: 40
                bottomPadding: 180
            }
            layoutProperties: DockLayoutProperties {
                verticalAlignment: VerticalAlignment.Bottom
                horizontalAlignment: HorizontalAlignment.Left
            }
            ImageView {
                imageSource: "asset:///images/info/cloud.png"
            }
        }

        Container {
            layout: StackLayout {
                rightPadding: 50
                topPadding: 70
            }
            layoutProperties: DockLayoutProperties {
                verticalAlignment: VerticalAlignment.Top
                horizontalAlignment: HorizontalAlignment.Right
            }
            ImageView {
                imageSource: "asset:///images/info/sun.png"
            }
        }

        Container {

            layout: StackLayout {
                bottomPadding: 180
            }

            layoutProperties: DockLayoutProperties {
                horizontalAlignment: HorizontalAlignment.Center
                verticalAlignment: VerticalAlignment.Center
            }

            TextArea {
                text: "Welcome to the weather guesser. This little app will predict (guess) the weather, not only today or tomorrow, but for the whole year. Sounds to good to be true? Go ahead and try it."
                enabled: false
                editable: false
                preferredWidth: 650
                textStyle {
                    base: SystemDefaults.TextStyles.TitleText
                    fontWeight: FontWeight.Light
                    fontStyle: FontStyle.Italic
                    fontStyleHint: FontStyleHint.Serif
                    color: Color.White
                    lineSpacing: 1.1
                }
            }
        }

        Container {
            layout: StackLayout {
                rightPadding: 30
                bottomPadding: 30
            }

            layoutProperties: DockLayoutProperties {
                verticalAlignment: VerticalAlignment.Bottom
                horizontalAlignment: HorizontalAlignment.Right
            }

            Label {
                text: "Cascades sample app 2012."
                textStyle {
                    base: SystemDefaults.TextStyles.SmallText
                    fontWeight: FontWeight.Light
                    fontStyleHint: FontStyleHint.Serif
                    color: Color.White
                }
            }
        }
    }
}
