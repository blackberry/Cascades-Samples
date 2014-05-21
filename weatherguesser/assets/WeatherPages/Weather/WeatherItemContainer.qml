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

// The common Container used for the two weather item types in the list. Contain
// animations used to present the items when they are added to the list after
// new data has been added to the ListViews model.
Container {
    // Animation properties.
    property bool initialized: ListItem.initialized
    property bool moving: false
    property int indexOffset
    
    onInitializedChanged: {
        if (! moving) {
            // If the item is not scrolling initialization mean that the
            // item has been added and we want to show it using an animation.
            initAnim.play()
        }
    }
    
    // Animations that are run when adding new items.
    animations: [
        // First set the opacity to 0 then scale up and fade in depending on position.
        SequentialAnimation {
            id: initAnim
            FadeTransition {
                toOpacity: 0
                duration: 0
            }
            ParallelAnimation {
                // Index dependent delay, if higher index then 6 (at the bottom of the list)
                // no delay.
                delay: (indexOffset+1 < 6) ? indexOffset * 75 : 0
                ScaleTransition {
                    fromX: 0.9
                    fromY: 0.9
                    toX: 1
                    toY: 1
                    duration: 150
                    easingCurve: StockCurve.QuarticOut
                }
                FadeTransition {
                    fromOpacity: 0
                    toOpacity: 1
                    duration: 150
                    easingCurve: StockCurve.QuadraticInOut
                }
            }
        }
    ]
    
    layout: StackLayout {
        orientation: LayoutOrientation.LeftToRight
    }
}
