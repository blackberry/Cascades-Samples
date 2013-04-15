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
var map;
var markers;

function initMaps() {
    console.log("init OpenLayers");
    var myLat = 43.449766;
    var myLong = -80.406096;

    map = new OpenLayers.Map('map_canvas');
    layer = new OpenLayers.Layer.OSM("Simple OSM Map");
    map.addLayer(layer);
    map.setCenter(new OpenLayers.LonLat(myLong, myLat).transform(
            new OpenLayers.Projection("EPSG:4326"), map.getProjectionObject()),
            14);

    map.events.register("move", map, centerChanged);

    OpenLayers.Control.Click = OpenLayers.Class(OpenLayers.Control, {
        defaultHandlerOptions : {
            'single' : true,
            'double' : false,
            'pixelTolerance' : 0,
            'stopSingle' : false,
            'stopDouble' : false
        },

        initialize : function(options) {
            this.handlerOptions = OpenLayers.Util.extend({},
                    this.defaultHandlerOptions);
            OpenLayers.Control.prototype.initialize.apply(this, arguments);
            this.handler = new OpenLayers.Handler.Click(this, {
                'click' : this.trigger
            }, this.handlerOptions);
        },

        trigger : clicked

    });

    var click = new OpenLayers.Control.Click();
    map.addControl(click);
    click.activate();

    markers = new OpenLayers.Layer.Markers( "Markers" );
    map.addLayer(markers);

    console.log("Done init OpenLayers");
}

function clicked(e) {
    var location = map.getLonLatFromPixel(e.xy);
    navigator.cascades.postMessage("clicked:" + location.lat + ","
            + location.lon + "," + e.xy.x + ","
            + e.xy.y);
}

function centerChanged() {
    var location = map.getCenter();
    navigator.cascades.postMessage("centerChanged:" + location.lat + ","
            + location.lon + "," + map.getPixelFromLonLat(location).x + ","
            + map.getPixelFromLonLat(location).y);

}

function onMessage(message) {

}

function setZoom(zoomLevel) {
    map.zoomTo(zoomLevel);
}

function zoomIn() {
    map.zoomIn();
}

function zoomOut() {
    map.zoomOut();
}

function setCenter(lat, lon) {
    var lonlat = new OpenLayers.LonLat(lon, lat);
    lonlat.transform(new OpenLayers.Projection("EPSG:4326"), map
            .getProjectionObject());
    map.setCenter(lonlat);
}

function setMapTypeId(mapType) {
}

// create a marker / push-pin
function createPushPin(lat, lon, title, iconpath) {
    var size = new OpenLayers.Size(60,60);
    var offset = new OpenLayers.Pixel(20,60);
    var icon = new OpenLayers.Icon(iconpath,size,offset);
    var marker = new OpenLayers.Marker(new OpenLayers.LonLat(lon,lat), icon);
    marker.events.register("click", marker, markerClicked);
    markers.addMarker(marker);
}

function removeAllPins() {
    markers.clearMarkers();
}

function markerClicked(e) {
    window.alert("Hello");
    var pinLoc = map.getLonLatFromPixel(e.xy);
    //pinLoc.transform(new OpenLayers.Projection("EPSG:4326"), map.getProjectionObject());
    navigator.cascades.postMessage("markerClicked:" + pinLoc.lat + "," + pinLoc.lon + "," + e.xy.x + "," + e.xy.y);

}
