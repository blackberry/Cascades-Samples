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

// A CustomListItem used instead of the StandardListItems to be able to use different colors.
CustomListItem {
    property string title
    dividerVisible: false

    Container {
        verticalAlignment: VerticalAlignment.Fill
        horizontalAlignment: HorizontalAlignment.Fill

        layout: DockLayout {
        }

        // Title.
        Container {
            leftPadding: ui.du(2) 
            rightPadding: ui.du(2) 
            verticalAlignment: VerticalAlignment.Center

            Label {
                text: title
                textStyle {
                    base: SystemDefaults.TextStyles.PrimaryText
                    color: Color.Black
                }
            }
        }
        
        // Divider.
        Container {
            horizontalAlignment: HorizontalAlignment.Fill
            verticalAlignment: VerticalAlignment.Bottom
            background: ui.palette.primaryBase
            minHeight: ui.du(0.1)
        }
    }
}
