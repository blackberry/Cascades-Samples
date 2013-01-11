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
#ifndef MapViewDemo_HPP_
#define MapViewDemo_HPP_

#include <bb/cascades/controls/container.h>
#include <bb/cascades/maps/MapView>
#include <bb/cascades/maps/MapData.hpp>
#include <bb/cascades/maps/ViewProperties>
#include <bb/platform/geo/GeoLocation.hpp>
#include <bb/platform/geo/Point.hpp>
#include <bb/platform/geo/BoundingBox.hpp>

#include <QObject>
#include <qmath.h>
#include <qpoint.h>
#include <qsize.h>

namespace bb {
namespace cascades {
class Application;
}
}

using namespace bb::cascades::maps;
using namespace bb::platform::geo;
using namespace bb::cascades;

/*!
 * @brief Application pane object
 *
 * Use this object to create and init app UI, to create context objects, to register the new meta types etc.
 */
class MapViewDemo: public QObject
{
    Q_OBJECT
public:
    MapViewDemo(bb::cascades::Application *app);
    virtual ~MapViewDemo()
    {
    }

    Q_INVOKABLE QString worldToPixelInvokable(QObject* mapObject, double lat, double lon);Q_INVOKABLE
    void updateMarkers(QObject* mapObject, QObject* containerObject);
    QPoint worldToPixel(QObject* mapObject, double lat, double lon);

};

#endif /* MapViewDemo_HPP_ */
