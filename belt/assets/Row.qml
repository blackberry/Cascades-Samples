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

// Custom container representing a row of buttons
Container {
    id: row
    property int leftmargin: 2
    
    leftPadding: ui.du(5)
    rightPadding: ui.du(5)
    
    layout: StackLayout {
        orientation: LayoutOrientation.LeftToRight
    }
    
    attachedObjects: [ 
        // Custom belt button representation                
        ComponentDefinition {                     
            id: compDef                      
            source: "BeltButton.qml"             
        }
    ] 
    
    // Function that creates the number of buttons depending
    // on the columns value
    function createButtons(columns, beltText, beltColors) {
        
        // Create columns number of buttons
        for(var i = 0; i < columns; i++) {
            var createdObject = compDef.createObject()
            
            // create a random number that will represent the index
            // into the list of button text's to use
            var rand = Math.floor((Math.random() * beltText.length))
            
            createdObject.leftMargin = ui.du(leftmargin)
            
            var text = beltText[rand]
            
            // If the text is not an empty string, then it means
            // that it's a belt action for the user to perform
            // Increment number of buttons the user needs to clear           
            if(text != "") {
                main.markedButtonIncrement()
            }
            
            // Set the text, and color of the botton
            // and add it to the container
            createdObject.text = beltText[rand]
            createdObject.color = beltColors[rand]
            row.add(createdObject)
        }
    }
}
