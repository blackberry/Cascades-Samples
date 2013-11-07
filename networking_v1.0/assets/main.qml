/* Copyright (c) 2013 BlackBerry Limited
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

Page {
    actions: [
        ActionItem {
            objectName: "refresh"
            title: "Refresh"
            ActionBar.placement: ActionBarPlacement.OnBar
            enabled: true
        }
    ]
    Container {
        // This Container holds the label and image pairs that report
        // on network status and connectivity.
        Container {
            horizontalAlignment: HorizontalAlignment.Center
            verticalAlignment: VerticalAlignment.Top
            topPadding: 30
            bottomMargin: 30
            layout: StackLayout {
                orientation: LayoutOrientation.LeftToRight
            }
            Label {
                objectName: "netConnection"
                text: "Network status:"
                textStyle.textAlign: TextAlign.Right
                verticalAlignment: VerticalAlignment.Center
            }
            ImageView {
                objectName: "netConnDot"
                imageSource: "asset:///images/unknown.gif"
                scalingMethod: ScalingMethod.Fill
                horizontalAlignment: HorizontalAlignment.Right
                verticalAlignment: VerticalAlignment.Center
                accessibility.description: "connection is unknown"
                accessibility.name: "Connection type"
            }
            Label {
                objectName: "netConnType"
                text: "   Connected by:"
                textStyle.textAlign: TextAlign.Right
                verticalAlignment: VerticalAlignment.Center
            }
            ImageView {
                objectName: "netConnTypeIcon"
                imageSource: "asset:///images/unknown.gif"
                scalingMethod: ScalingMethod.Fill
                horizontalAlignment: HorizontalAlignment.Right
                verticalAlignment: VerticalAlignment.Center
                accessibility.description: "connection is unknown"
                accessibility.name: "Connection type"
            }
        }

        // This Container holds the ListView control that displays the
        // contact list.
        Container {
            // A list that has two list item components, one for a header
            // and one for contact names. The list has an object name so
            // that we can set the data model from C++ code.
            ListView {
                objectName: "list"
                accessibility.name: "Contact list"
                // A simple data model is loaded with just a header
                // and some dummy data.
                // This will be replaced when we load the real one
                // from C++ code.
                dataModel: XmlDataModel {
                    source: "asset:///model.xml"

                }
                listItemComponents: [
                    // The header list item displays a title along with a counter
                    // that displays the number of children.
                    ListItemComponent {
                        type: "header"
                        Header {
                            title: ListItemData.title
                            subtitle: (ListItem.initialized ? ListItem.view.dataModel.childCount(ListItem.indexPath) : 0)
                        }
                    },
                    // The contact list item displays the name of the contact.
                    ListItemComponent {
                        type: "contacts"
                        StandardListItem {
                            title: ListItemData.title
                        }
                    }
                ]                
            }
        }
    }
}
