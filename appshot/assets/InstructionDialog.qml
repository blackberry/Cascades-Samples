/* Copyright (c) 2013, 2014 BlackBerry Limited.
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

Dialog {
    content: contentDelegate.object

    attachedObjects: [
        Delegate {
            id: contentDelegate

            // A component that is used the first time the app starts, it
            // display a simple instruction so that the use knows how to use the app.
            Container {
                id: introContainer
                rightPadding: ui.du(3)
                leftPadding: ui.du(3)
                background: Color.create("#121212")
                opacity: 0.75
                horizontalAlignment: HorizontalAlignment.Fill
                verticalAlignment: VerticalAlignment.Fill

                layout: DockLayout {
                }

                animations: FadeTransition {
                    id: hideAnim
                    fromOpacity: introContainer.opacity
                    toOpacity: 0
                    duration: 300

                    onEnded: {
                        close()
                    }
                }

                Label {
                    text: qsTr("Press screen for new pattern. Use \"Shoot Screen\" to set as wallpaper or share.")
                    multiline: true
                    horizontalAlignment: HorizontalAlignment.Center
                    verticalAlignment: VerticalAlignment.Center
                    textStyle.base: SystemDefaults.TextStyles.BigText
                    textStyle.color: Color.create("#fefefe")
                    textStyle.fontStyle: FontStyle.Italic
                }

                onTouch: {
                    if (event.touchType == TouchType.Down) {
                        hideAnim.play();

                        // Disable touch after the hide animation has started to avoid multiple triggers.
                        introContainer.touchPropagationMode = TouchPropagationMode.None
                    }
                }
            }
        }
    ]

    onOpened: {
        // Set the content to active to create the dialog content Container.
        contentDelegate.active = true;
    }

    onClosed: {
        // Disable the contentDelegate to unload its object.
        contentDelegate.active = false;
        _app.showInstruction = false;
    }
}