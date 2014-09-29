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

import bb.cascades 1.3

Container {
    property variant target1
    property variant target2
    property variant target3
    property variant target4
    property variant target5
    property int hideTouchArea: 100

    function playShowListViewAnimation() {
        showListViewAnimation.play()
    }

    function playHideListViewAnimation() {
        hideListViewAnimation.play()
    }

    animations: [
        ParallelAnimation {
            id: showListViewAnimation

            SequentialAnimation {
                target: target1
                FadeTransition {
                    duration: 500
                    fromOpacity: 1.0
                    toOpacity: 0
                    easingCurve: StockCurve.Linear
                }
            }
            SequentialAnimation {
                target: target2
                FadeTransition {
                    duration: 500
                    fromOpacity: 1.0
                    toOpacity: 0
                    easingCurve: StockCurve.Linear
                }
            }
            SequentialAnimation {
                target: target3
                FadeTransition {
                    duration: 500
                    fromOpacity: 1.0
                    toOpacity: 0
                    easingCurve: StockCurve.Linear
                }
            }
            SequentialAnimation {
                target: target4
                TranslateTransition {
                    duration: 500
                    fromY: 0
                    toY: -1050
                    easingCurve: StockCurve.Linear
                }
            }
            SequentialAnimation {
                target: target5
                TranslateTransition {
                    duration: 500
                    fromY: 1280
                    toY: 25
                    easingCurve: StockCurve.Linear
                }
            }
        },
        ParallelAnimation {
            id: hideListViewAnimation

            SequentialAnimation {
                target: target1
                FadeTransition {
                    duration: 500
                    fromOpacity: 0
                    toOpacity: 1.0
                    easingCurve: StockCurve.Linear
                }
            }
            SequentialAnimation {
                target: target2
                FadeTransition {
                    duration: 500
                    fromOpacity: 0
                    toOpacity: 1.0
                    easingCurve: StockCurve.Linear
                }
            }
            SequentialAnimation {
                target: target3
                FadeTransition {
                    duration: 500
                    fromOpacity: 0
                    toOpacity: 1.0
                    easingCurve: StockCurve.Linear
                }
            }
            SequentialAnimation {
                target: target4
                TranslateTransition {
                    duration: 500
                    fromY: -1000
                    toY: 0
                    easingCurve: StockCurve.Linear
                }
            }
            SequentialAnimation {
                target: target5
                TranslateTransition {
                    duration: 500
                    fromY: 0
                    toY: 1280
                    easingCurve: StockCurve.Linear
                }
            }
        }
    ]
}