/* Copyright (c) 2012, 2013  BlackBerry Limited.
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

Page {
    Container {
        leftPadding: ui.du(2.2)
        rightPadding: ui.du(2.2)

        background: Color.Black

        //! [0]
        Label {
            horizontalAlignment: HorizontalAlignment.Center

            text: qsTr("Number of items: %1").arg(_model.itemCount)

            textStyle {
                base: SystemDefaults.TextStyles.BigText
                color: Color.White
            }
        }
        //! [0]

        //! [1]
        ListView {
            dataModel: _model
        //! [1]

            listItemComponents: [
                ListItemComponent {
                    type: "item"
                    CustomItem {
                        preferredWidth: ui.du(85.3)

                        text: ListItemData
                        selected: ListItem.selected
                    }
                }
            ]

            //! [2]
            onTriggered: {
                clearSelection()
                select(indexPath)

                if (indexPath[0] == (dataModel.childCount([]) - 1)) { // Last item selected
                    _model.addItemsAt(indexPath);
                }
            }
            //! [2]

            function itemType(data, indexPath)
            {
                return (indexPath.length == 1 ? "item" : "")
            }
        }
    }
}
