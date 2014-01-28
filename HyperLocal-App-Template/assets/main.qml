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
    property variant phoneNumbersArray
    property variant foursquareArray
    property variant locationsArray
    property variant locationHandler
    property variant listViewArray
    property variant currentLat
    property variant currentLng

    property TemplateTabView oldTab: null

    attachedObjects: [
        ComponentDefinition {
            id: tabDef

            TemplateTabView {
                enabled: true
            }
        },

        /* Used for timing our page reloads. */
        FadeTransition {
            id: delay
            target: init.activePane.top.content
            delay: 500
            duration: 0
            property TemplateTabView oldTab: null
            onEnded: {
                /* Reset the tab we are leaving after a slight delay (background reload.) */
                if (oldTab) {
                    if (oldTab.webcontainer.url != oldTab.template.url)
                        oldTab.webcontainer.url = oldTab.template.url;
                }
            }
        }
    ] // attachedObjects

    onActiveTabChanged: {
        /* Reset the current URL if it has not been reset yet. Only needed on first load. */
        
        if (activeTab.webcontainer.url != activeTab.template.url)
            activeTab.webcontainer.url = activeTab.template.url;

        /* Store our previous tab. */
        delay.oldTab = oldTab;

        /* Trigger our fade animation. */
        delay.play();

        /* Keep track of the new tab. */
        oldTab = activeTab;

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

        /* --------------------------------------------------------
         *  MULTI VENUE HANDLING SETUP
         * --------------------------------------------------------
         */

		// --- locationHandler ---
        // 1 = invoke
        // 2 = display listview

        // locations
        var locationsArray = [];

        // how many phone numbers do we have in the locations array?
        for (var i = 0; i < appSettings.locations.length; ++ i) {
            var venueInfo = {
                title: appSettings.locations[i].name,
                description: appSettings.locations[i].address,
                status: "",
                address: appSettings.locations[i].address,
                coords: {
                    lat: appSettings.locations[i].coords.lat,
                    lng: appSettings.locations[i].coords.lng
                }
            };
            locationsArray.push(venueInfo);
        }
        init.locationsArray = locationsArray;
        var locationsArrayLength = locationsArray.length;

        // if only 1 number, just call it
        if (locationsArrayLength === 1) {
            init.locationHandler = 1;

            // 1 - 3 numbers, use dialog
        } else {
            init.locationHandler = 2;
        }

        // phone
        var phoneNumbersArray = [];

        // how many phone numbers do we have in the locations array?
        for (var i = 0; i < appSettings.locations.length; ++ i) {
            if (appSettings.locations[i].phone !== "") {
                var venueInfo = {
                    title: appSettings.locations[i].name,
                    description: appSettings.locations[i].phone,
                    status: "",
                    phone: appSettings.locations[i].phone,
                    coords: {
                        lat: appSettings.locations[i].coords.lat,
                        lng: appSettings.locations[i].coords.lng
                    }
                };
                phoneNumbersArray.push(venueInfo);
            }
        }
        init.phoneNumbersArray = phoneNumbersArray;
        var phoneNumbersArrayLength = init.phoneNumbersArray.length;

        // if only 1 number, just call it
        if (phoneNumbersArrayLength === 1) {
            init.locationHandler = 1;

            // 1 - 3 numbers, use dialog
        } else {
            init.locationHandler = 2;
        }

        // foursquare
        var foursquareArray = [];

        // how many foursquare numbers do we have in the locations array?
        for (var i = 0; i < appSettings.locations.length; ++ i) {
            if (appSettings.locations[i].foursquare !== "") {

                var venueInfo = {
                    title: appSettings.locations[i].name,
                    description: appSettings.locations[i].address,
                    status: "",
                    foursquare: appSettings.locations[i].foursquare,
                    coords: {
                        lat: appSettings.locations[i].coords.lat,
                        lng: appSettings.locations[i].coords.lng
                    }
                };

                foursquareArray.push(venueInfo);
            }
        }
        init.foursquareArray = foursquareArray;
        var foursquareArrayLength = init.foursquareArray.length;

        // if only 1 number, just call it
        if (foursquareArrayLength === 1) {
            init.locationHandler = 1;

            // 1 - 3 numbers, use dialog
        } else {
            init.locationHandler = 2;
        }

        app.newCoords.connect(gotNewCoords);
        app.getCurrentLocation();

    } // onCreationCompleted

    function gotNewCoords(lat, lng) {
        currentLat = lat;
        currentLng = lng;

        // update location array
        var newArray = [];
        newArray = Array.prototype.slice.apply(init.locationsArray);

        for (var i = 0; i < newArray.length; ++ i) {
            if (newArray[i].status === "") {
                var distance = getDistanceToVenue(newArray[i].coords.lat, newArray[i].coords.lng);
                newArray[i].status = distance;
            }
        }
        init.locationsArray = newArray;

        // update phone Array
        var newArray = [];
        newArray = Array.prototype.slice.apply(init.phoneNumbersArray);

        for (var i = 0; i < newArray.length; ++ i) {
            if (newArray[i].status === "") {
                var distance = getDistanceToVenue(newArray[i].coords.lat, newArray[i].coords.lng);
                newArray[i].status = distance;
            }
        }
        init.phoneNumbersArray = newArray;

        // update foursquare array
        var newArray = [];
        newArray = Array.prototype.slice.apply(init.foursquareArray);

        for (var i = 0; i < newArray.length; ++ i) {
            if (newArray[i].status === "") {
                var distance = getDistanceToVenue(newArray[i].coords.lat, newArray[i].coords.lng);
                newArray[i].status = distance;
            }
        }
        init.foursquareArray = newArray;
    }

	// calculate the distance to the venues
    function getDistanceToVenue(venueLat, venueLng) {
        var R = 6371;
        var dLat = this.deg2rad(venueLat - currentLat);
        var dLon = this.deg2rad(venueLng - currentLng);
        var a = Math.sin(dLat / 2) * Math.sin(dLat / 2) + Math.cos(this.deg2rad(venueLat)) * Math.cos(this.deg2rad(currentLat)) * Math.sin(dLon / 2) * Math.sin(dLon / 2);
        var c = 2 * Math.atan2(Math.sqrt(a), Math.sqrt(1 - a));
        var d = R * c;
        var _d;

        if (d === 0) {
            d = '0ft';

        } else if (d <= 0.001) {
            d = '0ft';

            // < 100m
        } else if (d < 0.1) {
            d = Math.round(d * 1000) / 1000;
            _d = d.toString().split('.0');

            if (_d[1].charAt(0) === '0') {
                _d[1] = _d[1].substring(1);
            }

            d = _d[1] + 'ft';

            // < 1k
        } else if (d < 1) {
            d = Math.round(d * 1000) / 1000;
            _d = d.toString().split('.');
            d = _d[1] + 'm';

            // < 10k
        } else if (d >= 1 && d < 10) {
            d = Math.round(d * 100) / 100;
            d = d.toString() + 'km';

            // far km
        } else if (d >= 1) {
            d = Math.round(d * 10) / 10;
            _d = d.toString().split('.');
            d = _d[0] + 'km';
        }

        return d;
    }

    function deg2rad(deg) {
        return deg * (Math.PI / 180);
    }

}// Tabbedpane
