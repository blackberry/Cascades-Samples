/* Copyright (c) 2012 Research In Motion Limited.
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

// The Edit Screen is where the text areas for editing and completing information for a new
// record is added together with two buttons for add/edit and cancel.

Sheet {
    id: addSheet
    signal addNewRecord(string firstName, string lastName, string quote)

    Page {
        id: addPage
        
        titleBar: TitleBar {
            id: addBar
            title: qsTr("Add") + Retranslate.onLanguageChanged
            visibility: ChromeVisibility.Visible
            
            dismissAction: ActionItem {
                title: qsTr("Cancel") + Retranslate.onLanguageChanged
                onTriggered: {
                    addSheet.close()
                }
            }
            
            acceptAction: ActionItem {
                title: qsTr("Save") + Retranslate.onLanguageChanged
                enabled: false
                onTriggered: {
                    addNewRecord(firstNameField.text, lastNameField.text, quoteField.text);
                    addSheet.close()
                }
            }
        } // TitleBar
        
        Container {
            id: editPane
            property real margins: 40

            background: backgroundPaint.imagePaint

            attachedObjects: [
                ImagePaintDefinition {
                    id: backgroundPaint
                    imageSource: "asset:///images/background.png"
                }
            ]
            topPadding: margins
            leftPadding: margins
            rightPadding: margins
            bottomPadding: margins
            
            layout: DockLayout {
            }
            
            Container {
                layout: StackLayout {
                }

                // The quote text area.
                TextArea {
                    id: quoteField
                    hintText: qsTr("Quote") + Retranslate.onLanguageChanged
                    bottomMargin: editPane.margins
                    enabled: false
                    maxHeight: 450
                    horizontalAlignment: HorizontalAlignment.Fill
                    
                    layoutProperties: StackLayoutProperties {
                        spaceQuota: 1
                    }
                
                }
                
                Container {
                    
                    layout: StackLayout {
                        orientation: LayoutOrientation.LeftToRight
                    }

                    // Text field for the first name
                    TextField {
                        id: firstNameField
                        hintText: qsTr("First name") + Retranslate.onLanguageChanged
                        enabled: false
                    }

                    // Text field for the last name
                    TextField {
                        id: lastNameField
                        hintText: qsTr("Last name") + Retranslate.onLanguageChanged
                        
                        onTextChanging: {

                            // A last name has to be entered disable buttons and text areas as long as the length is zero.
                            if (text.length > 0) {
                                addPage.titleBar.acceptAction.enabled = true;
                                quoteField.enabled = true;
                                firstNameField.enabled = true;
                            } else {
                                addPage.titleBar.acceptAction.enabled = false;
                                quoteField.enabled = false;
                                firstNameField.enabled = false;
                            }
                        }
                    }//Last name text field
                } // Name text Container
            } //Total text Container (name text + quote text)
        } // Content Container
    }
    
    onOpened: {
        // Focus can not be requested until the Sheet is part of the UI-tree, so we
        // wait until it has been shown and request focus for the last name field
        lastNameField.requestFocus();
    }
}
