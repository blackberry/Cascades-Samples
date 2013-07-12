/* Copyright (c) 2012-2013 Research In Motion Limited.
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

#include "AccelerometerService.hpp"
#include "AzimuthPitchRollService.hpp"
#include "BPSEventHandler.hpp"
#include "BPSMonitor.hpp"
#include "GeolocationService.hpp"
#include "LightService.hpp"
#include "MagnetometerService.hpp"
#include "ProximityService.hpp"
#include "VirtualKeyboardService.hpp"

namespace bb {
namespace cascades {
namespace bps {

//! [0]
BPSMonitor::BPSMonitor(QObject *parent)
	: QObject(parent)
	, m_bpsEventHandler(new BPSEventHandler())
	, m_virtualKeyboardService(0)
	, m_geolocationService(0)
	, m_accelerometerService(0)
	, m_magnetometerService(0)
	, m_proximityService(0)
	, m_azimuthPitchRollService(0)
	, m_lightService(0)
{
}
//! [0]

BPSMonitor::~BPSMonitor() {
	delete m_bpsEventHandler;
	delete m_virtualKeyboardService;
	delete m_geolocationService;
	delete m_accelerometerService;
	delete m_magnetometerService;
	delete m_proximityService;
	delete m_azimuthPitchRollService;
	delete m_lightService;
}

//! [1]
VirtualKeyboardService *BPSMonitor::virtualKeyboardService() {
	return m_virtualKeyboardService;
}

void BPSMonitor::setVirtualKeyboardService(VirtualKeyboardService *service) {
	m_bpsEventHandler->registerService(service);
	m_virtualKeyboardService = service;
}
//! [1]

GeolocationService *BPSMonitor::geolocationService() {
	return m_geolocationService;
}

void BPSMonitor::setGeolocationService(GeolocationService *service) {
	m_bpsEventHandler->registerService(service);
	m_geolocationService = service;
}

AccelerometerService *BPSMonitor::accelerometerService() {
	return m_accelerometerService;
}

void BPSMonitor::setAccelerometerService(AccelerometerService *service) {
	m_bpsEventHandler->registerService(service);
	m_accelerometerService = service;
}

MagnetometerService *BPSMonitor::magnetometerService() {
	return m_magnetometerService;
}

void BPSMonitor::setMagnetometerService(MagnetometerService *service) {
	m_bpsEventHandler->registerService(service);
	m_magnetometerService = service;
}

ProximityService *BPSMonitor::proximityService() {
	return m_proximityService;
}

void BPSMonitor::setProximityService(ProximityService *service) {
	m_bpsEventHandler->registerService(service);
	m_proximityService = service;
}

AzimuthPitchRollService *BPSMonitor::azimuthPitchRollService() {
	return m_azimuthPitchRollService;
}

void BPSMonitor::setAzimuthPitchRollService(AzimuthPitchRollService *service) {
	m_bpsEventHandler->registerService(service);
	m_azimuthPitchRollService = service;
}

LightService *BPSMonitor::lightService() {
	return m_lightService;
}

void BPSMonitor::setLightService(LightService *service) {
	m_bpsEventHandler->registerService(service);
	m_lightService = service;
}

} /* namespace bps */
} /* namespace cascades */
} /* namespace bb */
