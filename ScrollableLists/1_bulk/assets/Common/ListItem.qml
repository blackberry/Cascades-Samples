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

// Item component for the list.
Container {
    layout: DockLayout {
    }
    
    Container {
        layout: DockLayout {
        }
        layoutProperties: DockLayoutProperties {
            horizontalAlignment: HorizontalAlignment.Center
        }
        
        // Item backgorund image.
        ImageView {
            imageSource: "asset:///images/white_photo.png"
            preferredWidth: 768
            preferredHeight: 173
        }
                
        Container {
            id: highlightContainer
            background: Color.create("#75b5d3")
            opacity: 0.0
            preferredWidth: 768
            preferredHeight: 168
            
            layoutProperties: DockLayoutProperties {
                horizontalAlignment: HorizontalAlignment.Center                
            }
        }
        
        Container {
            layout: DockLayout  {
            }
            
            layoutProperties: DockLayoutProperties {
                horizontalAlignment: HorizontalAlignment.Center
                verticalAlignment: VerticalAlignment.Center
            }
            
            Label {
                // The title is bound to the data in models/dataModel.xml title attribute.
                text: ListItemData.title
                textStyle {
                    base: SystemDefaults.TextStyles.TitleText
                    color: Color.Black
                }
                
                layoutProperties: DockLayoutProperties {
                    verticalAlignment: VerticalAlignment.Center
                    horizontalAlignment: HorizontalAlignment.Center
                }
            }
        }
    }
    
    function setHighlight(highlighted) {
        if(highlighted) {
            highlightContainer.opacity = 0.9;
        } else {
            highlightContainer.opacity = 0.0;
        }    
    }

    ListItem.onSelectedChanged : {
        setHighlight(ListItem.selected);
    }
}