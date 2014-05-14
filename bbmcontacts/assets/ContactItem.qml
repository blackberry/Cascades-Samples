/* Copyright (c) 2013 BlackBerry Limited.
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
//! [0]
// Container that represents a Contact in the ListView
Container {

    topPadding: ui.du(4.4)
    leftPadding: ui.du(4.4)
    rightPadding: ui.du(4.4)

    Container {
        Container {
            layout: StackLayout {
                orientation: LayoutOrientation.LeftToRight
            }

            Container {
                layoutProperties: StackLayoutProperties {
                    spaceQuota: 1
                }
                // Contacts display name
                Label {
                    verticalAlignment: VerticalAlignment.Center

                    text: ListItemData.displayName
                    textStyle {
                        color: Color.White
                    }
                }
                // Contacts personal message
                Label {
                    verticalAlignment: VerticalAlignment.Center
                    leftMargin: ui.du(5.6)

                    multiline: true

                    text: ListItemData.personalMessage
                    textStyle {
                        color: Color.White
                        fontSize: FontSize.Small
                    }

                }
            }
            // The Contacts display image
            ImageView {
                verticalAlignment: VerticalAlignment.Center
                
                image: ListItemData.avatar
                scalingMethod: ScalingMethod.None
                preferredHeight: ui.du(11.1)
                preferredWidth: ui.du(11.1)
            }
        }
        Container {
            background: Color.White
            
            Divider {
            }
        }
    }
}
//! [0]