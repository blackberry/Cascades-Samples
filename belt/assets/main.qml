/*
 * Copyright (c) 2014 BlackBerry Limited.
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

import bb.cascades 1.4
import bb.device 1.3

Page {
    id: main
    // List of button names to be used for random name assignment
    property variant beltText: ["","call", "", "", "end", "", "menu", "back","", "trackpad"]
    
    // List of button colors that correspond to each of the above button names
    property variant beltColors: ["#EFEFEF","#44E500","#EFEFEF","#EFEFEF","#FF0000","#EFEFEF","#FFD800","#0094FF","#EFEFEF","#FF6DDA"]
    
    // Number of buttons that have been marked for clearing
    property int markedButtons: 0
    
    Container {
        layout: DockLayout { }
        
        Container {
            verticalAlignment: VerticalAlignment.Top
            id: root
            
            // Label for application description
            Label {
                // Localized text with the dynamic translation and locale updates support
                text: qsTr("Use trackpad to maneuver and press belt-key corresponding to button text. Clear all buttons to win!") + Retranslate.onLocaleOrLanguageChanged
                multiline: true
                textStyle.base: SystemDefaults.TextStyles.BodyText
            }
            
            Divider { }
            
            attachedObjects: [
                // Custom component representing a row of buttons                 
                ComponentDefinition {                     
                    id: row                      
                    source: "Row.qml"             
                },
                // Divider to insert at end of game board
                ComponentDefinition {
                    id: divider
                    
                    Divider { }
                },
                // Allows access to screen width, height in order to
                // know how many rows of buttons to generate.
                DisplayInfo {
                    id: displayInfo
                }
            ]
        }
        Container {
            verticalAlignment: VerticalAlignment.Bottom
            horizontalAlignment: HorizontalAlignment.Center
            
            // Button to allow user to begin game by generating button grid
            Button {
                id: generate
                text: "Generate Grid"
                onClicked: {
                    markedButtons = 0
                    
                    // Retrieve screen height and width
                    var height = displayInfo.pixelSize.height
                    var width = displayInfo.pixelSize.width
                    
                    // Calculation to determine number of buttons in a row based on screen width
                    var columns = (width - ui.du(10) - (ui.du(25) / 2))
                    columns = (columns / ui.du(25))
                    
                    // Take height and subtract padding
                    height = (height - ui.du(30))
                    
                    // 65 is the height of button plus padding
                    var rows = height / 65
                    
                    // Calculation to determine number of rows based on screen height
                    rows = (height - (rows * ui.du(2))) / 56
                    
                    // Generate the column rows of buttons
                    for(var i = 0; i < rows; i++) {
                        var createdObject = row.createObject()
                        createdObject.createButtons(columns, beltText, beltColors)
                        createdObject.topMargin = ui.du(2)
                        root.add(createdObject)
                    }
                    
                    // Add the ending divider
                    root.add(divider.createObject())
                    
                    // Remove button to make room for winning label
                    generate.visible = false
                }
                
                onCreationCompleted: {
                    requestFocus()
                }
            }
            // Label to be displayed when game is won
            Label {
                id: endGame
                visible: false
                text: "You WIN! - Thanks for Playing Belt."
                textStyle.base: SystemDefaults.TextStyles.BodyText
            }
        }
    }
    
    // Increments the number of buttons that have been marked
    // with belt actions to perform
    function markedButtonIncrement() {
        markedButtons++
    }
    
    // Verifies if the User has reached the end game goal
    function checkForWin() {
        --markedButtons
        if(markedButtons == 0) {
            endGame.visible = true;
        }
    }
}
