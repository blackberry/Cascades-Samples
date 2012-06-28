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

Container {
    id: colorHelper
    // The properties that can be set on the ColorHelper component.
    property alias colortext: text_part.text
    property alias containerColor: container_id.background

    layout: StackLayout {
        layoutDirection: LayoutDirection.LeftToRight;
        topPadding: 4
    }

    // The colored Label.
    Label {
        id: text_part
        layoutProperties: StackLayoutProperties {
            verticalAlignment: VerticalAlignment.Center
        }
        text: "text"

        textStyle {
            base: SystemDefaults.TextStyles.BodyText
            fontWeight: FontWeight.Bold
            // Setting color on a Label is done by the color attribute.
            color: container_id.background
        }
    }

    // The colored Container (rectangle). SpaceQuota is used so that this container is counted
    // less and the layout system will clip it rather then shrink the label width.
    Container {
        id: container_id
        layoutProperties: StackLayoutProperties {
            verticalAlignment: VerticalAlignment.Fill
            spaceQuota: 1
        }
        leftMargin: 5;

        // Setting the Color on a Container is done by setting it on the background attribute.
        background: Color.Black
    }
}
