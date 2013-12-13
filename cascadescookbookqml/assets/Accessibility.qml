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

// This is a simple A11y recipe that utilizes the screen reader functionality.
RecipePage {
    RecipeContainer {
        ScrollView {
            scrollViewProperties {
                scrollMode: ScrollMode.Vertical
            }
            Container {
                topPadding: 30
                leftPadding: 50
                rightPadding: leftPadding

                // This part contains instructions on how to use the screen reader functionality
                Label {
                    multiline: true
                    text: qsTr("Start the screenreader by pressing the powerbutton 3 times. Listen to accessibility data either by tapping or dragging on the screen. Double tap anywhere to execute an action \n\n/The Chef")
                }
                ImageView {
                    imageSource: "asset:///images/mushroom_soup.png"
                    // The accessibility tag here tells the screen reader to look at the label for more info.
                    accessibility {
                        labelledBy: soupLabel
                    }
                }
                Label {
                    id: soupLabel
                    text: "Image of the tasty soup"
                    textStyle.fontSize: FontSize.XSmall
                }
                // This container handles the switch between US measurements and Metric measurement.
                Container {
                    layout: StackLayout {
                        orientation: LayoutOrientation.LeftToRight

                    }
                    topMargin: 25
                    Label {

                        verticalAlignment: VerticalAlignment.Center

                        layoutProperties: StackLayoutProperties {
                            spaceQuota: 1
                        }
                        id: textlabel
                        text: "Choose unit:"

                    }
                    SegmentedControl {
                        layoutProperties: StackLayoutProperties {
                            spaceQuota: 1
                        }
                        Option {
                            id: metric
                            text: "Metric"
                            value: "metric"
                        }
                        Option {
                            text: "US"
                            value: "us"
                        }
                        onSelectedIndexChanged: {
                            if (metric.selected) {
                                mushroomLabel.text = "200g of mushrooms."
                                creamLabel.text = "1 dl of cream."
                            } else {
                                mushroomLabel.text = "7 ounces of mushrooms."
                                creamLabel.text = "Half a cup of cream."
                            }
                        }
                    }
                }
                // Nothing special, just a mushroom soup.
                Label {
                    topMargin: 50
                    text: "Ingredients:"
                    textStyle.fontSize: FontSize.Large
                    textStyle.fontWeight: FontWeight.Bold
                }
                Label {
                    id: mushroomLabel
                    text: "200g of mushrooms"
                }
                Label {
                    id: creamLabel
                    text: "1 dl of cream."
                }
                Label {
                    text: "a pinch of salt."
                }
                Label {
                    text: "3 straws of parsnip."
                    // The accessibility tag here tells the screen reader to look at the label for more info.
                    accessibility {
                        description: "Get three straws of parsnip that are about 10cm in length."
                    }
                }
                Label {
                    topMargin: 50
                    text: "Preparation Method:"
                    textStyle.fontSize: FontSize.Large
                    textStyle.fontWeight: FontWeight.Bold
                }
                Label {
                    multiline: true
                    text: "Put all in a mixer, \nMix until smooth. \nHeat on stove until hot. \nEnjoy"
                }
            }
        } // Scrollview
    } // RecipeContainer
}// RecipePage
