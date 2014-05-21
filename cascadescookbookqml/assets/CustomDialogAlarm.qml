/* Copyright (c) 2012, 2013, 2014 BlackBerry Limited.
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

// This is the CustomDialog for the CustomDialog recipe. It is a custom-built
// overlay illustrating alarm with at toggle button for dismissal.
Dialog {    
    Container {
        horizontalAlignment: HorizontalAlignment.Fill
        verticalAlignment: VerticalAlignment.Fill

        // The background is set to semi-transparent to indicate that it is not possible to interact
        // with the screen behind the dialog.
        background: Color.create (0.0, 0.0, 0.0, 0.5)

        layout: DockLayout {
        }

        Container {
            maxHeight: ui.du(39.7)
            leftPadding: ui.du(3)
            rightPadding: leftPadding

            horizontalAlignment: HorizontalAlignment.Center
            verticalAlignment: VerticalAlignment.Center
            
            layout: DockLayout {
            }

            ImageView {
                imageSource: "asset:///images/customdialog/customdialog_alarm.amd"
                verticalAlignment: VerticalAlignment.Fill
            }
            
            Container {
                topPadding: ui.du(.5)
                bottomPadding: ui.du(2)
                leftPadding: ui.du(2)
                horizontalAlignment: HorizontalAlignment.Fill
                verticalAlignment: VerticalAlignment.Fill
                                
                Label {
                    text: "FIRE ALARM!"
                    textStyle.base: SystemDefaults.TextStyles.TitleText
                    textStyle.color: Color.create("#fafafa")
                    horizontalAlignment: HorizontalAlignment.Center
                    
                    layoutProperties: StackLayoutProperties {
                        spaceQuota: 1
                    }
                }
                
                Label {
                    text: "BEEP! BEEP! BEEP! "
                    textStyle.base: SystemDefaults.TextStyles.TitleText
                    textStyle.color: Color.create("#262626")
                    layoutProperties: StackLayoutProperties {
                        spaceQuota: 2.5
                    }
                }
                
                ToggleButton {
                    id: fireAlarm
                    checked: true
                    horizontalAlignment: HorizontalAlignment.Center
                    layoutProperties: StackLayoutProperties {
                        spaceQuota: 1
                    }

                    onCheckedChanged: {
                        // Close the dialog when the ToggleButton turns the fire alarm off.
                        if(!checked) {
                            customdialog.close();
                        }
                    }
                }
            }
        }
    }

    onOpened: {
        // Reset the fire alarm as it is opened.
        fireAlarm.checked = true;
    }
}
