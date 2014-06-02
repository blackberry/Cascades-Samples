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

#ifndef MAPPINGAPP_HPP_
#define MAPPINGAPP_HPP_

#include <bb/platform/geo/Polyline>
#include <bb/platform/geo/Point>

#include <QObject>

class QAbstractAnimation;

namespace bb {
namespace cascades {
class Page;
namespace maps {
class MapView;
}
}
namespace platform {
namespace geo {
class GeoLocation;
}
}
}

class MappingApp: public QObject {
	Q_OBJECT
public:
	MappingApp();
	virtual ~MappingApp();

private:
	bb::cascades::maps::MapView* m_mapView;

	bb::cascades::Page* init();

	/**
	 * Creates an animation that makes a MapView follow a provided path
	 * by altering the view's latitude, longitude and heading so that
	 * the path's current segment is always pointing up. When this is
	 * used in conjunction with an appropriate tilt value (eg: 45),
	 * this is similar to following a car along a pre-defined path
	 *
	 * @param mapView The map control to modify.
	 * @param path The path to follow.
	 * @param mpsSpeed The speed at which to follow the path. This
	 *                 value is in meters per second. Useful values
	 *                 include 27.7 (= 100 km/h) and 26.8 (= 60
	 *                 miles/h).
	 */
	QAbstractAnimation* createTrackingAnimation(
			bb::cascades::maps::MapView* mapView,
			bb::platform::geo::Polyline path, double mpsSpeed);

	/**
	 * Creates an animation that jumps from one location to another.
	 * Only the latitude, longitude and altitude values are modified,
	 * and North is always pointing up.
	 *
	 * @param mapView The map control to modify.
	 * @param start The coordinates of the start of the jump.
	 * @param end The coordinates of the end of the jump.
	 * @param duration The duration of the animation (in milliseconds).
	 */
	QAbstractAnimation* createFlightAnimation(
			bb::cascades::maps::MapView* mapView,
			bb::platform::geo::Point start, bb::platform::geo::Point end,
			int duration);

	/**
	 * Creates a "change of POI" animation instead of a "change of view"
	 * animation.  This animation creates three points of interest, each
	 * one representing an airplane flight.  The animation then modifies
	 * these POIs as if they were flying to YOW.
	 *
	 * @param mapView The map control upon which to place the POIs.
	 */
	QAbstractAnimation* createMultiFlightAnimation(
			bb::cascades::maps::MapView* mapView);

	/**
	 * Create create an animation that moves the plane from it's current
	 * location to the given destination.  The animation is added to the
	 * provided animation.
	 *
	 * @param allFlights The container to add this animation into.
	 * @param plane The POI to animate.
	 * @param dest The destination of the plane.
	 */
	void setupSingleFlightInMultiFlightAnimation(
			QParallelAnimationGroup* allFlights,
			bb::platform::geo::GeoLocation* plane,
			bb::platform::geo::Point dest);

	Q_SLOT void onStartDriveAnimationClicked();
	Q_SLOT void onStartFlightAnimationClicked();
	Q_SLOT void onStartMultiFlightAnimationClicked();

	/**
	 * Verifies if the location setting is on or not.
	 * Displays toast message indicating that features might not work
	 * as expected when location is disabled.
	 */
	void verifyLocationEnabled();

};

#endif /* MAPPINGAPP_HPP_ */
