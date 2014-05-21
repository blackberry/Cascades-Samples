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

// A Component which shows a Label in a ScrollView, which line of the Label
// that is the top most visible line can be controlled via the line property.
Container {
    id: speedTextContainer
    
    // The speed text Container contain a Label within a ScrollView and
    // its position is controlled via which line should be the top most visible line.
    property int line: 0;
    property string remainingText: ""
    property alias text: speedTextLabel.text
    
    background: fieldBackgroundPaint.imagePaint
    bottomPadding: ui.sdu(0.8)
    
    attachedObjects: [
        ImagePaintDefinition {
            id: fieldBackgroundPaint
            imageSource: "asset:///images/border_image_text_field_source.png.amd"
        }
    ]
        
    // The speed text is put inside a ScrollView and its position is controlled 
    // in onLineChanged signal handler below.
    ScrollView {
        id: labelscroller
        horizontalAlignment: HorizontalAlignment.Center
        
        // Need to add a scrollmode to prevent touch interaction with the scrollview control. Will just show the text.
        scrollViewProperties {
            scrollMode: ScrollMode.None
        }
        
        Label {
            id: speedTextLabel
            property int lineOffset: 0
            multiline: true
            preferredWidth: ui.du(68.8)
                        
            textStyle {
                fontSize: FontSize.PointValue
                fontSizeValue: 10
            }
            
            attachedObjects: [
                // In order to get the height of on line of text we use a layout update handler
                // it measures the empty label to get the height of one line.
                LayoutUpdateHandler {
                    onLayoutFrameChanged: {
                        if (speedTextLabel.lineOffset === 0) {
                            speedTextLabel.lineOffset = layoutFrame.height
                            
                            // Once measured set the initial text.
                            speedTextLabel.text = "<html><span style='color:#e0e0e0;'>" + remainingText + "</span></html>"
                        }
                    }
                }
            ]
        }
    }
    
    onLineChanged: {
        // Show next line as input progresses.
        scrollToLine(line);
    }
    
    function scrollToLine(line) {
        labelscroller.scrollToPoint(0,  line * speedTextLabel.lineOffset);
    }
}
