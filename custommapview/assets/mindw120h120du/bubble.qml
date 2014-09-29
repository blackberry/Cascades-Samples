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

Container {
    // File path of the bubble image
    property string bubbleImageSource: "asset:///images/bubble.png"
    // pointerOffsetX, pointerOffsetY is the position of the pixel in bubble image that should point to the location. Change these to match your pin image.
    property int pointerOffsetX: 135
    property int pointerOffsetY: 180
    // Width and Height of the pin image. Used to further offset the bubble position so that it does not overlap the pin image.
    property int pinWidth: 60
    property int pinHeight: 60
    // Width of the content area
    property int contentWidth: 439
    // Height of the content area
    property int contentHeight: 125
    // Padding of the content area i.e. the border size of the bubble.
    property int padding: 10
    ////////////////////////////////////////////////////////////////////////
    property int x: 0
    property int y: 0
    property int actualX: x - pointerOffsetX
    property int actualY: y - pointerOffsetY - pinHeight
    property double lat
    property double lon
    overlapTouchPolicy: OverlapTouchPolicy.Allow
    layoutProperties: AbsoluteLayoutProperties {
        id: position
        positionX: actualX
        positionY: actualY
    }
    layout: DockLayout {
    }
    ImageView {
        imageSource: bubbleImageSource
        overlapTouchPolicy: OverlapTouchPolicy.Allow
    }
    Container {
        topPadding: padding
        leftPadding: padding
        rightPadding: padding
        preferredHeight: contentHeight
        preferredWidth: contentWidth
        maxHeight: contentHeight
        maxWidth: contentWidth
        overlapTouchPolicy: OverlapTouchPolicy.Allow
        layout: DockLayout {
        }
        Label {
            horizontalAlignment: HorizontalAlignment.Left
            verticalAlignment: VerticalAlignment.Center
            overlapTouchPolicy: OverlapTouchPolicy.Allow
            text: qsTr("Hey, Lets go!")
            textStyle.color: Color.White
            multiline: true
        }
        Button {
            preferredWidth: 50
            horizontalAlignment: HorizontalAlignment.Right
            verticalAlignment: VerticalAlignment.Center
            overlapTouchPolicy: OverlapTouchPolicy.Allow
            imageSource: "asset:///images/url.png"
        }
    }
    animations: [
        ParallelAnimation {
            id: anim
            ScaleTransition {
                fromX: 0
                toX: 1
                fromY: 0
                toY: 1
                duration: 300
                easingCurve: StockCurve.BounceOut
            }
            FadeTransition {
                fromOpacity: 0
                toOpacity: 1
                duration: 200
            }
        }
    ]
    function play() {
        anim.play();
    }
}
