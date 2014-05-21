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

// This recipe shows how Color can be set on text and Containers 
// and gives examples of colors used in the blackberry 10 platform.
// The QML uses a helper QML file (ColorSwatch.qml) that layouts a rect and a text
// so that we can easily call it multiple times with all the colors!
RecipePage {
    RecipeContainer {
        ScrollView {
            Container {
                topPadding: ui.du(2)
                leftPadding: topPadding
                rightPadding: topPadding
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
                    layout: StackLayout {
                        orientation: LayoutOrientation.LeftToRight
                    }
                    
                    // A white icon is colored by the iconColor in the component found in ColorIcon.qml 
                    ColorIcon {
                        iconImageSource: "asset:///images/color/icon_outgoing.png"
                        iconColor: Color.create("#00B800");
                        layoutProperties: StackLayoutProperties {
                            spaceQuota: 1
                        }
                    }
                    
                    ColorIcon {
                        iconImageSource: "asset:///images/color/icon_waiting.png"
                        iconColor: Color.create("#E6B400");
                        layoutProperties: StackLayoutProperties {
                            spaceQuota: 1
                        }
                    }
                    
                    ColorIcon {
                        iconImageSource: "asset:///images/color/icon_missed.png"
                        iconColor: Color.create("#D60000");
                        layoutProperties: StackLayoutProperties {
                            spaceQuota: 1
                        }
                    }
                }

                // Primary colors.
                Label {
                    verticalAlignment: VerticalAlignment.Center
                    text: "Primary"

                    textStyle {
                        base: SystemDefaults.TextStyles.BigText
                    }
                }

                ColorSwatch {
                    swatchColor: ui.palette.primary
                }
                ColorSwatch {
                    swatchColor: ui.palette.primaryBase
                }
                ColorSwatch {
                    swatchColor: ui.palette.primarySoft
                }
                
                // Application colors.
                Label {
                    verticalAlignment: VerticalAlignment.Center
                    text: "Application"
                    
                    textStyle {
                        base: SystemDefaults.TextStyles.BigText
                    }
                }
                
                ColorSwatch {
                    swatchColor: Color.create("#96B800")
                }
                ColorSwatch {
                    swatchColor: Color.create("#CC3F10")
                }
                ColorSwatch {
                    swatchColor: Color.create("#0098F0")
                }
                ColorSwatch {
                    swatchColor: Color.create("#A30D7E")
                }
                ColorSwatch {
                    swatchColor: Color.create("#667B94")
                }


                Label {
                    verticalAlignment: VerticalAlignment.Center
                    text: "Text"
                    
                    textStyle {
                        base: SystemDefaults.TextStyles.BigText
                    }
                }
                
                ColorSwatch {
                    swatchColor: ui.palette.text
                }
                ColorSwatch {
                    swatchColor: ui.palette.textPrimary
                }
                
                // The following two colors are meant to be used when the background has a certain color.
                ColorSwatch {
                    swatchColor: ui.palette.textOnPrimary
                    background: ui.palette.primary
                }
                ColorSwatch {
                    swatchColor: ui.palette.textOnPlain
                    background: ui.palette.plain
                }

                // Background colors.
                Label {
                    verticalAlignment: VerticalAlignment.Center
                    text: "Background"
                    
                    textStyle {
                        base: SystemDefaults.TextStyles.BigText
                    }
                }
                
                // The background colors are contrasted against the primary color to make them visible.
                ColorSwatch {
                    swatchColor: ui.palette.background
                    background: ui.palette.primary
                }
                
                ColorSwatch {
                    swatchColor: ui.palette.plain
                    background: ui.palette.primary
                }
                
                ColorSwatch {
                    swatchColor: ui.palette.plainBase
                    background: ui.palette.primary
                }                

            }
        }
    }
}
