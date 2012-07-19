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
    }
    
    // A colored Container is used to highlight the item on selection.
    Container {
        id: highlight
        background: Color.Black
        opacity: stampContainer.ListItem.active ? 0.2 : 0.0
        
        layoutProperties: DockLayoutProperties {
            horizontalAlignment: HorizontalAlignment.Fill
            verticalAlignment: VerticalAlignment.Fill
        }        
    }
    
    // The item image is delivered to the item from a model (in this case models/stamps.xml).
    ImageView {
        
        // In list items it is always best to use imageSources of content type (relative path) 
        // rather then asset (prefixed asset:///). This since asset type images are loaded 
        // immediately and can affect the scroll performance of the list (see the models/stamps.xml 
        // to see what an imageSource of content type typically looks like.
        imageSource: ListItemData.thumbURL
        scalingMethod: ScalingMethod.AspectFit
        layoutProperties: DockLayoutProperties {
            verticalAlignment: VerticalAlignment.Center
            horizontalAlignment: HorizontalAlignment.Center
        }
    }
}
