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
import "Common"
RecipePage {
    RecipeContainer {
        id: themeContainer
        property bool brightThemeSelected: (Application.themeSupport.theme.colorTheme.style == VisualStyle.Bright)?true:false;
        
        layout: StackLayout {
        }
        
        Header {
            title: "Theme"
        }
        
        Container {        		
            leftPadding: ui.du(2)
            rightPadding: ui.du(2)
            topPadding: ui.du(2)
            bottomPadding: ui.du(2)
            
            DropDown {                
                title: "Theme selection"
                
                options: [
                    Option {
                        value: VisualStyle.Dark
                        text: "Dark Theme"
                        selected: !themeContainer.brightThemeSelected
                    },
                    Option {
                        value: VisualStyle.Bright
                        text: "Bright Theme"
                        selected: themeContainer.brightThemeSelected
                    }
                ]
                
                onSelectedOptionChanged: {
                    // Set the theme when selection changes.
                    Application.themeSupport.setVisualStyle(selectedOption.value);
                }
            }
        }
    }
}
