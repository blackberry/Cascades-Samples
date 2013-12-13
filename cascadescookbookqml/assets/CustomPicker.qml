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

RecipePage {
    RecipeContainer {
            Container {
                topPadding: 35
                
                Container {
                    leftPadding: 20
                    rightPadding: 20
                    bottomPadding: 20

					Label {
					    id: testPickertext
         				text: "What to select";        				
         		    }
					Label {
         				id: priceText
         				text: " "
         			}
					
					Container {
         				preferredHeight: 20
         				preferredWidth: 20
         			}

                    Picker {
                        id: picker
                        title: "Custom Pizza Picker"
                        expanded:true
                        dataModel: XmlDataModel {
                            source: "models/custompickermodel.xml"
                        }

                        pickerItemComponents: [
                            PickerItemComponent {
                                type: "pizza"
                                content: Container {
                                    layout: DockLayout {
                                    }
                                    Label {
                                        multiline: true
                                        maxWidth: 250
                                        text: pickerItemData.text
                                        verticalAlignment: VerticalAlignment.Center
                                        horizontalAlignment: HorizontalAlignment.Center
                                        
                                        textStyle {
                                            base: SystemDefaults.TextStyles.SubtitleText
                                        }
                                    }
                                }
                            },
                            PickerItemComponent {
                                type: "size"
                                content: Container {
                                    layout: DockLayout {
                                    }
                                    topPadding: 5
                                    bottomPadding: 5
                                    leftPadding: 15
                                    rightPadding: 15
                                    
                                    Label {
                                        text: {
                                            var sizeData = pickerItemData.text
                                            sizeData
                                        }
                                        verticalAlignment: VerticalAlignment.Center
                                        horizontalAlignment: HorizontalAlignment.Left
    
                                        textStyle {
                                            base: SystemDefaults.TextStyles.SubtitleText
                                        }
                                    }
                                    
	                                ImageView {
	                                    scaleX: 0.75
	                                    scaleY: 0.75
	                                    rotationZ: 15
	                                    imageSource: "asset:///images/custompicker/sliceImage.png"
	                                    horizontalAlignment: HorizontalAlignment.Right
	                                    verticalAlignment: VerticalAlignment.Center   
	                                }  
	                                    
	                                Label {
                                    	text: "x" + pickerItemData.slices
                                    	verticalAlignment: VerticalAlignment.Bottom
                                    	horizontalAlignment: HorizontalAlignment.Right 
                                            
                                        textStyle {
                                            base: SystemDefaults.TextStyles.SubtitleText
                                        }
                                    }
                                }
                            },
                            PickerItemComponent {
                                type: "style"
                                content: Container {
                                    layout: DockLayout {   
                                    } 
                                    Label {
                                        text: pickerItemData.text
                                        horizontalAlignment: HorizontalAlignment.Center
                                        verticalAlignment: VerticalAlignment.Center
                                        
                                        textStyle {
                                            base: SystemDefaults.TextStyles.SubtitleText
                                        }
                                    }
                                }
                            }
                        ]

                        onSelectedValueChanged: {
                            var index0 = picker.selectedIndex(0);
                            var index1 = picker.selectedIndex(1);
                            var index2 = picker.selectedIndex(2);
                            
                            var type = dataModel.data([0, picker.selectedIndex(0)]);
                            var size = dataModel.data([1, index1]);
                            var extra = dataModel.data([2, index2]);
                            
                            // Total amount to pay for the pizza.
                            var price = (type.price*size.factor)+ Number(extra.price);
                            
                            testPickertext.text ="Order: " + type.text +", "+size.text+", "+extra.text+"."
                            priceText.text = "Bill:  " + price + " $"
                            picker.description = price + " $"
                        }    
                    } // Picker
                } // Container
            } // Container
    } // RecipeContainer
}// RecipePage
