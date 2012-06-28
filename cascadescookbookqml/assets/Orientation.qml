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
import "Common"

// When creating an UI it is often the case that it has to be able to be shown in more
// then one orientations. Some modification to the UI is often necessary when for example
// switching between Landscape and Portrait mode. In this recipe it is shown how to set
// what orientation an app or a page supports and how to listen to orientation changes.
RecipePage {
    RecipeContainer {
        onCreationCompleted: {
            // When the recipe is created, we want to let Cascades know that we support all the orientations.
            OrientationSupport.supportedDisplayOrientation = SupportedDisplayOrientation.All;
        }
        ImageView {
            id: imageId
            imageSource: "asset:///images/orientation/portrait.png"
            layoutProperties: DockLayoutProperties {
                horizontalAlignment: HorizontalAlignment.Center
                verticalAlignment: VerticalAlignment.Center
            }
        }
    }
    attachedObjects: [
        // The orientation handler takes care of orientation change events. All we really want to do is to
        // switch image when we get a signal to these functions.
        OrientationHandler {
            id: handler
            onUiOrientationChanging: {
                console.log ("Orientation Changing to: " + uiOrientation)
            }
            onUiOrientationChanged: {
                if (uiOrientation == UiOrientation.Landscape) {
                    imageId.imageSource = "asset:///images/orientation/landscape.png"
                } else {
                    imageId.imageSource = "asset:///images/orientation/portrait.png"
                }
            }
        }
    ]
}
