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
import "WeatherPages"

// The different pages in the weather guesser is added to a TabbedPane.
TabbedPane {
    id: mainTab
    showTabsOnActionBar: true

    // The content of a Tab can either be added in-line or by using delegates. If in-line
    // content is used the Tabs will all be loaded and added to the UI at same time. Delegates
    // gives more flexibility to when it comes to how the content of Tab is loaded, which
    // can be important when it comes to both start up time and memory handling.
    Tab {
        title: qsTr("Home") + Retranslate.onLanguageChanged
        imageSource: "asset:///images/menuicons/icon_home.png"

        // The first tab is activated immediately and always kept it loaded, it is the home tab and
        // considered to be the most important tab which the user visit frequently.
        // Note that this is basically the same as just adding it as an in-line component directly to the tabs
        // default content property. We do it like this for consistency with the rest of the app structure.
        delegateActivationPolicy: TabDelegateActivationPolicy.ActivateImmediately

        delegate: Delegate {
            id: weatherPageDelegate

            // This delegate use the default component property to set up the object that it should handle.
            HomeWeatherPage {
                id: homeCityPage
                city: _appSettings.home
            }
        }

        onTriggered: {
            // Check if the delegates object is defined, if so trigger a function to reset
            // the list in the weather page (the object) to the top position.
            if (weatherPageDelegate.object != undefined) {
                weatherPageDelegate.object.resetToTop();
            }
        }
    }

    // The city browse page where filtering is done based on continents.
    Tab {
        title: qsTr("Browse Cities") + Retranslate.onLanguageChanged
        imageSource: "asset:///images/menuicons/icon_browse.png"

        // Default delegateActivationPolicy explicitly stated for clarity only, the delegates object
        // (the ContinentsTab) will be created/destroyed when the tab is selected/unselected.
        delegateActivationPolicy: TabDelegateActivationPolicy.Default

        delegate: Delegate {
            source: "ContinentsTab.qml"
        }
    }

    // A tab where a list of favorite cities are shown
    Tab {
        title: qsTr("Favorites") + Retranslate.onLanguageChanged
        imageSource: "asset:///images/menuicons/icon_favorites.png"

        // The favorites tab is a Tab that we want to load quickly when returning to the
        // tab so we set it to activate the first time it is selected.
        delegateActivationPolicy: TabDelegateActivationPolicy.ActivateWhenSelected

        delegate: Delegate {
            id: favDelegate
            source: "FavoriteTab.qml"
        }

        onTriggered: {
            // We refresh the favorite list every time we visit the tab since we
            // do not know if new items have been added, the operation is cheap in this
            // case since nothing will happen unless the revision has changed.
            if (favDelegate.object != undefined) {
                favDelegate.object.refreshFavModel();
            }
        }
    }

    // A tab where the maximum and minimum temperatures are shown
    Tab {
        title: qsTr("Max/Min") + Retranslate.onLanguageChanged
        imageSource: "asset:///images/menuicons/icon_maxmin.png"
        
        delegate: Delegate {
            source: "MaxMinTab.qml"
        }
    }

    // An information page presenting some background for the application
    Tab {
        title: qsTr("Info") + Retranslate.onLanguageChanged
        imageSource: "asset:///images/menuicons/icon_info.png"
        
        delegate: Delegate {
            source: "InfoTab.qml"
        }
    }
}
