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

// The controls for canceling and updating a quote that is in edit mode.
Container {
    id: editControls
    property bool updateEnabled: false
    
    signal update();
    signal cancel();
    
    layout: StackLayout {
        layoutDirection: LayoutDirection.LeftToRight
        bottomPadding: 40
    }
    Button {
        text: "Cancel"
        onClicked: {
            editControls.cancel();
        }
        layoutProperties: StackLayoutProperties {
            spaceQuota: 1
        }
    }
    Label {
        id: editLabel
        text: "Edit"
        textStyle {
            base: quoteText.style
            alignment: TextAlignment.Center
            color: Color.create ("#fafafa")
        }
        layoutProperties: StackLayoutProperties {
            horizontalAlignment: HorizontalAlignment.Fill
            verticalAlignment: VerticalAlignment.Center
            spaceQuota: 1
        }
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
}
