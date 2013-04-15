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

Container {
    id: root
    property alias x: position.positionX
    property alias y: position.positionY
    property double lat
    property double lon
    property alias anim: anim

    clipContentToBounds: false

    layoutProperties: AbsoluteLayoutProperties {
        id: position
        onPositionXChanged: {
            bubble.visible = false
        }
        onPositionYChanged: {
            bubble.visible = false
        }
    }

    layout: AbsoluteLayout {
    }

    Bubble {
        id: bubble

        visible: false

        layoutProperties: AbsoluteLayoutProperties {
            positionX: 30
            positionY: -140
        }
    }

    ImageView {
        animations: [
            ScaleTransition {
                id: anim
                fromX: 0
                toX: 1
                fromY: 0
                toY: 1
                duration: 500
                easingCurve: StockCurve.BounceOut
            }
        ]
        imageSource: "asset:///images/on_map_pin.png"
        focusPolicy: FocusPolicy.Touch
        onFocusedChanged: {
            if (focused) {
                anim.play();
                bubble.visible = true
            } else {
                bubble.visible = false
            }
        }
    }

    overlapTouchPolicy: OverlapTouchPolicy.Allow
}
