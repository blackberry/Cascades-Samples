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
import bb.cascades 1.2

TabbedPane {
    
    Menu.definition: MenuDefinition {
        // Add a Help action
        helpAction: HelpActionItem {
            onTriggered: {
                //ToDo
            }
        }
        
        // Add any remaining actions
        actions: [
            ActionItem {
                title: "Invite To Download"
                enabled: bbmHandler.allowed
                onTriggered: {
                    bbmHandler.sendInvite();
                }
            }
        
        ]
        
        // Add a Settings action
        settingsAction: SettingsActionItem {
            onTriggered: {
                //ToDo 
            }
        }
    }
    
    showTabsOnActionBar: true
    Tab {
        title: "My Feeds"
        imageSource: "asset:///images/home.png"
        delegate: Delegate {
            Feeds {
                dataModelSource: "models/myFeeds.xml"
            }
        }
    }
    Tab {
        title: "News"
        imageSource: "asset:///images/news.png"
        delegate: Delegate {
            Feeds {
                dataModelSource: "models/news.xml"
            }
        }
    }
    Tab {
        title: "Tech"
        delegate: Delegate {
            Feeds {
                dataModelSource: "models/tech.xml"
            }
        }
    }
    Tab {
        title: "Sports"
        imageSource: "asset:///images/sports.png"
        delegate: Delegate {
            Feeds {
                dataModelSource: "models/sports.xml"
            }
        }
    
    }
}
