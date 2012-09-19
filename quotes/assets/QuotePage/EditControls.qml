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

// This is the controls for canceling and updating a quote that is in edit mode.

Container {
    id: editControls
    property bool updateEnabled: false
    signal update()
    signal cancel()
    bottomPadding: 40
    
    layout: StackLayout {
        orientation: LayoutOrientation.LeftToRight
    }
    
    Button {
        text: "Cancel"
        
        layoutProperties: StackLayoutProperties {
            spaceQuota: 1
        }
        
        onClicked: {
            editControls.cancel();
        }
    }
    
    Label {
        id: editLabel
        text: "Edit"
        horizontalAlignment: HorizontalAlignment.Center
        verticalAlignment: VerticalAlignment.Center

        layoutProperties: StackLayoutProperties {
            spaceQuota: 1
        }

        textStyle.base: quoteStyleLightBody.style
    }
    
    Button {
        id: updateButton
        text: "Update"
        enabled: updateEnabled
        
        layoutProperties: StackLayoutProperties {
            spaceQuota: 1
        }
        
        onClicked: {
            editControls.update();
        }
    }
}// Container
