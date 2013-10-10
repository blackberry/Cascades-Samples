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

import bb.cascades 1.2
import com.netimage 1.0

Container {
    id: imageItem

    verticalAlignment: VerticalAlignment.Fill
    horizontalAlignment: HorizontalAlignment.Fill
    layout: DockLayout {
    }

    ImageView {
        id: itemImage
        horizontalAlignment: HorizontalAlignment.Fill
        verticalAlignment: VerticalAlignment.Fill
        scalingMethod: ScalingMethod.AspectFill
        //touchPropagationMode: TouchPropagationMode.PassThrough

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
    ImageView {
        imageSource: "asset:///images/gradient.png"
        horizontalAlignment: HorizontalAlignment.Fill
        verticalAlignment: VerticalAlignment.Fill
        touchPropagationMode: TouchPropagationMode.PassThrough
    }
    Container {
        leftPadding: 20
        topPadding: leftPadding
        rightPadding: leftPadding
        bottomPadding: leftPadding
        verticalAlignment: VerticalAlignment.Fill
        horizontalAlignment: HorizontalAlignment.Fill
        layout: StackLayout {
            orientation: LayoutOrientation.BottomToTop
        }

        DSLabel {
            id: writerLabel
            text: ListItemData["dc:creator"]
            visible: (text != "")
            textStyleBase: SystemDefaults.TextStyles.SmallText
            maxLineCount: 1
            horizontalAlignment: HorizontalAlignment.Right
            accessibilityMode: A11yMode.Collapsed
        }

        DSLabel {
            id: titleLabel
            text: ListItemData.title
            textStyleBase: SystemDefaults.TextStyles.TitleText
            maxLineCount: 3
            accessibilityMode: A11yMode.Collapsed
        }
        accessibility: CustomA11yObject {
            role: A11yRole.ListItem
            name: titleLabel.text + ". Written by: " + writerLabel.text
            ComponentA11ySpecialization {
                onActivationRequested: {
                    if (event.type == A11yComponentActivationType.Release) {
                        imageItem.ListItem.view.triggered(imageItem.ListItem.indexPath)
                    }
                }
            }
        }
    }
}
