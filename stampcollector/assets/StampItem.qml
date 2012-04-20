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

// Stamp Item used by the list to present a small thumbnail image of the stamps.

Container {
    id: stampContainer
    layout: DockLayout {
        leftPadding: 30
        rightPadding: leftPadding
        topPadding: 30
    }
    
    Container {
        preferredHeight: 250
        layout: DockLayout {
        }
        ImageView {
            imageSource: ListItemData.thumbURL
            scalingMethod: ScalingMethod.AspectFit
            layoutProperties: DockLayoutProperties {
                verticalAlignment: VerticalAlignment.Center
                horizontalAlignment: HorizontalAlignment.Center
            }
        }
    }

    // Both the activation and selection of an item has the same visual appearance.
    function setHighlight (highlighted) {
        if (highlighted) {
            stampContainer.opacity = 0.7;
        } else {
            stampContainer.opacity = 1.0;
        }
    }

    // Signal handler for list item activation.    
    ListItem.onActiveChanged: {
        setHighlight (ListItem.active);
    }
    
    // Signal handler for list item selection.
    ListItem.onSelectedChanged: {
        setHighlight (ListItem.selected);
    }
}
