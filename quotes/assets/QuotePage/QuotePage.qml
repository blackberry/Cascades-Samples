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

Page {
    id: quotePage
    
    // Data property, this is updated when navigation occurs, has three entries.
    property variant quoteData: {'firstname': "John", 'lastname':"Doe", 'quote':"Bla bla bla bla blaaa."}
    
    // Signals for handling data updates triggered from the Quote Page.
    signal deleteQuote();
    signal updateQuote(string firstName, string lastName, string quote);
    signal back();
        
    titleBar: TitleBar {
        title: qsTr("Edit") + Retranslate.onLanguageChanged
        visibility: quoteBubble.editMode ? ChromeVisibility.Visible : ChromeVisibility.Hidden; 
        acceptAction: ActionItem {
            title: qsTr("Save") + Retranslate.onLanguageChanged
        	enabled: quoteBubble.enableSave
            onTriggered: {
                quoteBubble.editMode = false;
                quotePage.updateQuote(quoteBubble.pendingFirstName, quoteBubble.pendingLastName, quoteBubble.quoteText);
            }
        }

        dismissAction: ActionItem {
            title: qsTr("Cancel") + Retranslate.onLanguageChanged
            onTriggered: {
                quoteBubble.editMode = false;
                quoteBubble.quoteText = quotePage.quoteData["quote"];
            }
        }
    }
    
    Container {
        background: backgroundPaint.imagePaint
        			
        attachedObjects: [
            ImagePaintDefinition {
                id: backgroundPaint		
                imageSource: "asset:///images/background.png"
            }
        ]
        
        layout: DockLayout {
        }
        
        QuoteBubble {
            id: quoteBubble
            // Since the data presented in the Quote bubble is connected to the list of wuotes
            // and may change (on for example deletion), some of the properties of the QuoteBuble are set in the 
            // onQuoteDataChanged signal handler below.

			horizontalAlignment: HorizontalAlignment.Center
            verticalAlignment: VerticalAlignment.Center
        } // QuoteBubble
    } // Container

    shortcuts: [
        SystemShortcut {
            // The edit  short cut puts the Page in edit mode.
            type: SystemShortcuts.Edit
            onTriggered: {
                quoteBubble.editMode = true;
            }
        }
    ]

    paneProperties: NavigationPaneProperties {
        backButton: ActionItem {
            title: qsTr("Names") + Retranslate.onLanguageChanged
            onTriggered: {
                quoteBubble.editMode = false;
                quotePage.back();
            }
        }
    }
    
    actions: [
        ActionItem {            
            title: qsTr("Edit") + Retranslate.onLanguageChanged
            imageSource: "asset:///images/Edit.png"
            ActionBar.placement: ActionBarPlacement.OnBar
            enabled: !quoteBubble.editMode

            onTriggered: {
                quoteBubble.editMode = true
            }
        },
        DeleteActionItem {
            objectName: "DeleteAction"
            
            onTriggered: {
                quotePage.deleteQuote();
                quoteBubble.editMode = false
            }
        }
    ]
    
    onQuoteDataChanged: {
        // If the first name is not defined set it to an empty string
        quoteBubble.firstName =  (quotePage.quoteData["firstname"] == undefined) ? "" : quotePage.quoteData["firstname"]
        quoteBubble.lastName = quotePage.quoteData["lastname"]
        quoteBubble.quoteText = quotePage.quoteData["quote"]    
    }
    
    attachedObjects: [
        // Style definitions needed since this page is not following the current theme set
        TextStyleDefinition {
            id: quoteStyleLightBody
            base: SystemDefaults.TextStyles.BodyText
            color: Color.create("#fafafa")
        }
    ]
    
}// Quote Page
