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

#include "MapViewDemo.hpp"

#include <bb/cascades/AbstractPane>
#include <bb/cascades/Application>
#include <bb/cascades/QmlDocument>
#include <bb/cascades/maps/MapView>
#include <bb/cascades/maps/ViewProperties>
#include <bb/platform/geo/BoundingBox.hpp>

using namespace bb::cascades;
using namespace bb::cascades::maps;
using namespace bb::platform::geo;

MapViewDemo::MapViewDemo(bb::cascades::Application *app)
    : QObject(app)
{
    // create scene document from main.qml asset
    // set parent to created document to ensure it exists for the whole application lifetime
    QmlDocument *qml = QmlDocument::create("asset:///main.qml").parent(this);
    qml->setContextProperty("_mapViewTest", this);

    // create root object for the UI
    AbstractPane *root = qml->createRootObject<AbstractPane>();
    // set created root object as a scene
    app->setScene(root);
}

int MapViewDemo::longitudeToX(QObject* mapObject, double longitude) const
{
    const MapView* mapView = qobject_cast<MapView*>(mapObject);
    if (!mapView)
        return 0;

    // retrieve the view properties
    const ViewProperties viewProperties = mapView->viewProperties();

    const QSize windowSize = viewProperties.windowSize();
    const BoundingBox worldBox = viewProperties.boundingBox();

    const double longitudePositionPercent = (longitude - worldBox.left())
                                            / (worldBox.right() - worldBox.left());
    return (qFloor((windowSize.width() * longitudePositionPercent)));
}

int MapViewDemo::latitudeToY(QObject* mapObject, double latitude) const
{
    const MapView* mapView = qobject_cast<MapView*>(mapObject);
    if (!mapView)
        return 0;

    // retrieve the view properties
    const ViewProperties viewProperties = mapView->viewProperties();

    const QSize windowSize = viewProperties.windowSize();
    const BoundingBox worldBox = viewProperties.boundingBox();

    const double verticalHeightDegrees = worldBox.top() - worldBox.bottom();
    const double locationHeight = latitude - worldBox.bottom();
    double latitudePositionPercent = locationHeight / verticalHeightDegrees;

    // invert percent as pixels start from top
    latitudePositionPercent = 1 - latitudePositionPercent;

    // convert percent to pixel
    const double pixel = (windowSize.height() * latitudePositionPercent);
    return (qFloor(pixel));
}

int MapViewDemo::worldToWindow(QObject* mapObject, double latitude) const
{
    const double longitude = -80.519603;

    const MapView* mapView = qobject_cast<MapView*>(mapObject);
    if (!mapView)
        return 0;

    // retrieve the view properties
    const ViewProperties viewProperties = mapView->viewProperties();

    const QSize windowSize = viewProperties.windowSize();
    const BoundingBox worldBox = viewProperties.boundingBox();

    const double verticalHeightDegrees = worldBox.top() - worldBox.bottom();
    const double locationHeight = latitude - worldBox.bottom();
    double latitudePositionPercent = locationHeight / verticalHeightDegrees;

    // invert percent as pixels start from top
    latitudePositionPercent = 1 - latitudePositionPercent;

    // convert percent to pixels
    QPoint windowPos;
    windowPos.setY(windowSize.height() * latitudePositionPercent);

    const double longitudePositionPercent = (longitude - worldBox.left())
                                            / (worldBox.right() - worldBox.left());
    windowPos.setX(windowSize.width() * longitudePositionPercent);

    return windowPos.x();
}
