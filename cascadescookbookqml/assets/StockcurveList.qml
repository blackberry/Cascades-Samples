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

Container {
    property alias model: stockCurveList.dataModel
    property alias title: titleLabel.text
    signal triggeredItemData(variant itemData)
    background: Color.create("#fefefe")
    preferredWidth: ui.du(15)
    topPadding: ui.du(2)

    Label {
        id: titleLabel
        textStyle.base: SystemDefaults.TextStyles.SubtitleText
        horizontalAlignment: HorizontalAlignment.Center
        textStyle.color: Color.create("#272727")
    }

    ListView {
        id: stockCurveList
        dataModel: ArrayDataModel {
        }

        listItemComponents: [
            ListItemComponent {
                type: ""
                CustomListItem {
                    id: item
                    dividerVisible: false
                    highlightAppearance: HighlightAppearance.None
                    
                    Container {
                        background: item.ListItem.selected ? Color.create("#9975b5d3") : Color.Transparent
                        horizontalAlignment: HorizontalAlignment.Fill
                        verticalAlignment: VerticalAlignment.Fill
                        layout: DockLayout {
                        }

                        ImageView {
                            horizontalAlignment: HorizontalAlignment.Center
                            verticalAlignment: VerticalAlignment.Center
                            imageSource: "asset:///images/stockcurve/icon_" + ListItemData.name + ".png"
                        }
                    }
                }
            }
        ]

        onTriggered: {
            var itemData = model.data(indexPath);
            triggeredItemData(itemData);
            clearSelection();
            select(indexPath); 
        }
    }
    
    function clearListSelection() {
        stockCurveList.clearSelection();
    }
}
            
            