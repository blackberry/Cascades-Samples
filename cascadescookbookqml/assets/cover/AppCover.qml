/* Copyright (c) 2013, 2014 BlackBerry Limited.
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

// This is the file used for the SceneCover shown when the application
// is running in minimized mode on the running apps page.
Container {
    background: coverBackground.imagePaint

    layout: DockLayout {
    }

    Container {
        horizontalAlignment: HorizontalAlignment.Center
        verticalAlignment: VerticalAlignment.Bottom

        // In order for the image to look good on a 720x720 device a bit of Padding is added to force the image to be scaled on 720x720
        topPadding: ui.du(3)

        ImageView {
            imageSource: "asset:///images/active_frames_pepper.png"
            scalingMethod: ScalingMethod.AspectFit
        }
    }

	AppCoverHeader {
        title: "QML"
        backgroundOpacity: 0.5

        // A slightly green header is put behind the text, 
        // the green color is chosen so that it is visible on the black background
        headerColor: "#36412d"
     }

     // A black book nine-sliced image.
     attachedObjects: ImagePaintDefinition {
         id: coverBackground
         imageSource: "asset:///images/application-cover.amd"
     }
}
