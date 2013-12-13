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

// This recipe shows how to use different text styles. It is shown how to style text 
// using: SystemDefaults, FontSize, fontfamily and a custom font.
RecipePage {
    property string alphabet: "A B C D E F G H I J K L M N O P Q R S T U V X Y Z\n" +
    "a b c d e f g h i j k l m n o p q r s t u v x y z\n"

	RecipeContainer {
	    ScrollView {
        	Container {
        	    topPadding: 20
        	    
        	    // The standard text styles that can be accessed via SystemDefaults.
                Header {
                    title: "Default TextStyles"
                }
                Container {
                    leftPadding: 20
                    rightPadding: leftPadding

					Label {
                        text: "BigText"
                        textStyle {
                            base: SystemDefaults.TextStyles.BigText
                        }
                    }
                    Label {
                        text: "TitleText"
                        textStyle {
                            base: SystemDefaults.TextStyles.TitleText
                        }
                    }
                    
                    Label {
                        text: "PrimaryText"
                        textStyle {
                            base: SystemDefaults.TextStyles.PrimaryText
                        }
                    }
                    Label {
                        text: "BodyText"
                        textStyle {
                            base: SystemDefaults.TextStyles.BodyText
                        }
                    }
                    Label {
                        text: "SubtitleText"
                        textStyle {
                            base: SystemDefaults.TextStyles.SubtitleText
                        }
                    }
                    Label {
                        text: "SmallText"
                        textStyle {
                            base: SystemDefaults.TextStyles.SmallText
                        }
                    } 
                }
                
                // This is how you set the FontSize in a more explicit manner, check the last 
                // Label for really explicit using PointValue.
                Header {
                    title: "FontSize examples"
                }

                Container {
                    leftPadding: 20
                    rightPadding: leftPadding

                    Label {
                        text: "Small"
                        textStyle {
                            fontSize: FontSize.Small
                        }
                    }
                    Label {
                        text: "Large"
                        textStyle {
                            fontSize: FontSize.Large
                        }
                    }
                    Label {
                        text: "XLarge"
                        textStyle {
                            fontSize: FontSize.XLarge
                        }
                    }
                    Label {
                        text: "XXLarge"
                        textStyle {
                            fontSize: FontSize.XXLarge
                        }
                    }
                    Label {
                        text: "PointValue " + textStyle.fontSizeValue
                        textStyle {
                            fontSize: FontSize.PointValue
                            fontSizeValue: 23
                        }
                    }
                }

				// There are a couple of generic font families that you can use for 
				// formatting text. Use with care ;-)
                Header {
                    title: "Font Family Generic"
                }
                Container {
                    leftPadding: 20
                    rightPadding: 20
                    Label {
                        text: "Sans-serif"
                    }
                    Label {
                        text: "Serif"
                        textStyle {
                            fontFamily: "Serif"
                        }
                    }
                    
                    Label {
                        text: "Monospace"
                        textStyle {
                            fontFamily: "Monospace"
                        }
                    }
                    Label {
                        text: "Fantasy"
                        textStyle {
                            fontFamily: "Fantasy"
                        }
                    }
                    Label {
                        text: "Cursive"
                        textStyle {
                            fontFamily: "Cursive"
                        }
                    }
                }
                
                // It is possible to use your own fonts, in this example it is set up using a attached object
                // take a look at the bottom of this file to see how its done. 
                Header {
                    title: "Custom Font"
                }
                Container {
                    leftPadding: 20
                    rightPadding: leftPadding
                    
                    Container {
                        layout: StackLayout {
                            orientation: LayoutOrientation.LeftToRight
                        }
                        Label {
                            text: "SourceCodePro-Medium"
                            multiline: true
                            textStyle {
                                // The custom font is set up as an attached object at the bottom of this file.
                                base: customTextStyle.style
                                fontFamily: "SourceCodePro-Medium"
                            }
                        }
                        
                        Label {
                            text: alphabet
                            multiline: true
                            textStyle {
                                base: customTextStyle.style
                                fontFamily: "SourceCodePro-Medium"
                            }
                        }
                    }
                    
                    // The other text styling properties applies to a custom font as well, here is an example.
                    Label {
                        text: "Additional text styling applied to a custom font."
                        multiline: true
                        textStyle {
                            // The custom font is set up as an attached object at the bottom of this file.
                            base: customTextStyle.style
                            fontSize: FontSize.Large
                            fontWeight: FontWeight.Bold
                            color: Color.create("#0073BC")
                            fontFamily: "SourceCodePro-Medium"
                        }
                    }
                }
            }
        }
    }
	
    attachedObjects: [
        TextStyleDefinition {
            id: customTextStyle
            
            // A FontFaceRule is used to add an internal font family name to an external font.
            // Here a custom font is loaded from the fonts folder in assets.
            rules: [
                FontFaceRule {
                    id: baseStyleFontRule
                    source: "asset:///fonts/SourceCodePro_Fonts/TTF/SourceCodePro-Medium.ttf"
                    fontFamily: "SourceCodePro-Medium"
                }
            ]
        }
    ]
}