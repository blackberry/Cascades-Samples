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
 
  /*
  This component is a text component used by the feedlist
  */
 
import bb.cascades 1.2


Container {
    leftPadding: 20
    topPadding: leftPadding
    rightPadding: leftPadding
    bottomPadding: leftPadding
    verticalAlignment: VerticalAlignment.Fill
    horizontalAlignment: HorizontalAlignment.Fill
    
    Label {
        id: titleLabel
        text: ListItemData.title
        textStyle.base: SystemDefaults.TextStyles.TitleText
        multiline: true
        accessibilityMode: A11yMode.Collapsed
        
    }
    Label {
        // We render as HTML to ensure we don't display gibberish.
        id: descriptionLabel
        text: "<html><div>" + ListItemData.description + "</div></html>"
        textStyle.base: SystemDefaults.TextStyles.BodyText
        multiline: true
    }
    Label {
        id: writerLabel 
        text: (ListItemData)["dc:creator"]
        textStyle.base: SystemDefaults.TextStyles.SmallText
        horizontalAlignment: HorizontalAlignment.Right
        accessibilityMode: A11yMode.Collapsed
    }
    Divider {
    }
    
    accessibility: CustomA11yObject {
        role: A11yRole.ListItem
        labelledBy: titleLabel
        description: "Written by" + writerLabel.text
    }
}
