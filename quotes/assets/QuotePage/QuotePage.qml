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
import bb.cascades 1.0

Page {
    
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
            horizontalAlignment: HorizontalAlignment.Center
            verticalAlignment: VerticalAlignment.Center
        } // QuoteBubble
    } // Container
    
    paneProperties: NavigationPaneProperties {
        backButton: ActionItem {
            title: "Names"
            onTriggered: {
                nav.pop();
                quoteBubble.editMode = false;
            }
        }
    }
    
    actions: [
        ActionItem {            
            title: "Edit"
            imageSource: "asset:///images/Edit.png"
            ActionBar.placement: ActionBarPlacement.OnBar
            
            onTriggered: {
                quoteBubble.editMode = true
            }
        },
        DeleteActionItem {
            objectName: "DeleteAction"
            title: "Delete"
            
            onTriggered: {
                _quoteApp.deleteRecord();
                quoteBubble.editMode = false
            }
        }
    ]
    
    attachedObjects: [
        // Style definitions needed since this page is not following the current theme set
        TextStyleDefinition {
            id: quoteStyleLightBody
            base: SystemDefaults.TextStyles.BodyText
            color: Color.create("#fafafa")
        }
    ]
    
}// Quote Page
