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

RecipePage {
    RecipeContainer {
            Container {
                topPadding: ui.du(3)
                
                Container {
                    leftPadding: ui.du(2)
                    rightPadding: ui.du(2)

					Label {
					    id: testPickertext
         				text: "What to select";        				
         		    }
					Label {
         				id: priceText
         				text: " "
         			}
					
					Container {
         				preferredHeight: ui.du(2)
         				preferredWidth:  ui.du(2)
         			}

                    Picker {
                        id: picker
                        title: "Custom Pizza Picker"
                        expanded:true

                        // A DataModel is used to populate the picker.
                        dataModel: XmlDataModel {
                            source: "models/custompickermodel.xml"
                        }

                        // Picker items are set up similarly as to how its done in a ListView.
                        pickerItemComponents: [
                            PickerItemComponent {
                                type: "pizza"
                                content: Container {
                                    layout: DockLayout {
                                    }
                                    Label {
                                        multiline: true
                                        maxWidth:  ui.du(25)
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
                                    topPadding: ui.du(.5)
                                    bottomPadding: ui.du(.5)
                                    leftPadding: ui.du(1)
                                    rightPadding: ui.du(1)
                                    
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
                            // These are the currently selected indexes.
                            var index0 = picker.selectedIndex(0);
                            var index1 = picker.selectedIndex(1);
                            var index2 = picker.selectedIndex(2);
                            
                            // Get the data items corresponding to the selection.
                            var type = dataModel.data([0, picker.selectedIndex(0)]);
                            var size = dataModel.data([1, index1]);
                            var extra = dataModel.data([2, index2]);
                            
                            // And calculate the total amount to pay for the pizza base on the new selection.
                            var price = (type.price*size.factor)+ Number(extra.price);
                            
                            testPickertext.text ="Order: " + type.text +", "+size.text+", "+extra.text+"."
                            priceText.text = "Bill:  " + price + " $"
                            picker.description = price + " $"
                        }    
                    } 
                }
            }
    }
}
