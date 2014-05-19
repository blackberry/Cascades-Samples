/* Copyright (c) 2012, 2013, 2014 BlackBerry Limited.
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
import "Common"

// This recipe illustrates how the different preset easing curves, "stock curves",
// effect the timing of the animation (stock, in this case, is a collection
// and has nothing to do with the financial market). The curves predefined
// in Cascades are presented in a RadioButton list to the right. When a new curve is
// selected, an object will be animated using the animation curve so that you can see how
// the different curves behave.
RecipePage {
    id: stockCurveRecipe
    property bool breakEgg: true

    RecipeContainer {
        Container {
            layout: StackLayout {
                orientation: LayoutOrientation.LeftToRight
            }

            Container {
                topPadding: ui.du(2)
                verticalAlignment: VerticalAlignment.Fill

                layoutProperties: StackLayoutProperties {
                    spaceQuota: 2
                }

                // This is the Label presenting the current choice of easingCurve,
                // so we start by setting a Linear curve.
                Label {
                    id: stockCurveDescription
                    horizontalAlignment: HorizontalAlignment.Center
                    text: "Linear"
                    textStyle.base: SystemDefaults.TextStyles.SubtitleText
                }

                // This is the animated object that will animate using different easing curves.
                ImageView {
                    id: egg
                    imageSource: "asset:///images/stockcurve/egg.png"
                    horizontalAlignment: HorizontalAlignment.Center

                    animations: [
                        SequentialAnimation {
                            id: anim
                            animations: [
                                // Initial animations which resets the object.
                                FadeTransition {
                                    toOpacity: 0
                                },
                                TranslateTransition {
                                    toY: 0
                                },
                                FadeTransition {
                                    toOpacity: 1
                                },

                                // This is the animation that uses the selected easing curve.
                                TranslateTransition {
                                    id: easeAnim

                                    // This distance is overridden by a LayoutUpdateHandler below that measures the size of the
                                    // Page content container and sets the end distance depending on that.
                                    toY: 780
                                    duration: 1500
                                    delay: 500;

                                    // Initially a Linear easingCurve is used.
                                    easingCurve: StockCurve.Linear
                                }
                            ]
                            onStarted: {
                                egg.imageSource = "asset:///images/stockcurve/egg.png"
                            }
                            onEnded: {
                                // If the animation is set to break the egg in onEnded.
                                if (stockCurveRecipe.breakEgg) {
                                    egg.imageSource = "asset:///images/stockcurve/broken_egg.png"
                                }
                            }
                        }
                    ]
                }
            }

            // The easing curves are presented in three different lists so that one can see what an animation looks like with different curves.
            StockcurveList {
                id: outList
                title: "Out"
                onCreationCompleted: {
                    model.append({
                            "name": "Linear",
                            "curve": StockCurve.Linear
                        });
                    model.append({
                            "name": "SineOut",
                            "curve": StockCurve.SineOut
                        });
                    model.append({
                            "name": "CubicOut",
                            "curve": StockCurve.CubicOut
                        });
                    model.append({
                            "name": "QuarticOut",
                            "curve": StockCurve.QuarticOut
                        });
                    model.append({
                            "name": "QuinticOut",
                            "curve": StockCurve.QuinticOut
                        });
                    model.append({
                            "name": "CircularOut",
                            "curve": StockCurve.CircularOut
                        });
                    model.append({
                            "name": "BackOut",
                            "curve": StockCurve.BackOut
                        });
                    model.append({
                            "name": "ElasticOut",
                            "curve": StockCurve.ElasticOut
                        });
                    model.append({
                            "name": "DoubleElasticOut",
                            "curve": StockCurve.DoubleElasticOut
                        });
                    model.append({
                            "name": "BounceOut",
                            "curve": StockCurve.BounceOut
                        });
                    model.append({
                            "name": "DoubleBounceOut",
                            "curve": StockCurve.DoubleBounceOut
                        });
                }

                onTriggeredItemData: {
                    playAnimWithCurve(itemData.curve, itemData.name);
                    inList.clearListSelection();
                    inOutList.clearListSelection();
                }
            }

            StockcurveList {
                id: inList
                title: "In"

                onCreationCompleted: {
                    model.append({
                            "name": "Linear",
                            "curve": StockCurve.Linear
                        });
                    model.append({
                            "name": "SineIn",
                            "curve": StockCurve.SineIn
                        });
                    model.append({
                            "name": "CubicIn",
                            "curve": StockCurve.CubicIn
                        });
                    model.append({
                            "name": "QuarticIn",
                            "curve": StockCurve.QuarticIn
                        });
                    model.append({
                            "name": "QuinticIn",
                            "curve": StockCurve.QuinticIn
                        });
                    model.append({
                            "name": "CircularIn",
                            "curve": StockCurve.CircularIn
                        });
                    model.append({
                            "name": "BackIn",
                            "curve": StockCurve.BackIn
                        });
                }

                onTriggeredItemData: {
                    playAnimWithCurve(itemData.curve, itemData.name);
                    outList.clearListSelection();
                    inOutList.clearListSelection();
                }
            }

            StockcurveList {
                id: inOutList
                title: "InOut"

                onCreationCompleted: {
                    model.append({
                            "name": "Linear",
                            "curve": StockCurve.Linear
                        });
                    model.append({
                            "name": "SineInOut",
                            "curve": StockCurve.SineInOut
                        });
                    model.append({
                            "name": "CubicInOut",
                            "curve": StockCurve.CubicInOut
                        });
                    model.append({
                            "name": "QuarticInOut",
                            "curve": StockCurve.QuarticInOut
                        });
                    model.append({
                            "name": "QuinticInOut",
                            "curve": StockCurve.QuinticInOut
                        });
                    model.append({
                            "name": "CircularInOut",
                            "curve": StockCurve.CircularInOut
                        });
                    model.append({
                            "name": "BackInOut",
                            "curve": StockCurve.BackInOut
                        });
                }

                onTriggeredItemData: {
                    playAnimWithCurve(itemData.curve, itemData.name);
                    inList.clearListSelection();
                    outList.clearListSelection();
                }
            }
        }

        // A layout update handler is used to determine the size of the screen and set the
        // distance the egg should travel.
        attachedObjects: LayoutUpdateHandler {
            id: screeLayoutHandler

            onLayoutFrameChanged: {
                easeAnim.toY = layoutFrame.height - ui.px(200)
                anim.play();
            }
        }
    }

    function playAnimWithCurve(curve, name) {

        // Set the StockCurve name.
        stockCurveDescription.text = name;

        // If the easing curve selected results in an abrupt stop, we break the egg when the animation is over.
        if (name.indexOf("InOut") == -1 && (name.indexOf("In") >= 0 || name.indexOf("Elastic") >= 0 || name.indexOf("Bounce") >= 0 || name.indexOf("Linear") >= 0)) {
            breakEgg = true;
        } else {
            breakEgg = false;
        }

        // Get the selected easing curve, update the animation, and play it
        // (first stop it to avoid conflicts if the animation is already running).
        easeAnim.easingCurve = curve;
        anim.stop();
        anim.play();
    }
}
