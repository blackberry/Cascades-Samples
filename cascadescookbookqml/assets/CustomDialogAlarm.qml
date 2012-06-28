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
import "Common"

// This is the CustomDialog for the CustomDialog recipe. It is a custom built
// overlay illustrating alarm with at toggle button for dismissal.

CustomDialog {
    
    Container {
        // The dialog does not automatically fill the entire screen like a Page does, so in order
        // for it to be possible to center the dialog on screen, the width and height has to be set.
        preferredWidth: 768
        preferredHeight: 1280
        
        // The background is set to semi transparent to indicate that it is not possible to interact
        // with the screen behind the dialog.
        background: Color.create (0.0, 0.0, 0.0, 0.5)

        layout: DockLayout {
        }

        Container {
            maxHeight: 397
            
            layout: DockLayout {
            }

            layoutProperties: DockLayoutProperties {
                horizontalAlignment: HorizontalAlignment.Center
                verticalAlignment: VerticalAlignment.Center
            }

            ImageView {
                imageSource: "asset:///images/customdialog/customdialog_alarm"
            }
            
            Container {
                layout: StackLayout {
                    topPadding: 5
                    bottomPadding: 23
                    leftPadding: 23
                }
                
                layoutProperties: DockLayoutProperties {
                    horizontalAlignment: HorizontalAlignment.Fill
                    verticalAlignment: VerticalAlignment.Fill
                }
                
                Label {
                    text: "FIRE ALARM!"
                    textStyle.base: SystemDefaults.TextStyles.TitleText
                    textStyle.color: Color.create("#fafafa")
                    layoutProperties: StackLayoutProperties {
                        horizontalAlignment: HorizontalAlignment.Center
                        spaceQuota: 1
                    }
                }
                
                Label {
                    text: "BEEP! BEEP! BEEP! "
                    textStyle.base: SystemDefaults.TextStyles.TitleText
                    layoutProperties: StackLayoutProperties {
                        spaceQuota: 2.5
                    }
                }
                
                ToggleButton {
                    id: fireAlarm
                    checked: true

                    onCheckedChanged: {
                        // Close the dialog when the toggle button turns the fire alarm off.
                        if(!checked) {
                            customdialog.visible = false
                        }
                    }
                    layoutProperties: StackLayoutProperties {
                        horizontalAlignment: HorizontalAlignment.Center
                        spaceQuota: 1
                    }
                }
            }
        }
    }

    onVisibleChanged: {
        if(visible) {
            // Reset the fire alarm as it is opened.
            fireAlarm.checked = true;
        }
    }
}