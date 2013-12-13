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

// This recipe shows how Color can be set on text and Containers 
// and gives examples of colors used in the blackberry 10 platform.
// The QML uses a helper QML file (ColorHelper.qml) that layouts a rect and a text
// so that we can easily call it multiple times with all the colors!
RecipePage {
    RecipeContainer {
        ScrollView {
            scrollViewProperties {
                scrollMode: ScrollMode.Vertical
            }
            Container {
                topPadding: 20
                leftPadding: 20
                rightPadding: 20
                bottomPadding: 20
                horizontalAlignment: HorizontalAlignment.Center
                verticalAlignment: VerticalAlignment.Center

                Label {
                    verticalAlignment: VerticalAlignment.Center
                    text: "Functional"

                    textStyle {
                        base: SystemDefaults.TextStyles.BigText
                    }
                }
                
                // Functional colors.
                Container {
                    horizontalAlignment: HorizontalAlignment.Fill
                    layout: DockLayout {
                    }
                    
                    Container {
                        horizontalAlignment: HorizontalAlignment.Left
                        layout: StackLayout {
                        }
                        ImageView {
                            horizontalAlignment: HorizontalAlignment.Center
                            imageSource: "asset:///images/color/icon_outgoing.png"
                        }
                        Label {
                            verticalAlignment: VerticalAlignment.Bottom
                            text: "#00B800"

                            textStyle {
                                base: SystemDefaults.TextStyles.BodyText
                                fontWeight: FontWeight.Bold
                                color: Color.create("#00B800")
                            }
                        }
                    }
                    
                    Container {
                        horizontalAlignment: HorizontalAlignment.Center
                        layout: DockLayout {
                        }
                        Container {
                            horizontalAlignment: HorizontalAlignment.Left
                            layout: StackLayout {
                            }
                            ImageView {
                                horizontalAlignment: HorizontalAlignment.Center
                                imageSource: "asset:///images/color/icon_waiting.png"
                            }
                            Label {
                                verticalAlignment: VerticalAlignment.Center
                                text: "#E6B400"

                                textStyle {
                                    base: SystemDefaults.TextStyles.BodyText
                                    fontWeight: FontWeight.Bold
                                    color: Color.create("#E6B400")
                                }
                            }
                        }
                    }
                    
                    Container {
                        horizontalAlignment: HorizontalAlignment.Right
                        layout: DockLayout {
                        }
                        Container {
                            horizontalAlignment: HorizontalAlignment.Left
                            layout: StackLayout {
                            }
                            ImageView {
                                horizontalAlignment: HorizontalAlignment.Center
                                imageSource: "asset:///images/color/icon_missed.png"
                            }
                            Label {
                                verticalAlignment: VerticalAlignment.Center
                                text: "#D60000"

                                textStyle {
                                    base: SystemDefaults.TextStyles.BodyText
                                    fontWeight: FontWeight.Bold
                                    color: Color.create("#D60000")
                                }
                            }
                        }
                    }
                }//Container for functional colors end.

                // Brand colors.
                Label {
                    verticalAlignment: VerticalAlignment.Center
                    text: "Brand"

                    textStyle {
                        base: SystemDefaults.TextStyles.BigText
                    }
                }

                ColorHelper {
                    colortext: "#0073BC"
                    containerColor: Color.create("#0073BC")
                }
                ColorHelper {
                    colortext: "#00A8DF"
                    containerColor: Color.create("#00A8DF")
                }

                // Application colors.
                Label {
                    verticalAlignment: VerticalAlignment.Center
                    text: "Application"

                    textStyle {
                        base: SystemDefaults.TextStyles.BigText
                    }
                }

                ColorHelper {
                    colortext: "#96B800"
                    containerColor: Color.create("#96B800")
                }
                ColorHelper {
                    colortext: "#CC3F10"
                    containerColor: Color.create("#CC3F10")
                }
                ColorHelper {
                    colortext: "#0098F0"
                    containerColor: Color.create("#0098F0")
                }
                ColorHelper {
                    colortext: "#A30D7E"
                    containerColor: Color.create("#A30D7E")
                }
                ColorHelper {
                    colortext: "#667B94"
                    containerColor: Color.create("#667B94")
                }

                // Background colors.
                Label {
                    verticalAlignment: VerticalAlignment.Center
                    text: "Background"

                    textStyle {
                        base: SystemDefaults.TextStyles.BigText
                    }
                }

                ColorHelper {
                    colortext: "#121212"
                    containerColor: Color.create("#121212")
                }
                Container {
                    background: Color.create("#667B94")
                    topMargin: 4
                    rightPadding: 4
                    leftPadding: 4
                    bottomPadding: 4
                    ColorHelper {
                        colortext: "#F8F8F8"
                        containerColor: Color.create("#F8F8F8")
                    }
                }
                ColorHelper {
                    colortext: "#262626"
                    containerColor: Color.create("#262626")
                }

            } // Container
        } //ScrollView
    } // RecipeContainer
}// RecipePage
