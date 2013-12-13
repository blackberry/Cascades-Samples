/* Copyright (c) 2013 BlackBerry Limited.
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

// A Topic Item presenting a subject and an list of addresses.
Container {
    id: topicItem
    property alias topic: topicLabel.text
    property alias adressList: adressListLabel.text
    
    topPadding: 20
    rightPadding: 20
    leftPadding: 20

    Label {
        id: topicLabel
        textStyle.base: SystemDefaults.TextStyles.BigText
        multiline: true
        accessibilityMode: A11yMode.Collapsed
    }
    
    Label {
        id: adressListLabel
        multiline: true
        accessibilityMode: A11yMode.Collapsed
    }
    
    Divider {
    }
    
    // The item uses a CustomA11yObject on the Container so it can be read by 
    // the Screen Reader as one object only used when the item is used as a ListItemComponent.
    accessibility: CustomA11yObject {
        role: A11yRole.ListItem
        name: topicLabel.text + ", " + adressListLabel.text
        
        ComponentA11ySpecialization {
            onActivationRequested: {
                // When a list item is activated send the trigger signal from the list view.
                if (event.type == A11yComponentActivationType.Release) {
                    topicItem.ListItem.view.triggered(item.ListItem.indexPath);
                }
            }
        }
    }
}
