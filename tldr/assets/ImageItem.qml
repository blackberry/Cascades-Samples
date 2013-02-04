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

/*
 * This component is a image component used by the feedlist
 */
 
import bb.cascades 1.0
import com.netimage 1.0

Container {
    id: imageItem
    
    leftPadding: 20
    topPadding: leftPadding
    rightPadding: leftPadding
    bottomPadding: leftPadding
    verticalAlignment: VerticalAlignment.Fill
    horizontalAlignment: HorizontalAlignment.Fill
    
    Label {
        text: ListItemData.title
        textStyle.base: SystemDefaults.TextStyles.TitleText
        multiline: true
    }
    
    ImageView {
        id: itemImage
        horizontalAlignment: HorizontalAlignment.Fill
        verticalAlignment: VerticalAlignment.Fill
        minHeight: 300
        maxHeight: 768
        scalingMethod: ScalingMethod.AspectFit
        
        // This is where we get the image as soon as it's downloaded from the internet via the netimagetracker
        image: tracker.image
        
        attachedObjects: [
            // The NetImageTracker is a custom ImageTracker with the ability to 
            // load images from the internet (see netimagetracker.h/.cpp). 
            NetImageTracker {
                id: tracker
                
                // The image source is set 
                source: ListItemData.imageSource
                
                // In this sample the same manager is used for all list item trackers, 
                // it handles the networked requests and contain the image cache (see netimagetracker.cpp/.h).
                // Note that if managers with many different cacheIds are used you have to be 
                // careful not to use up to much disk space for the different caches.
                manager: imageItem.ListItem.view.listImageManager
            }
        ]
    }
    
    Label {
        text: ListItemData.description
        textStyle.base: SystemDefaults.TextStyles.BodyText
        multiline: imageItem.ListItem.selected
        preferredHeight: imageItem.ListItem.selected ? 0 : 400.0
    }
    
    Label {
        text: ListItemData["dc:creator"]
        textStyle.base: SystemDefaults.TextStyles.SmallText
        horizontalAlignment: HorizontalAlignment.Right
    }
    
    Divider {
    }
}
