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

#include "LocationSession.hpp"

#include "RawLocationParser.hpp"

#include <bb/multimedia/SystemSound>

#include <QtCore/QVariant>

#include <iostream>

using namespace bb::multimedia;

//! [0]
LocationSession::LocationSession(QObject* parent, bool satInfo, MapView *map) :
		QObject(parent), m_soundEnabled(false), m_positionSource(QGeoPositionInfoSource::createDefaultSource(this)), m_isPropagated(false) {

	setMapView(map);

	// Long pressing on the mapview will drop a push pin. This triggers
	MapLongPressToPinDrop *longPressAction = new MapLongPressToPinDrop(map);
	bool connected = connect(longPressAction, SIGNAL(pinCreated(const QString&)), this, SLOT(onPinCreated(const QString&)));


	if (m_positionSource) {
		connect(m_positionSource, SIGNAL(positionUpdated(const QGeoPositionInfo &)), this, SLOT(positionUpdated(const QGeoPositionInfo &)));
		connect(m_positionSource, SIGNAL(updateTimeout()), this, SLOT(positionUpdateTimeout()));

		log(tr("Initialized QGeoPositionInfoSource"));
	} else {
		log(tr("Failed to initialized QGeoPositionInfoSource"));
	}

	if (satInfo) {
		m_satelliteSource = QGeoSatelliteInfoSource::createDefaultSource(this);

		if (m_satelliteSource) {
			connect(m_satelliteSource, SIGNAL(satellitesInUseUpdated(const QList<QGeoSatelliteInfo> &)), this, SLOT(satellitesInUseUpdated(const QList<QGeoSatelliteInfo> &)));
			connect(m_satelliteSource, SIGNAL(satellitesInViewUpdated(const QList<QGeoSatelliteInfo> &)), this, SLOT(satellitesInViewUpdated(const QList<QGeoSatelliteInfo> &)));

			log(tr("Initialized QGeoSatelliteInfoSource"));
		} else {
			log(tr("Failed to initialized QGeoSatelliteInfoSource"));
		}
	}

	m_direction = m_horizontalAccuracy = m_verticalAccuracy = m_magneticVariation = tr("--");
	m_time = tr("-/-/- -:-");
	m_groundSpeed = tr("-- km/h");
	m_verticalSpeed = tr("-- km/h");
	m_satellitesInUse = m_satellitesInView = tr("No satellite information available");
}
//! [0]

//! [1]
void LocationSession::startUpdates() {
	if (m_positionSource) {
		m_positionSource->startUpdates();
		log(tr("Position updates started"));
	}

	if (m_satelliteSource) {
		m_satelliteSource->startUpdates();
		log(tr("Satellite updates started"));
	}
}
//! [1]

//! [2]
void LocationSession::requestUpdate() {
	if (m_positionSource) {
		m_positionSource->requestUpdate(120000);
		log(tr("Update requested"));
	}
}
//! [2]

//! [3]
void LocationSession::stopUpdates() {
	if (m_positionSource) {
		m_positionSource->stopUpdates();
		log(tr("Position updates stopped"));
	}

	if (m_satelliteSource) {
		m_satelliteSource->stopUpdates();
		log(tr("Satellite updates stopped"));
	}

	deleteLater();
}
//! [3]

//! [4]
void LocationSession::resetSession(const QString &type) {
	if (m_soundEnabled)
		SystemSound::play(SystemSound::GeneralNotification);

	if (m_positionSource) {
		m_positionSource->stopUpdates();
		m_positionSource->setProperty("reset", type);
		m_positionSource->startUpdates();
		log(tr("%1 RESET").arg(type));
	}
}
//! [4]

//! [5]
void LocationSession::positionUpdated(const QGeoPositionInfo& pos) {
	if (m_soundEnabled)
		SystemSound::play(SystemSound::GeneralNotification);

	m_latitude = pos.coordinate().latitude();
	m_longitude = pos.coordinate().longitude();
	m_altitude = pos.coordinate().altitude();
	m_time = pos.timestamp().toString();
	m_direction = QString::number(pos.attribute(QGeoPositionInfo::Direction));
	m_groundSpeed = QString::number(pos.attribute(QGeoPositionInfo::GroundSpeed));
	m_verticalSpeed = QString::number(pos.attribute(QGeoPositionInfo::VerticalSpeed));
	m_horizontalAccuracy = QString::number(pos.attribute(QGeoPositionInfo::HorizontalAccuracy));
	m_verticalAccuracy = QString::number(pos.attribute(QGeoPositionInfo::VerticalAccuracy));
	m_magneticVariation = QString::number(pos.attribute(QGeoPositionInfo::MagneticVariation));

	parseRawData();

	emit dataChanged();

	Geographic *myGeo = mapView->mapData()->geographic("myLocation");
	GeoLocation *myLocation = NULL;
	if(myGeo != NULL){
		myLocation = qobject_cast<GeoLocation*>(myGeo);
	}

	if(myLocation == NULL){
		myLocation = new GeoLocation("myLocation");
		mapView->mapData()->add(myLocation);
	}



	myLocation->setLatitude(latitude());
	myLocation->setLongitude(longitude());
	myLocation->setName("You're here!");
	myLocation->setDescription(QString::number(latitude()) + ", " + QString::number(longitude()));
	mapView->setFocusedId(myLocation->id());
	mapView->setLocationOnFocused();

	log(tr("update"));
}
//! [5]

QGeoPositionInfoSource* LocationSession::positionSource() const {
	return m_positionSource;
}

void LocationSession::setSoundEnabled(bool enabled) {
	m_soundEnabled = enabled;
}

void LocationSession::positionUpdateTimeout() {
	log(tr("positionUpdateTimeout() received"));
}

static QString satellitesToString(const QList<QGeoSatelliteInfo> &satellites) {
	QString text;

	foreach (const QGeoSatelliteInfo &info, satellites){
	text += QObject::tr("PRN: %1\nAzimuth: %2\nElevation: %3\nSignal: %4\n")
	.arg(info.prnNumber())
	.arg(info.attribute(QGeoSatelliteInfo::Azimuth))
	.arg(info.attribute(QGeoSatelliteInfo::Elevation))
	.arg(info.signalStrength());
}

	return text;
}

void LocationSession::satellitesInUseUpdated(const QList<QGeoSatelliteInfo> &satellites) {
	log(tr("satellitesInUseUpdated received"));

	m_satellitesInUse = tr("Satellites In Use\n%1").arg(satellitesToString(satellites));

	emit dataChanged();
}

void LocationSession::satellitesInViewUpdated(const QList<QGeoSatelliteInfo> &satellites) {
	log(tr("satellitesInViewUpdated received"));

	m_satellitesInView = tr("Satellites In View\n%1").arg(satellitesToString(satellites));

	emit dataChanged();
}

void LocationSession::parseRawData() {
	// Parsing the raw data from the low level Location Manager. Use this only if a field is not accessible via QGeoPositionInfo above.
	const QVariant replyData = (m_positionSource ? m_positionSource->property("replyDat") : QVariant());

	if (!replyData.isValid()) {
		log(tr("!!! Invalid replyDat."));
	}

	RawLocationParser parser(replyData);

	m_method = parser.positionMethod();
	m_horizontalDilution = QString::number(parser.hdop());
	m_verticalDilution = QString::number(parser.vdop());
	m_positionDilution = QString::number(parser.pdop());
	m_ttff = QString::number(parser.ttff());
	m_gpsWeek = QString::number(parser.gpsWeek());
	m_gpsTimeOfWeek = QString::number(parser.gpsTow());
	m_isPropagated = parser.propagated();

	const double latitude = parser.latitude();
	const double longitude = parser.longitude();
	const double altitude = parser.altitude();
	const double hAccuracy = parser.horizontalAccuracy();
	const double vAccuracy = parser.verticalAccuracy();
	const double heading = parser.heading();
	const double speed = parser.speed();
	const double utc = parser.utc();

	log(
			tr("Method: %0, Latitude: %1, Longitude: %2, Altitude: %3, Horizontal Accuracy: %4, Vertical Accuracy: %5, Heading: %6, Speed: %7, TTFF: %8, GPS Week: %9, ").arg(m_method).arg(latitude).arg(longitude).arg(altitude).arg(hAccuracy).arg(vAccuracy).arg(heading).arg(speed).arg(m_ttff).arg(m_gpsWeek)
					+ tr("GPS TOW: %0, UTC: %1, Horizontal Dilution: %2, Vertical Dilution: %3, Positional Dilution: %4, Propagated: %5").arg(m_gpsTimeOfWeek).arg(utc).arg(m_horizontalDilution).arg(m_verticalDilution).arg(m_positionDilution).arg(m_isPropagated ? tr("true") : tr("false")), false);

	const QString error = parser.error();
	if (error.length() > 3) {
		log(tr("!!! [Error] %1").arg(error));
	}

	for (int i = 0; i < parser.numberOfSatellites(); i++) {
		const double id = parser.satelliteId(i);
		const double cno = parser.satelliteCarrierToNoiseRatio(i);
		const bool ephemerisAvailable = parser.satelliteEphemerisAvailable(i);
		const double azimuth = parser.satelliteAzimuth(i);
		const double elevation = parser.satelliteElevation(i);
		const bool tracked = parser.satelliteTracked(i);
		const bool used = parser.satelliteUsed(i);

		log(tr("\t[Satellite %0], ID: %1, CNO: %2, Ephemeris Available: %3, Azimuth: %4, Elevation: %5, Tracked: %6, Used: %7").arg(i).arg(id).arg(cno).arg(ephemerisAvailable ? tr("true") : tr("false")).arg(azimuth).arg(elevation).arg(tracked ? tr("true") : tr("false")).arg(used ? tr("true") : tr("false")), false);
	}
}

void LocationSession::log(const QString &msg, bool showInUi) {
	std::cout << msg.toStdString() << std::endl;
	if (showInUi) {
		m_log += msg + QLatin1String("\n");
		emit logChanged();
	}
}

void LocationSession::setMapView(MapView *value) {
	mapView = value;
}

QString LocationSession::method() const {
	return m_method;
}

double LocationSession::latitude() const {
	return m_latitude;
}

double LocationSession::longitude() const {
	return m_longitude;
}

double LocationSession::altitude() const {
	return m_altitude;
}

QString LocationSession::time() const {
	return m_time;
}

QString LocationSession::direction() const {
	return m_direction;
}

QString LocationSession::groundSpeed() const {
	return m_groundSpeed;
}

QString LocationSession::verticalSpeed() const {
	return m_verticalSpeed;
}

QString LocationSession::horizontalAccuracy() const {
	return m_horizontalAccuracy;
}

QString LocationSession::verticalAccuracy() const {
	return m_verticalAccuracy;
}

QString LocationSession::magneticVariation() const {
	return m_magneticVariation;
}

QString LocationSession::horizontalDilution() const {
	return m_horizontalDilution;
}

QString LocationSession::verticalDilution() const {
	return m_verticalDilution;
}

QString LocationSession::positionDilution() const {
	return m_positionDilution;
}

QString LocationSession::ttff() const {
	return m_ttff;
}

QString LocationSession::gpsWeek() const {
	return m_gpsWeek;
}

QString LocationSession::gpsTimeOfWeek() const {
	return m_gpsTimeOfWeek;
}

bool LocationSession::isPropagated() const {
	return m_isPropagated;
}

QString LocationSession::satellitesInUse() const {
	return m_satellitesInUse;
}

QString LocationSession::satellitesInView() const {
	return m_satellitesInView;
}

void LocationSession::onPinCreated(const QString& pinID) {
	log("pin created: " + pinID, false);
}

QString LocationSession::log() const {
	return m_log;
}
