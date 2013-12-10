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
import bb.cascades 1.0
import bb.data 1.0

TabbedPane {
    id: init

    property variant currentTab: false

    attachedObjects: [
        ComponentDefinition {
            id: tabDef

            TemplateTabView {
                enabled: true
            }
        }
    ] // attachedObjects

    onActiveTabChanged: {
        
        // if user has navigated away from initial "tab url", change location back
        var currentUrl = activeTab.webcontainer.url;
        var avtiveTabTitle = activeTab.title;
        var allTabs = appSettings.tabs;
        var allTabsLength = allTabs.length;

        for (var i = 0; i < allTabsLength; i ++) {
            if (activeTab.title === allTabs[i].name) {
                if (allTabs[i].url != currentUrl) {
                    console.log('-- not at original URL, change back!')
                    activeTab.webcontainer.url = activeTab.template.url;
                }
            }
        }
        
    } // onActiveTabChanged

    onCreationCompleted: {

        // set the orientation
        var orientation = appSettings.settings.orientation;
        if (orientation === 'portrait') {
            OrientationSupport.supportedDisplayOrientation = SupportedDisplayOrientation.DisplayPortrait;
        } else if (orientation === 'landscape') {
            OrientationSupport.supportedDisplayOrientation = SupportedDisplayOrientation.DisplayLandscape;
        } else {
            OrientationSupport.supportedDisplayOrientation = SupportedDisplayOrientation.All;
        }

        // add the tabs from our settings.json file
        for (var i = 0; i < appSettings.tabs.length; i ++) {
            var templateTab = tabDef.createObject();
            templateTab.template = appSettings.tabs[i];
            init.add(templateTab);
        }
    } // onCreationCompleted
} // Tabbedpane