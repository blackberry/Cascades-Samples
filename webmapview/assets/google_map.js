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
var markersArray = [];

//! [0]
function initMaps() {
    console.log("init GoogleMaps");
    var myLat = 43.449766;
    var myLong = -80.406096;
    var myLocation = new google.maps.LatLng(myLat, myLong);

    var mapOptions = {
        zoom : 14,
        center : myLocation,
        mapTypeId : google.maps.MapTypeId.HYBRID,
        mapTypeControl : false,
        zoomControl : false,
        streetViewControl : false,
        styles : [ {
            featureType : "poi",
            elementType : "labels",
            stylers : [ {
                visibility : "off"
            } ]
        } ]
    };
    googleMap = new google.maps.Map(document.getElementById("map_canvas"),
            mapOptions);

    google.maps.event.addListener(googleMap, 'click', clicked);
    google.maps.event.addListener(googleMap, 'center_changed', centerChanged);

    console.log("Done init GoogleMaps");
}
//! [0]

//! [1]
function clicked(event) {
    var point = convertPoint(event.latLng);
    navigator.cascades.postMessage("clicked:" + event.latLng.lat() + "," + event.latLng.lng() + "," + point.x + "," + point.y);
}

function centerChanged() {
    var point = convertPoint(googleMap.getCenter());
    navigator.cascades.postMessage("centerChanged:" + googleMap.getCenter().lat() + "," + googleMap.getCenter().lng() + "," + point.x + "," + point.y);

}
//! [1]

function onMessage(message) {

}

function setZoom(zoom) {
    googleMap.setZoom(zoom);
}

//! [2]
function zoomIn() {
    googleMap.setZoom(googleMap.getZoom()+1);
}

function zoomOut() {
    googleMap.setZoom(googleMap.getZoom()-1);
}
//! [2]

function setCenter(lat, lon) {
    var center = new google.maps.LatLng(lat, lon);
    googleMap.setCenter(center);
}

function setMapTypeId(mapType) {
    googleMap.setMapTypeId(mapType);
}

// create a marker / push-pin
function createPushPin(lat, lon, title, icon) {
    var placeLoc = new google.maps.LatLng(lat, lon);
    var marker = new google.maps.Marker({
        map : googleMap,
        position : placeLoc,
        draggable : true,
        animation : google.maps.Animation.DROP,
        icon : icon
    });
    google.maps.event.addListener(marker, 'click', markerClicked);
    markersArray.push(marker);
}

function removeAllPins() {
    if (markersArray) {
        for (i in markersArray) {
            markersArray[i].setMap(null);
        }
    }
}

function markerClicked(event) {
    var point = convertPoint(event.latLng);
    navigator.cascades.postMessage("markerClicked:" + event.latLng.lat() + "," + event.latLng.lng() + "," + point.x + "," + point.y);
}

function convertPoint(latLng) {
    var topRight=googleMap.getProjection().fromLatLngToPoint(googleMap.getBounds().getNorthEast());
    var bottomLeft=googleMap.getProjection().fromLatLngToPoint(googleMap.getBounds().getSouthWest());
    var scale=Math.pow(2,googleMap.getZoom());
    var worldPoint=googleMap.getProjection().fromLatLngToPoint(latLng);
    return new google.maps.Point((worldPoint.x- bottomLeft.x)*scale,(worldPoint.y-topRight.y)*scale);
}
