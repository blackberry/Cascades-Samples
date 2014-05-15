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

Sheet {
    id: root

    //! [0]
    onCreationCompleted: {
        _app.queryFinished.connect(root.open)
        _app.closeQueryResults.connect(root.close)
    }
    //! [0]
    Page {
        titleBar: TitleBar {
            title: "Query Results"
            dismissAction: ActionItem {
                title: "Close"
                onTriggered: {
                    root.close();
                }
            }
        }
        Container {
            topPadding: ui.du(5.6)
            leftPadding: ui.du(5.6)
            rightPadding: ui.du(5.6)
            bottomPadding: ui.du(5.6)

            //! [1]
            ListView {
                horizontalAlignment: HorizontalAlignment.Fill
                dataModel: _app.model
                listItemComponents: ListItemComponent {
                    type: "item"
                    StandardListItem {
                        title: ListItemData.label
                        imageSource: ListItemData.imageSource
                    }
                }
                onTriggered: {
                    _app.invokeTarget(dataModel.data(indexPath).name)
                }
            }
            //! [1]
        }
    }
}
