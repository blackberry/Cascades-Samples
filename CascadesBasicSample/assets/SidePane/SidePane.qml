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

/**
*  This is a simple Page layout that displays readings from the devices built in sensors
*
*  You will learn how to:
*  -- Define a back button for the Pane that will pop this Page off the Navigation stack
**/

Page {
    id: sidePane
    property real margins: 40
    content: Container {
        background: Color.create (0.3, 0.3, 0.3)
        
        Label {
            text: "Hello Side Pane"
            textStyle {
                color: Color.White
            }
        }
    }
    
    paneProperties: NavigationPaneProperties {
        backButton: ActionItem {
            title: "Back"
            onTriggered: { 
                // _navPane is set in code.
                _navPane.popAndDelete ();
            }
        }
    }

}

