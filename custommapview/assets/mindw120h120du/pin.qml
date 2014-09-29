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
import bb.cascades 1.0

//! [0]
Container {
    // File path of the pin image
    property string pinImageSource: "asset:///images/on_map_pin.png"
    // pointerOffsetX, pointerOffsetY is the position of the pixel in pin image that should point to the location. Change these to match your pin image.
    property int pointerOffsetX: 20
    property int pointerOffsetY: 58
    /////////////////////////////////////////////////////////
    id: root
    property int x: 0
    property int y: 0
    property double lat
    property double lon
    property alias animDrop: animDrop
    clipContentToBounds: false
    overlapTouchPolicy: OverlapTouchPolicy.Allow
    layoutProperties: AbsoluteLayoutProperties {
        id: position
        positionX: x - pointerOffsetX
        positionY: y - pointerOffsetY
    }
    ImageView {
        id: pinImage
        scaleX: .8
        scaleY: .8
        imageSource: pinImageSource
        focusPolicy: FocusPolicy.Touch
        overlapTouchPolicy: OverlapTouchPolicy.Allow
        onFocusedChanged: {
            if (focused) {
                animFocus.play();
                root.parent.showBubble(root);
            }
            if (! focused) {
                animUnfocus.play();
            }
        }
        animations: [
            ScaleTransition {
                id: animFocus
                fromX: .8
                toX: 1
                fromY: .8
                toY: 1
                duration: 300
                easingCurve: StockCurve.BounceOut
            },
            ScaleTransition {
                id: animUnfocus
                fromX: 1
                toX: .8
                fromY: 1
                toY: .8
                duration: 300
                easingCurve: StockCurve.BounceOut
            }
        ]
    }
    animations: [
        TranslateTransition {
            id: animDrop
            fromY: - position.positionY
            toY: 0
            duration: 600
            easingCurve: StockCurve.BounceOut
        }
    ]
}
//! [0]
