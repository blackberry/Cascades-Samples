/* Copyright (c) 2013, 2014 BlackBerry Limited.
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

// A Container that contain a grid of colored boxes in a semi-random pattern,
// inspired by the legendary Bauhaus artist Josef Albers "Homage to the Square". 
Container {
	horizontalAlignment: HorizontalAlignment.Fill
    verticalAlignment: VerticalAlignment.Fill
    background: Color.create("#d6d5d0")

    layout: DockLayout {
    }

    Container {
        verticalAlignment: VerticalAlignment.Center
        horizontalAlignment: HorizontalAlignment.Center
        rightPadding: ui.du(2)
        bottomPadding: ui.du(2)
                
        AlbersGrid {
            id: alberGrid
        }
    }

    onTouch: {
        if(event.touchType == TouchType.Down){
            setColorsAndPosition();
        }
    }
            
    attachedObjects: XmlDataModel {
        id: colorModel 
        source: "model/colormodel.xml"
    }
    
    function setColorsAndPosition() {        
        // In this function the colors and position of the boxes
        // are set up, colors are taken from a model and positions
        // are set up by adjusting the padding.
        var nbrOfColors = colorModel.childCount([]);
        var colorItemIndex = Math.floor(Math.random() * nbrOfColors);
        var colorData = colorModel.data([colorItemIndex]);
        
        alberGrid.color1 = Color.create("#"+colorData.color1);
        alberGrid.color2 = Color.create("#"+colorData.color2);
        alberGrid.color3 = Color.create("#"+colorData.color3);
        alberGrid.color4 = Color.create("#"+colorData.color4);

		alberGrid.box1Top = 5 + Math.floor(Math.random() * 35)
        alberGrid.box1Left = 5 + Math.floor(Math.random() * 35)
        alberGrid.box2Top = 5 + Math.floor(Math.random() * 30)     
        alberGrid.box2Left = 5 + Math.floor(Math.random() * 30)
        alberGrid.box3Top = 5 + Math.floor(Math.random() * 15)
        alberGrid.box3Left = 5 + Math.floor(Math.random() * 15)
    }
    
    onCreationCompleted: {
        setColorsAndPosition()
    }
}
