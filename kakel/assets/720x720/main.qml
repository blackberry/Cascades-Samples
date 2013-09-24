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
    Container {
        topPadding: 20
        background: backgroundPaint.imagePaint
        attachedObjects: [
            ImagePaintDefinition {
                id: backgroundPaint
                imageSource: "asset:///images/background.png"
            }
        ]

        // Kakel Title Label and Move Count Label when playing
        Label {
            horizontalAlignment: HorizontalAlignment.Center
            textStyle.base: appDefaultBigText.style
            
            // kakel.numMoves is a property that we keep track on in cpp code
            // every time it's changed the text will be updated.
            text: (kakel.numMoves == 0) ? "KAKEL" : "Moves: " + kakel.numMoves       
        }        
                
   
        // Group the container's player area, a background and a nice flare on top.
        Container {
            topMargin: 20
            horizontalAlignment: HorizontalAlignment.Center
            
            layout: DockLayout {
            }

            // This container is the one we find from CPP and add all the tiles to
            // It's so much easier to do that from C++.
            Container {
                objectName: "playArea"

                layout: StackLayout {
                    orientation: LayoutOrientation.LeftToRight
                }

                // The dark gray is an awesome trick, we can use it to see where we put our player area when layouting.
                background: Color.DarkGray
                preferredWidth: 500
                preferredHeight: preferredWidth                
            }
        }
        
        // Container for simple/complex mode buttons
        Container {
            topMargin: 20
            verticalAlignment: VerticalAlignment.Bottom
            horizontalAlignment: HorizontalAlignment.Center
            layout: StackLayout {
                orientation: LayoutOrientation.LeftToRight
            }

            // These buttons will call the C++ code to generate new play fields.
            // You can alter the size of the grid, go ahead and try it!
            Button {
                text: "Simple"
                imageSource: "asset:///images/icon_simple"
                onClicked: {
                    kakel.setNumTiles(4);
                    kakel.resetTiles();
                }
            }
            Button {
                text: "Complex"
                imageSource: "asset:///images/icon_complex"
                onClicked: {
                    kakel.setNumTiles(6);
                    kakel.resetTiles();
                }
            }// Button
        }// Container
    }// Main Container
    
    attachedObjects: [
        TextStyleDefinition {
            id: appDefaultBigText
            base: SystemDefaults.TextStyles.TitleText
            fontWeight: FontWeight.Default
            color: Color.create("#E6E9F8")
        },
        TextStyleDefinition {
            id: appDefaultTitleText
            base: SystemDefaults.TextStyles.TitleText
            fontWeight: FontWeight.Default
            color: Color.create("#E6E9F8")
        }        
    ]
}// Page
