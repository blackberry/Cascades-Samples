/*
 * Copyright 2010-2011 Research In Motion Limited.
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

// initialize the map
var googleMap;
var bingMap;
var markersArray = [];

function initMaps() {
    console.log("init BingMaps");
    var myLat = 43.449766;
    var myLong = -80.406096;


    bingMap = new Microsoft.Maps.Map(document.getElementById("map_canvas"), {
        credentials : 'Aq19BSd2VjCXz6tNKIolUJs-jyrrWdSHSealOdlKq7jb18l1OSZWzPk3ae2qA5ac',
        center : new Microsoft.Maps.Location(myLat, myLong),
        zoom : 14,
        mapTypeId : Microsoft.Maps.MapTypeId.auto,
        enableClickableLogo: false,
        enableSearchLogo: false,
        showDashboard: false,
        showMapTypeSelector: false,
        showScalebar: false,
        showCopyright : false
    });

    Microsoft.Maps.Events.addHandler(bingMap, 'viewchange', centerChanged);
    Microsoft.Maps.Events.addHandler(bingMap, 'click', clicked);
    console.log("Done init BingMaps");
}

function clicked(e) {
    var location = bingMap.tryPixelToLocation(new Microsoft.Maps.Point(e.getX(), e.getY()), Microsoft.Maps.PixelReference.page);
    navigator.cascades.postMessage("clicked:" + location.latitude + "," + location.longitude + "," + e.pageX + "," + e.pageY);
}

function centerChanged() {
    var point = bingMap.tryLocationToPixel(bingMap.getCenter(), Microsoft.Maps.PixelReference.page);
    navigator.cascades.postMessage("centerChanged:" + bingMap.getCenter().latitude + "," + bingMap.getCenter().longitude + "," + point.x + "," + point.y);

}

function onMessage(message) {

}

function setZoom(zoomLevel) {
    var options = bingMap.getOptions();
    options.zoom = zoomLevel;
    bingMap.setView(options);
}

function zoomIn() {
    setZoom(bingMap.getZoom() + 1);
}

function zoomOut() {
    setZoom(bingMap.getZoom() - 1);
}

function setCenter(lat, lon) {
    bingMap.setView({center:new Microsoft.Maps.Location(lat, lon)});
}

function setMapTypeId(mapType) {
    var options = bingMap.getOptions();
    options.mapTypeId = mapType;
    bingMap.setView(options);
}

// create a marker / push-pin
function createPushPin(lat, lon, title, iconpath) {
    var pin = new Microsoft.Maps.Pushpin(new Microsoft.Maps.Location(lat, lon), {icon:iconpath, height:60, width:60, anchor:new Microsoft.Maps.Point(20,58), draggable: true});
    bingMap.entities.push(pin);
    Microsoft.Maps.Events.addHandler(pin, 'click', markerClicked);
    markersArray.push(pin);
}

function removeAllPins() {
    if (markersArray) {
        for (i in markersArray) {
            bingMap.entities.remove(markersArray[i])
        }
    }
}

function markerClicked(e) {
    var pinLoc = e.target.getLocation();
    var point = bingMap.tryLocationToPixel(pinLoc, Microsoft.Maps.PixelReference.page);
    navigator.cascades.postMessage("markerClicked:" + pinLoc.latitude + "," + pinLoc.longitude + "," + point.x + "," + point.y);
}
