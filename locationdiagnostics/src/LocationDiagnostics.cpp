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

#include "LocationDiagnostics.hpp"

#include "LocationSession.hpp"

#include <QCoreApplication>
#include <QSettings>

//! [0]
LocationDiagnostics::LocationDiagnostics(QObject *parent)
    : QObject(parent)
{
    // Load default values from persistent storage
    QSettings settings;
    m_positionMethod = settings.value("positionMethod", "all").toString();
    m_assistanceMode = settings.value("assistanceMode", "password").toString();
    m_frequency = settings.value("frequency", "1").toInt();
    m_useSound = settings.value("useSound", true).toBool();
    m_backgroundMode = settings.value("backgroundMode", true).toBool();

    connect(qApp, SIGNAL(manualExit()), SLOT(onManualExit()));
}
//! [0]

//! [1]
LocationSession* LocationDiagnostics::createLocationSession(bool trackingMode, QObject *var)
{
    // Create a new location session
    LocationSession* session = new LocationSession(this, true, qobject_cast<Page*>(var)->findChild<MapView*>("myMap"));

    // Configure the session according to the property values
    session->setSoundEnabled(m_useSound);

    if (!session->positionSource())
        return session;

    if (m_positionMethod == QLatin1String("all")) {
        session->positionSource()->setPreferredPositioningMethods(QGeoPositionInfoSource::AllPositioningMethods);
    } else if (m_positionMethod == QLatin1String("nonSatellite")) {
        session->positionSource()->setPreferredPositioningMethods(QGeoPositionInfoSource::NonSatellitePositioningMethods);
    } else if (m_positionMethod == QLatin1String("satellite")) {
        session->positionSource()->setPreferredPositioningMethods(QGeoPositionInfoSource::SatellitePositioningMethods);
    } else if (m_positionMethod == QLatin1String("gps")) {
        session->positionSource()->setProperty("provider", "gnss");
        session->positionSource()->setProperty("fixType", "gps_autonomous");
    } else if (m_positionMethod == QLatin1String("cellular")) {
        session->positionSource()->setProperty("provider", "network");
        session->positionSource()->setProperty("fixType", "cellsite");
    } else if (m_positionMethod == QLatin1String("wifi")) {
        session->positionSource()->setProperty("provider", "network");
        session->positionSource()->setProperty("fixType", "wifi");
    } else if (m_positionMethod == QLatin1String("msb")) {
        session->positionSource()->setProperty("provider", "gnss");
        session->positionSource()->setProperty("fixType", "gps_ms_based");
    } else if (m_positionMethod == QLatin1String("msa")) {
        session->positionSource()->setProperty("provider", "gnss");
        session->positionSource()->setProperty("fixType", "gps_ms_assisted");
    }

    if (m_assistanceMode == QLatin1String("pde")) {
        if (!m_pdeUrl.isEmpty())
            session->positionSource()->setProperty("qtaPDEURL", m_pdeUrl);
    } else if (m_assistanceMode == QLatin1String("slp")) {
        if (!m_slpUrl.isEmpty())
            session->positionSource()->setProperty("slpURL", m_slpUrl);
    } else if (m_assistanceMode == QLatin1String("password")) {
        if (!m_appId.isEmpty())
            session->positionSource()->setProperty("qtaAppID", m_appId);

        if (!m_password.isEmpty())
            session->positionSource()->setProperty("qtaAppPassword", m_password);
    }

    session->positionSource()->setUpdateInterval(m_frequency * 1000);
    session->positionSource()->setProperty("canRunInBackground", m_backgroundMode);


    // Activate the session
    if (trackingMode) {
        session->startUpdates();
    } else {
        session->requestUpdate();
    }

    return session;
}
//! [1]

void LocationDiagnostics::onManualExit()
{
	qApp->exit(0);
}

//! [2]
QString LocationDiagnostics::positionMethod() const
{
    return m_positionMethod;
}

void LocationDiagnostics::setPositionMethod(const QString &method)
{
    if (m_positionMethod == method)
        return;

    m_positionMethod = method;

    QSettings settings;
    settings.setValue("positionMethod", m_positionMethod);

    emit positionMethodChanged();
}
//! [2]

QString LocationDiagnostics::assistanceMode() const
{
    return m_assistanceMode;
}

void LocationDiagnostics::setAssistanceMode(const QString &mode)
{
    if (m_assistanceMode == mode)
        return;

    m_assistanceMode = mode;

    QSettings settings;
    settings.setValue("assistanceMode", m_assistanceMode);

    emit assistanceModeChanged();
}

QString LocationDiagnostics::pdeUrl() const
{
    return m_pdeUrl;
}

void LocationDiagnostics::setPdeUrl(const QString &url)
{
    if (m_pdeUrl == url)
        return;

    m_pdeUrl = url;
    emit pdeUrlChanged();
}

QString LocationDiagnostics::slpUrl() const
{
    return m_slpUrl;
}

void LocationDiagnostics::setSlpUrl(const QString &url)
{
    if (m_slpUrl == url)
        return;

    m_slpUrl = url;
    emit slpUrlChanged();
}

QString LocationDiagnostics::appId() const
{
    return m_appId;
}

void LocationDiagnostics::setAppId(const QString &appId)
{
    if (m_appId == appId)
        return;

    m_appId = appId;
    emit appIdChanged();
}

QString LocationDiagnostics::password() const
{
    return m_password;
}

void LocationDiagnostics::setPassword(const QString &password)
{
    if (m_password == password)
        return;

    m_password = password;
    emit passwordChanged();
}

int LocationDiagnostics::frequency() const
{
    return m_frequency;
}

void LocationDiagnostics::setFrequency(int frequency)
{
    if (m_frequency == frequency)
        return;

    m_frequency = frequency;

    QSettings settings;
    settings.setValue("frequency", m_frequency);

    emit frequencyChanged();
}

bool LocationDiagnostics::useSound() const
{
    return m_useSound;
}

void LocationDiagnostics::setUseSound(bool sound)
{
    if (m_useSound == sound)
        return;

    m_useSound = sound;

    QSettings settings;
    settings.setValue("useSound", m_useSound);

    emit useSoundChanged();
}

bool LocationDiagnostics::backgroundMode() const
{
    return m_backgroundMode;
}

void LocationDiagnostics::setBackgroundMode(bool mode)
{
    if (m_backgroundMode == mode)
        return;

    m_backgroundMode = mode;

    QSettings settings;
    settings.setValue("backgroundMode", m_backgroundMode);

    emit backgroundModeChanged();
}

