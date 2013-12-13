/* Copyright (c) 2012 BlackBerry Limited.
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
import bb.cascades 1.2
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
                topPadding: 20
                verticalAlignment: VerticalAlignment.Fill
                
                layoutProperties: StackLayoutProperties {
                    spaceQuota: 35
                }

                // This is the Label presenting the current choice of easingCurve,
				// so we start by setting a Linear curve.
                Label {
                    id: stockCurveDescription
                    horizontalAlignment: HorizontalAlignment.Center
                    text: "Linear"
                    textStyle.base: SystemDefaults.TextStyles.SmallText
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
                                    toY: 325
                                    duration: 750
                                    delay: 500;

                                    // Initially the Linear is set as easingCurve.
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
                        }// SequentialAnimations
                    ]// animations
                }// ImageView
            }// Container
            
            // The selection of easing curve is done in a panel to the right. The
            // panel has three radio groups, one for each ease type (out/in/inout).
            ScrollView {
                verticalAlignment: VerticalAlignment.Fill
                
				scrollViewProperties {
				      scrollMode: ScrollMode.Vertical
				}
				
                layoutProperties: StackLayoutProperties {
                    spaceQuota: 65
                }
                                
                Container {
                    layout: StackLayout {
                        orientation: LayoutOrientation.LeftToRight
                    }
                
                    // RadioGroup for ease-out animation stock curves (see StockCurveRadioGroup.qml)
                    StockCurveRadioGroup {
                        id: outRadioGroup
                        title: "Out"
                
                        // The easing curves options for ease out
                        stockCurves: [
                            StockCurve.Linear,
                            StockCurve.SineOut,
                            StockCurve.CubicOut,
                            StockCurve.QuarticOut,
                            StockCurve.QuinticOut,
                            StockCurve.CircularOut,
                            StockCurve.BackOut,
                            StockCurve.ElasticOut,
                            StockCurve.DoubleElasticOut,
                            StockCurve.BounceOut,
                            StockCurve.DoubleBounceOut
                        ]
                
                        // The easing curve names that will be presented in the stockCurveDescription Label
                        stockCurveNames: [
                            "Linear",
                            "SineOut",
                            "CubicOut",
                            "QuarticOut",
                            "QuinticOut",
                            "CircularOut",
                            "BackOut",
                            "ElasticOut",
                            "DoubleElasticOut",
                            "BounceOut",
                            "DoubleBounceOut"
                        ]
                        onSelectedCurveChanged: {
                            // Reset the other groups (de-select option).
                            inRadioGroup.resetSelectedOption ();
                            inOutRadioGroup.resetSelectedOption ();
                
                            // Set up the new easingCurve and run the animation.
                            playAnimWithCurve (curve, name);
                        }
                    }
                
                    // RadioGroup for ease-in animation stock curves (StockCurveRadioGroupSmall.qml)
                    StockCurveRadioGroupSmall {
                        id: inRadioGroup
                        title: "In"
                
                        // The easing curves options for ease in
                        stockCurves: [
                            StockCurve.Linear,
                            StockCurve.SineIn,
                            StockCurve.CubicIn,
                            StockCurve.QuarticIn,
                            StockCurve.QuinticIn,
                            StockCurve.CircularIn,
                            StockCurve.BackIn
                        ]
                
                        // The easing curve names that will be presented in the stockCurveDescription Label
                        stockCurveNames: [
                            "Linear",
                            "SineIn",
                            "CubicIn",
                            "QuarticIn",
                            "QuinticIn",
                            "CircularIn",
                            "BackIn"
                        ]
                        onSelectedCurveChanged: {
                            // Reset the other groups (de-select option).
                            outRadioGroup.resetSelectedOption ();
                            inOutRadioGroup.resetSelectedOption ();
                
                            // Set up new easingCurve and run the animation.
                            playAnimWithCurve (curve, name);
                        }
                    }
                
                    // Radio group for ease-inout animation stock curves.
                    StockCurveRadioGroupSmall {
                        id: inOutRadioGroup
                        title: "InOut"
                
                        // The easing curves options for ease inout
                        stockCurves: [
                            StockCurve.Linear,
                            StockCurve.SineInOut,
                            StockCurve.CubicInOut,
                            StockCurve.QuarticInOut,
                            StockCurve.QuinticInOut,
                            StockCurve.CircularInOut,
                            StockCurve.BackInOut
                        ]
                
                        // The easing curve names that will be presented in the stockCurveDescription Label
                        stockCurveNames: [
                            "Linear",
                            "SineInOut",
                            "CubicInOut",
                            "QuarticInOut",
                            "QuinticInOut",
                            "CircularInOut",
                            "BackInOut"
                        ]
                        onSelectedCurveChanged: {
                            // Reset the other groups (de-select option).
                            outRadioGroup.resetSelectedOption ();
                            inRadioGroup.resetSelectedOption ();
                
                            // Set up new easingCurve and run the animation.
                            playAnimWithCurve (curve, name);
                        }
                    }// StockCurveRadioGroupSmall
                }// Stock curve Container
            }// ScrollView
        }// Top Container
    }// RecipeContainer

    // Play the first animation immediately when the recipe is created and select the
    // corresponding option.
    onCreationCompleted: {
        anim.play ();
        outRadioGroup.setSelectedIndex (0);
    }
    
    function playAnimWithCurve (curve, name) {

        // Set the StockCurve name.
        stockCurveDescription.text = name;

        // If the easing curve selected results in an abrupt stop, we break the egg when the animation is over.
        if (name.indexOf ("InOut") == -1 && (name.indexOf ("In") >= 0 || name.indexOf ("Elastic") >= 0 || name.indexOf ("Bounce") >= 0 || name.indexOf ("Linear") >= 0)) {
            breakEgg = true;
        } else {
            breakEgg = false;
        }

        // Get the selected easing curve, update the animation, and play it
        // (first stop it to avoid conflicts if the animation is already running).
        easeAnim.easingCurve = curve;
        anim.stop ();
        anim.play ();
    }
}// RecipePage
