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
import bb.cascades 1.2

// This is another Stamp Container used by the list to present a small thumbnail image of the stamps.
Container {
    id: stampContainer
    
    layout: DockLayout {
    }
    
    // A colored Container is used to highlight the item on selection.
    Container {
        id: highlight
        background: Color.Black
        opacity: 0.0
        horizontalAlignment: HorizontalAlignment.Fill
        verticalAlignment: VerticalAlignment.Fill 
        accessibilityMode: A11yMode.Collapsed       
    }
    
    // The image of the item is delivered from a model (in this case; an XML model from models/stamps.xml).
    ImageView {
        
        // When using ListItems its best to use imageSources of content-type (relative path) 
        // rather then asset-type (prefixed asset:///). This is because asset-type images are loaded 
        // immediately and can affect the scroll performance of the list (see the models/stamps.xml 
        // to see what an imageSource of content type typically looks like).
        imageSource: ListItemData.thumbURL
        scalingMethod: ScalingMethod.AspectFit
        verticalAlignment: VerticalAlignment.Center
        horizontalAlignment: HorizontalAlignment.Center
        accessibilityMode: A11yMode.Collapsed    
    }
    
    accessibility:CustomA11yObject  {
        role: A11yRole.ListItem
        name: "Stamp Image"
        description: "Select item for more information."
        
        // When in A11y mode we need to seed a signal to the listview to trigg the navigation.
        ComponentA11ySpecialization {
            onActivationRequested: {
                if (event.type == A11yComponentActivationType.Release) {
                    stampContainer.ListItem.view.triggered(stampContainer.ListItem.indexPath)
                }
            }
        }
    }
    
    // Both the activation and selection of an item has the same visual appearance, we alter the opacity of the item.
    function setHighlight (highlighted) {
        if (highlighted) {
            highlight.opacity = 0.2;
        } else {
            highlight.opacity = 0.0;
        }
    }

    // Signal handler for ListItem activation
    ListItem.onActivationChanged: {
        setHighlight (ListItem.active);
    }

    // Signal handler for ListItem selection
    ListItem.onSelectionChanged: {
        setHighlight (ListItem.selected);
    }
}
