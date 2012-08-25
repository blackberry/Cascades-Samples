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
import "BottomPane"

/**
*  This is a Page layout that displays readings from the devices built in sensors
*
*  You will learn how to:
*  -- Setup a NavagationPane
*  -- Add an ActionItem to the ActionBar
**/

NavigationPane {
    id: nav
	
	Page {
	    objectName: "page1"
	    content: Container {
	        layout: AbsoluteLayout {
	                }
	        ImageView {
			    id: imageHexBackground
			    //Image source: http://commons.wikimedia.org/wiki/File%3AMesilask%C3%A4rg.jpg 
			    //Attribution: By Mariakeernik (Own work) [CC-BY-SA-3.0 (http://creativecommons.org/licenses/by-sa/3.0)], via Wikimedia Commons
			    imageSource: "asset:///images/hex_background2.jpg"
			    preferredWidth: 768
			    preferredHeight: 1280	
			}        
			Label {
			    text: "Hello BB10 !"
			    textStyle {
			        color: Color.White
			    }
				layoutProperties: AbsoluteLayoutProperties {
			        positionX: 300
			        positionY: 300
                }  
			}
            Button {
	        	objectName: "buttonOpenPane1"
	            text: "Open Side Pane"
	            layoutProperties: AbsoluteLayoutProperties {
			        positionX: 20
			        positionY: 120
                }    
	            preferredWidth: 768
	        }
	        
            BottomPane {
                
                id: bottomPane
                layoutProperties: DockLayoutProperties {
                    verticalAlignment: VerticalAlignment.Fill
                    horizontalAlignment: HorizontalAlignment.Fill
                }
            }
            
	    }
	    actions: [
            ActionItem {
                objectName: "bottomButton"
                title: "Open Bottom Pane"
                imageSource: "asset:///images/Gear.png"
                onTriggered: {
                    bottomPane.trigger();
                }
            }
        ]
	}
	
}