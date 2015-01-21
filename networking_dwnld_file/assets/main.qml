/*
 * Copyright (c) 2011-2014 BlackBerry Limited.
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 * http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, 
 * software distributed under the License is distributed on an 
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, 
 * either express or implied.
 * See the License for the specific language governing permissions 
 * and limitations under the License.
 */

import bb.cascades 1.2

Page {
    titleBar: TitleBar {
        title: "Qt Networking Sample App"
    }
    Container {        
        Container {
            horizontalAlignment: HorizontalAlignment.Center
            verticalAlignment: VerticalAlignment.Top
            topPadding: 30
            bottomMargin: 30
            layout: StackLayout {
                orientation: LayoutOrientation.LeftToRight
            }
            Label {
                id: lblOnlineStatus
                objectName: "netConnection"
                text: "Online status:"
                textStyle.textAlign: TextAlign.Right
                verticalAlignment: VerticalAlignment.Center
                textFit.minFontSizeValue: 6.0
                textFit.maxFontSizeValue: 7.0
            }
            ImageView {
                objectName: "netConnDot"
                imageSource: "asset:///images/unknown.png"
                scalingMethod: ScalingMethod.Fill
                horizontalAlignment: HorizontalAlignment.Right
                verticalAlignment: VerticalAlignment.Center
                accessibility.labelledBy: [ lblOnlineStatus ]
            }
            Label {
                text: "   Connected by:"
                id: lblConnectionType
                objectName: "netConnType"
                textStyle.textAlign: TextAlign.Right
                verticalAlignment: VerticalAlignment.Center
                textFit.minFontSizeValue: 6.0
                textFit.maxFontSizeValue: 7.0
            }
            ImageView {
                objectName: "netConnTypeIcon"
                imageSource: "asset:///images/unknown.png"
                scalingMethod: ScalingMethod.Fill
                horizontalAlignment: HorizontalAlignment.Right
                verticalAlignment: VerticalAlignment.Center
                accessibility.labelledBy: [ lblConnectionType ]
            }
        }
        Container {
            layout: StackLayout {
                orientation: LayoutOrientation.LeftToRight
            }
            horizontalAlignment: HorizontalAlignment.Right
            rightPadding: 30.0
            Label {
                text: "View Raw Headers"
                objectName: "lblViewRawHdr"
                textStyle.fontSize: FontSize.XSmall
                verticalAlignment: VerticalAlignment.Center
                horizontalAlignment: HorizontalAlignment.Right
                enabled: false
            }
            ToggleButton {
                id: rawHeaderTgl
                objectName: "tbnVewRawHdr"
                scaleX: 0.7
                scaleY: 0.7
                checked: false
                enabled: false

                onCheckedChanged: {
                    _RawHeaderView.visible = rawHeaderTgl.checked
                }
            }
        }
        Container {
            ListView {
                objectName: "list"
                accessibility.name: "Contact list"
                // A simple data model is loaded with just a header
                // and some dummy data which will be replaced when
                // we load the real data model from our C++ code
                dataModel: XmlDataModel {
                    source: "asset:///contacts_empty.xml"

                }
                listItemComponents: [
                    // The header list item displays a title along
                    // with a counter that displays the number of
                    // contacts in the list
                    ListItemComponent {
                        type: "header"
                        Header {
                            title: ListItemData.title
                            subtitle: "Number of contacts: " + 
                                (ListItem.initialized ?
                                    ListItem.view.dataModel.childCount(ListItem.indexPath) : 0)
                        }
                    },
                    // This list item displays the names of the
                    // contacts
                    ListItemComponent {
                        type: "contacts"
                        StandardListItem {
                            title: ListItemData.title
                        }
                    }
                ]
            }
        }
        RawHeaderView {
            horizontalAlignment: HorizontalAlignment.Fill
            verticalAlignment: VerticalAlignment.Fill

            visible: _RawHeaderView.visible
        }
    }
}
