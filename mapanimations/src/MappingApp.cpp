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

#include "MappingApp.hpp"

#include <bb/cascades/ActionBarPlacement>
#include <bb/cascades/ActionItem>
#include <bb/cascades/Application>
#include <bb/cascades/Container>
#include <bb/cascades/Page>
#include <bb/cascades/maps/MapData>
#include <bb/cascades/maps/MapView>
#include <bb/platform/geo/GeoPolyline>
#include <bb/platform/geo/GeoLocation>
#include <bb/platform/geo/Polyline>
#include <bb/system/SystemDialog>
#include <QtLocationSubset/QGeoCoordinate>
#include <QtLocationSubset/QGeoPositionInfoSource>

#include <QDebug>
#include <QAbstractAnimation>

using namespace bb::cascades;
using namespace bb::cascades::maps;
using namespace bb::platform::geo;
using QtMobilitySubset::QGeoCoordinate;
using QtMobilitySubset::QGeoPositionInfoSource;

/**
 * The multi-flight animation modifies a property as part of the animation,
 * which needs to be QVariant compatible.  For GeoLocation's the location
 * information is stored in a Point, which is not a QVariant. To accommodate
 * this, an "animation interpreter" function is used to determine the
 * appropriate changes at a specific point in the animation's progress.
 */
QVariant pointAnimationInterpolator(const bb::platform::geo::Point& start,
		const bb::platform::geo::Point& end, qreal progress)
{
	Point result;
	result.setLatitude((end.latitude() - start.latitude()) * progress + start.latitude());
	result.setLongitude((end.longitude() - start.longitude()) * progress + start.longitude());
	// not really necessary, but for completeness
	result.setAltitude((end.altitude() - start.altitude()) * progress + start.altitude());
	QVariant v;
	v.setValue(result);
	return v;
}

const Coordinate drivingCoordinates[28] = { Coordinate(45.34254984806693,-75.92945946719877),  Coordinate(45.3425414457549, -75.93035697294239),
											Coordinate(45.34315026720957, -75.93028220153305), Coordinate(45.34353751590488, -75.93010563421292),
											Coordinate(45.34415384365244, -75.93010941154165), Coordinate(45.3446852579663, -75.93045412888773),
											Coordinate(45.34510219586706, -75.93091086896648), Coordinate(45.3478399457104, -75.92664696330979),
											Coordinate(45.34892905492961, -75.92470264219908), Coordinate(45.34940283817473, -75.92394023369711),
											Coordinate(45.34964903911299, -75.92286006616197), Coordinate(45.34957471608524, -75.92186111620949),
											Coordinate(45.34942614506517, -75.92095675975592), Coordinate(45.34914680663247, -75.92094662004),
											Coordinate(45.34876501569315, -75.92071910835911), Coordinate(45.34814675067516, -75.92025552400592),
											Coordinate(45.34472217000616, -75.9158069805362),  Coordinate(45.34448272728834, -75.91535604586107),
											Coordinate(45.3441660679114, -75.91436625488032),  Coordinate(45.3439733396328, -75.91389463948262),
											Coordinate(45.34355028143146, -75.91328243932992), Coordinate(45.34259459771675, -75.91208641090967),
											Coordinate(45.34182514673778, -75.9109443388713),  Coordinate(45.343281998413, -75.90920432927531),
											Coordinate(45.34340746859033, -75.90895093003152), Coordinate(45.34357846477555, -75.90902807032643),
											Coordinate(45.34358883511185, -75.90919302761208), Coordinate(45.34349362982461, -75.9094334832255) };

MappingApp::MappingApp() {

	//qRegisterMetaType<bb::platform::geo::Point>( "bb::platform::geo::Point" );
	qRegisterAnimationInterpolator<bb::platform::geo::Point>(pointAnimationInterpolator);

	Page* root = init();

	verifyLocationEnabled();

	Application::instance()->setScene(root);
}

MappingApp::~MappingApp() {
}

Page* MappingApp::init() {
	Page * page = new Page();
	Container* topContainer = Container::create();

	m_mapView = new MapView;
	m_mapView->setAltitude(10000);
	m_mapView->setLatitude(45.34254984806693);
	m_mapView->setLongitude(-75.92945946719877);

	topContainer->add(m_mapView);

	ActionItem* startDriveAnimation =
				ActionItem::create().title("Drive").imageSource(
				QUrl("asset:///compass.png")).onTriggered(this,
				SLOT( onStartDriveAnimationClicked() ));
	page->addAction(startDriveAnimation, ActionBarPlacement::OnBar);

	ActionItem* startFlightAnimation =
				ActionItem::create().title("Start Flight").imageSource(
				QUrl("asset:///airplane_blue_045.png")).onTriggered(this,
				SLOT( onStartFlightAnimationClicked() ));
	page->addAction(startFlightAnimation, ActionBarPlacement::OnBar);

	ActionItem* startMultiFlightAnimation =
				ActionItem::create().title("Flight Tracker").imageSource(
				QUrl("asset:///airplane_blue_270.png")).onTriggered(this,
				SLOT( onStartMultiFlightAnimationClicked() ));
	page->addAction(startMultiFlightAnimation, ActionBarPlacement::OnBar);

	page->setContent(topContainer);

	return page;
}

void MappingApp::onStartDriveAnimationClicked() {
	Polyline inToFarar;
	for (int i = 0; i < 18; i++) {
		inToFarar.append(drivingCoordinates[i]);
	}
	GeoPolyline* path = new GeoPolyline;
	path->setName("Drive path from Innovation to Farar");
	path->setLine(inToFarar);

	m_mapView->mapData()->clear();
	m_mapView->mapData()->add(path);
	m_mapView->setAltitude(1000);
	m_mapView->setTilt(45);
	QAbstractAnimation* animation = createTrackingAnimation(m_mapView,path->line(), 60);
	animation->start(QAbstractAnimation::DeleteWhenStopped);
}

void MappingApp::onStartFlightAnimationClicked() {
	// The follow start location and alt shows YKF, Waterloo Airport
	Point ykf = Point(43.460833, -80.378611);
	ykf.setAltitude(4000);
	// The follow location and alt shows YOW, Ottawa airport
	Point yow = Point(45.323487, -75.665087);
	yow.setAltitude(8000);

	m_mapView->resetHeading();
	m_mapView->resetTilt();

	createFlightAnimation(m_mapView, ykf, yow, 10000)->start(QAbstractAnimation::DeleteWhenStopped);
}

void MappingApp::onStartMultiFlightAnimationClicked() {
	m_mapView->mapData()->clear();
	QAbstractAnimation* animation = createMultiFlightAnimation(m_mapView);
	m_mapView->setLocationOnVisible();
	animation->start(QAbstractAnimation::DeleteWhenStopped);
}

QAbstractAnimation* MappingApp::createTrackingAnimation(
		bb::cascades::maps::MapView* mapView, bb::platform::geo::Polyline path,
		double mpsSpeed)
{
	qDebug() << __PRETTY_FUNCTION__;

	/* This function creates a set of sequential animations: one for each
	 * segment of the given line.
	 *
	 * Within each segment, there are three
	 * animations that are performed in parallel: heading, latitude and
	 * longitude.
	 *
	 * The duration of the heading change is hard-coded to a half-second.
	 *
	 * The duration of the remaining latitude and longitude changes are based on the
	 * distance of the segment and the provided speed.
	 *
	 * The change in heading requires a little bit of logic checking. The
	 * maximum turn for a car should 180 degrees. However, the animation's
	 * easing curve doesn't know the difference between turning from 135 to
	 * -225 by turning 90 degrees to the right and by turning 270 degrees
	 *  to the left. Thus, where the change in heading results in a turn
	 *  greater than 180 degrees, modulus arithmetic is applied to keep the
	 *  turn less than 180 degrees.
	 */

	QSequentialAnimationGroup* totalAnimation = new QSequentialAnimationGroup;

	double previousBearing = 0;

	for (int i = 0; i < path.count() - 1; i++) {
		Coordinate start = path.at(i);
		Coordinate end = path.at(i + 1);
		QGeoCoordinate qStart(start.latitude(), start.longitude());
		QGeoCoordinate qEnd(end.latitude(), end.longitude());
		double bearing = qStart.azimuthTo(qEnd);
		double distance = qStart.distanceTo(qEnd);
		double duration = distance / mpsSpeed * 1000;

		qDebug() << "segment: " << i;
		qDebug() << "distance: (meters)" << distance;
		qDebug() << "duration: (meters / millisecond)" << duration;
		qDebug() << "heading: " << bearing;

		QParallelAnimationGroup* segmentAnimation = new QParallelAnimationGroup;

		if (qAbs(bearing - previousBearing) > 180) {
			if (bearing < previousBearing) {
				bearing += 360;
			} else {
				bearing -= 360;
			}
		}

		QPropertyAnimation* bearingAni = new QPropertyAnimation;
		bearingAni->setTargetObject(mapView);
		bearingAni->setPropertyName("heading");
		bearingAni->setDuration(500);
		bearingAni->setEndValue(bearing);
		bearingAni->setEasingCurve(QEasingCurve::Linear);
		segmentAnimation->addAnimation(bearingAni);
		previousBearing = bearing;

		QPropertyAnimation* latAni = new QPropertyAnimation();
		latAni->setTargetObject(mapView);
		latAni->setPropertyName("latitude");
		latAni->setDuration(duration);
		latAni->setStartValue(QVariant(start.latitude()));
		latAni->setEndValue(QVariant(end.latitude()));
		latAni->setEasingCurve(QEasingCurve::Linear);
		segmentAnimation->addAnimation(latAni);

		QPropertyAnimation* lonAni = new QPropertyAnimation();
		lonAni->setTargetObject(mapView);
		lonAni->setPropertyName("longitude");
		lonAni->setDuration(duration);
		lonAni->setStartValue(QVariant(start.longitude()));
		lonAni->setEndValue(QVariant(end.longitude()));
		lonAni->setEasingCurve(QEasingCurve::Linear);
		segmentAnimation->addAnimation(lonAni);

		totalAnimation->addAnimation(segmentAnimation);
	}

	return totalAnimation;
}

QAbstractAnimation* MappingApp::createFlightAnimation(
		bb::cascades::maps::MapView* mapView, bb::platform::geo::Point start,
		bb::platform::geo::Point end, int duration)
{

	/* Background information:
	 * There are three animations being performed at the same time, change
	 * of latitude, change of longitude and change of altitude.  The
	 * latitude and longitude changes are performed throughout the
	 * animation. However, the change in altitude is broken up into two
	 * sequential segments: one for zooming out, and the second for zooming
	 * back in.
	 */

	qDebug() << __PRETTY_FUNCTION__;

	double deltaLat = qAbs(start.latitude() - end.latitude());
	double deltaLon = qAbs(start.longitude() - end.longitude());
	double maxAlt = qMax(deltaLat, deltaLon) * 100000;

	QParallelAnimationGroup* aniGroup = new QParallelAnimationGroup;

	QPropertyAnimation* latAni = new QPropertyAnimation();
	latAni->setTargetObject(mapView);
	latAni->setPropertyName("latitude");
	latAni->setDuration(duration);
	latAni->setStartValue(QVariant(start.latitude()));
	latAni->setEndValue(QVariant(end.latitude()));
	latAni->setEasingCurve(QEasingCurve::InOutCubic);
	aniGroup->addAnimation(latAni);

	QPropertyAnimation* lonAni = new QPropertyAnimation();
	lonAni->setTargetObject(mapView);
	lonAni->setPropertyName("longitude");
	lonAni->setDuration(duration);
	lonAni->setStartValue(QVariant(start.longitude()));
	lonAni->setEndValue(QVariant(end.longitude()));
	lonAni->setEasingCurve(QEasingCurve::InOutCubic);
	aniGroup->addAnimation(lonAni);

	QSequentialAnimationGroup* zoomOutIn = new QSequentialAnimationGroup();

	QPropertyAnimation* zoomOut = new QPropertyAnimation;
	zoomOut->setTargetObject(mapView);
	zoomOut->setPropertyName("altitude");
	zoomOut->setDuration(duration / 2);
	zoomOut->setStartValue(QVariant(start.altitude()));
	zoomOut->setEndValue(QVariant(maxAlt));
	zoomOut->setEasingCurve(QEasingCurve::OutQuad);
	zoomOutIn->addAnimation(zoomOut);

	QPropertyAnimation* zoomIn = new QPropertyAnimation;
	zoomIn->setTargetObject(mapView);
	zoomIn->setPropertyName("altitude");
	zoomIn->setDuration(duration / 2);
	zoomIn->setEndValue(QVariant(end.altitude()));
	zoomIn->setEasingCurve(QEasingCurve::InQuad);
	zoomOutIn->addAnimation(zoomIn);

	aniGroup->addAnimation(zoomOutIn);

	return aniGroup;
}

QAbstractAnimation* MappingApp::createMultiFlightAnimation(
		bb::cascades::maps::MapView* mapView)
{
	QParallelAnimationGroup* allFlights = new QParallelAnimationGroup;
	Point yow = Point(45.323487, -75.665087);
	yow.setAltitude(8000);

	GeoLocation* flight = new GeoLocation;
	flight->setName("LAX - YOW");
	flight->setDescription("Flight from Los Angeles, CA, US to Ottawa, ON, CA");
	flight->setPoint(Point(33.943218, -118.406208));
	flight->setAltitude(8000);
	flight->setMarker(Marker("asset:///airplane_blue_045.png", QSize(81, 81),
					  QPoint(40, 40), QPoint(67, 13)));

	mapView->mapData()->add(flight);

	setupSingleFlightInMultiFlightAnimation(allFlights, flight, yow);

	flight = new GeoLocation;
	flight->setName("LHR - YOW");
	flight->setDescription("Flight from Heathrow, London, UK to Ottawa, ON, CA");
	flight->setPoint(Point(51.47283, -0.453041));
	flight->setAltitude(8000);
	flight->setMarker(Marker("asset:///airplane_blue_270.png", QSize(81, 81),
					  QPoint(40, 40), QPoint(3, 41)));

	mapView->mapData()->add(flight);

	setupSingleFlightInMultiFlightAnimation(allFlights, flight, yow);

	flight = new GeoLocation;
	flight->setName("GRU - YOW");
	flight->setDescription("Flight from Sau Paulo, Brazil to Ottawa, ON, CA");
	flight->setPoint(Point(-23.426138, -46.481741));
	flight->setAltitude(8000);
	flight->setMarker(Marker("asset:///airplane_blue_000.png", QSize(81, 81),
					  QPoint(40, 40), QPoint(40, 3)));

	mapView->mapData()->add(flight);

	setupSingleFlightInMultiFlightAnimation(allFlights, flight, yow);

	return allFlights;
}

void MappingApp::setupSingleFlightInMultiFlightAnimation(
		QParallelAnimationGroup* allFlights,
		bb::platform::geo::GeoLocation* plane, bb::platform::geo::Point dest)
{
	QVariant vStart;
	vStart.setValue(plane->point());
	QVariant vDest;
	vDest.setValue(dest);

	QPropertyAnimation* latAni = new QPropertyAnimation();
	latAni->setTargetObject(plane);
	latAni->setPropertyName("point");
	latAni->setDuration(20 * 1000);
	latAni->setStartValue(vStart);
	latAni->setEndValue(vDest);
	latAni->setEasingCurve(QEasingCurve::InOutQuad);
	allFlights->addAnimation(latAni);
}

void MappingApp::verifyLocationEnabled() {
	QtMobilitySubset::QGeoPositionInfoSource *m_geopositionISource = QGeoPositionInfoSource::createDefaultSource(this);
	if (!m_geopositionISource->property("locationServicesEnabled").toBool()) {
		bb::system::SystemDialog *sd = new bb::system::SystemDialog(this);
		sd->setTitle("Can't Access Location");
		sd->setBody("Some features will not be available because location services are turned off or you have no access.");
		sd->confirmButton()->setEnabled(false);
		sd->cancelButton()->setLabel("Dismiss");
		sd->cancelButton()->setEnabled(true);
		sd->show();
	}
}
