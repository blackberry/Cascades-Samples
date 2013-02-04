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
import "WeatherPage"

TabbedPane {
    id: mainTab
    showTabsOnActionBar: true
    
    // The home weather page that the user can select which city that they want to show
    // here by long pressing a city in the list and setting it as home.
    Tab {
        title: "Home"
        imageSource: "asset:///images/menuicons/icon_home.png"
        
        WeatherPage {
            id: homeCityPage
            
            // The data model and the city property of the home weather is kept in the
            // _homeModel which is created and bound in C++.
            weatherData: _homeModel;
            city: _homeModel.city;
        }
        onTriggered: {
            homeCityPage.resetToTop();
        }
    }

    // The city browse page where filtering is done based on continents
    Tab {
        title: "Browse Cities"
        imageSource: "asset:///images/menuicons/icon_browse.png"
        ContinentsPage {
            id: continents
        }
    }

    // A tab where a list of favorite cities are shown
    Tab {
        title: "Favorites"
        imageSource: "asset:///images/menuicons/icon_favorites.png"
        FavoritePage {
            id: favorites
        }
    }

    // A tab where the maximum and minimum temperatures are shown
    Tab {
        title: "Max/Min"
        imageSource: "asset:///images/menuicons/icon_maxmin.png"
        MaxMinPage {
        }
    }

    // An information page presenting some background for the application
    Tab {
        title: "Info"
        imageSource: "asset:///images/menuicons/icon_info.png"
        InfoPage {
        }
    }
    
    attachedObjects: [
        // Non UI objects are specified as attached objects
        // When modifying the SystemDefult fonts, like changing weight or color, 
        // text styles can be accessed either directly in the control or as attached objects.
        TextStyleDefinition {
            id: bigTextNormal
            base: SystemDefaults.TextStyles.BigText
            fontWeight: FontWeight.Normal
        },
        TextStyleDefinition {
            id: bigTextNormalRed
            base: SystemDefaults.TextStyles.BigText
            fontWeight: FontWeight.Normal
            color: Color.create("#d8225E")
        },
        TextStyleDefinition {
            id: bigTextNormalBlue
            base: SystemDefaults.TextStyles.BigText
            fontWeight: FontWeight.Normal
            color: Color.create("#006dba")
        },
        TextStyleDefinition {
            id: titleTextItalicWhite
            base: SystemDefaults.TextStyles.TitleText
            fontStyle: FontStyle.Italic
            color: Color.White
        },
        TextStyleDefinition {
            id: smallTextWhite
            base: SystemDefaults.TextStyles.SmallText
            color: Color.White
        }
    ]
}// TabbedPane
