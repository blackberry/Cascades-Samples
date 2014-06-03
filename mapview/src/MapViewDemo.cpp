/* Copyright (c) 2012, 2013  BlackBerry Limited.
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
#include "MapViewDemo.hpp"

#include <bb/cascades/AbstractPane>
#include <bb/cascades/Application>
#include <bb/cascades/Container>
#include <bb/cascades/maps/MapView>
#include <bb/cascades/maps/MapData>
#include <bb/cascades/maps/DataProvider>
#include <bb/cascades/QmlDocument>
#include <bb/platform/geo/Point>
#include <bb/platform/geo/GeoLocation>
#include <bb/platform/geo/Marker>
#include <bb/UIToolkitSupport>

using namespace bb;
using namespace bb::cascades;
using namespace bb::cascades::maps;
using namespace bb::platform::geo;

//! [0]
MapViewDemo::MapViewDemo()
    : QObject()
{
    // create scene document from main.qml asset
    // set parent to created document to ensure it exists for the whole application lifetime
    QmlDocument *qml = QmlDocument::create("asset:///main.qml").parent(this);
    qml->setContextProperty("_mapViewTest", this);

    // create root object for the UI
    AbstractPane *root = qml->createRootObject<AbstractPane>();

    QObject* mapViewAsQObject = root->findChild<QObject*>(QString("mapViewObj"));
    if (mapViewAsQObject) {
        mapView = qobject_cast<bb::cascades::maps::MapView*>(mapViewAsQObject);
        mapView->setCaptionGoButtonVisible(true);
        if (mapView) {
            // creating a data provider just for the device location object. that way, when the clear function is call, this object is not removed.
            DataProvider* deviceLocDataProv = new DataProvider("device-location-data-provider");
            mapView->mapData()->addProvider(deviceLocDataProv);

            // create a geolocation just for the device's location
            deviceLocation = new GeoLocation("device-location-id");
            deviceLocation->setName("Current Device Location");
            deviceLocation->setDescription("<html><a href=\"http://www.blackberry.com\">Hyperlinks</a> are super useful in bubbles.</html>");

            // for that location, replace the standard default pin with the provided bulls eye asset
            Marker bullseye = Marker(UIToolkitSupport::absolutePathFromUrl(
                                QUrl("asset:///images/me.png")), QSize(60, 60),
                                QPoint(29, 29), QPoint(29, 1));
            deviceLocation->setMarker(bullseye);

            deviceLocDataProv->add(deviceLocation);
        }
    }

    // set created root object as a scene
    bb::cascades::Application::instance()->setScene(root);
}
//! [0]
//! [1]
void MapViewDemo::addPinAtCurrentMapCenter() {
    if (mapView) {
        GeoLocation* newDrop = new GeoLocation();
        newDrop->setLatitude(mapView->latitude());
        newDrop->setLongitude(mapView->longitude());
        QString desc = QString("Coordinates: %1, %2").arg(mapView->latitude(),
                                0, 'f', 3).arg(mapView->longitude(), 0, 'f', 3);
        newDrop->setName("Dropped Pin");
        newDrop->setDescription(desc);

        // use the marker in the assets, as opposed to the default marker
        Marker flag;
        flag.setIconUri(UIToolkitSupport::absolutePathFromUrl(
                        QUrl("asset:///images/on_map_pin.png")));
        flag.setIconSize(QSize(60, 60));
        flag.setLocationCoordinate(QPoint(20, 59));
        flag.setCaptionTailCoordinate(QPoint(20, 1));
        newDrop->setMarker(flag);

        mapView->mapData()->add(newDrop);
    }
}
//! [1]
//! [2]
void MapViewDemo::clearPins() {
    if (mapView) {
        // this will remove all pins, except the "device location" pin, as it's in a different data provider
        mapView->mapData()->defaultProvider()->clear();
    }
}
//! [2]
void MapViewDemo::updateDeviceLocation(double lat, double lon) {
    qDebug() << "MapViewDemo::updateDeviceLocation( " << lat << ", " << lon
            << " )";
    if (deviceLocation) {
        deviceLocation->setLatitude(lat);
        deviceLocation->setLongitude(lon);
    }
    mapView->setLatitude(lat);
    mapView->setLongitude(lon);
}
