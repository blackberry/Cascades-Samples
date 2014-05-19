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

Container {
    id: colorSwatch
    // The properties that can be set on the colorSwatch component.
    property alias swatchColor: container_id.background
    
    leftPadding: ui.du(0.5)
    rightPadding: ui.du(0.5)
    topPadding: ui.du(0.25)
    bottomPadding: topPadding;
    
    onBackgroundChanged: {
        // If the background paint changes, increase the spacing around the swatch to make the background more visible.
        topPadding = ui.du(0.5);    
    }
    
    

    layout: StackLayout {
        orientation: LayoutOrientation.LeftToRight

    }

    // The colored Label
    Container {
        verticalAlignment: VerticalAlignment.Center
        layoutProperties: StackLayoutProperties {
            spaceQuota: 5
        }
        Label {
            id: text_part
            text: Color.toHexString(swatchColor).toUpperCase()

            textStyle {
                base: SystemDefaults.TextStyles.BodyText
                fontWeight: FontWeight.Bold
                // Set the color on a Label by the color attribute.
                color: swatchColor
            }
        }
    }
    
    // Set up the colored Container (rectangle). SpaceQuota is used so that this container is counted
    // as less space and the layout system will clip it rather than shrink the label width.
    Container {
        id: container_id
        verticalAlignment: VerticalAlignment.Fill
        layoutProperties: StackLayoutProperties {
            spaceQuota: 11
        }

        // Set the Color on a Container by setting the background attribute.
        background: Color.Black
    }
}
