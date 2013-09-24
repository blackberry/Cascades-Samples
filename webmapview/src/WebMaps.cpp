/* Copyright (c) 2013 BlackBerry Limited.
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
#include "WebMaps.hpp"

#include <QDebug>
#include <QFile>

//! [0]
WebMaps::WebMaps(QObject *parent)
    : QObject(parent)
    , m_currentProvider(GoogleMaps)
    , m_currentViewModeIndex(0)
{
//! [0]

    //! [1]
    // Google Maps
    {
        QVariantMap entry;
        entry["scriptFile"] = QLatin1String("local:///assets/google_map.js");
        entry["mapUrl"] = QLatin1String("https://maps.googleapis.com/maps/api/js?sensor=true&libraries=places");

        QVariantList viewModes;
        {
            QVariantMap viewMode;
            viewMode["title"] = tr("Hybrid");
            viewMode["mapType"] = QLatin1String("google.maps.MapTypeId.HYBRID");
            viewModes << viewMode;
        }
        {
            QVariantMap viewMode;
            viewMode["title"] = tr("Road");
            viewMode["mapType"] = QLatin1String("google.maps.MapTypeId.ROADMAP");
            viewModes << viewMode;
        }
        {
            QVariantMap viewMode;
            viewMode["title"] = tr("Satellite");
            viewMode["mapType"] = QLatin1String("google.maps.MapTypeId.SATELLITE");
            viewModes << viewMode;
        }
        {
            QVariantMap viewMode;
            viewMode["title"] = tr("Terrain");
            viewMode["mapType"] = QLatin1String("google.maps.MapTypeId.TERRAIN");
            viewModes << viewMode;
        }
        entry["viewModes"] = viewModes;

        m_providerData.insert(GoogleMaps, entry);
    }
    //! [1]

    // Bing Maps
    {
        QVariantMap entry;
        entry["scriptFile"] = QLatin1String("local:///assets/bing_map.js");
        entry["mapUrl"] = QLatin1String("http://ecn.dev.virtualearth.net/mapcontrol/mapcontrol.ashx?v=7.0&mkt=en-gb");

        QVariantList viewModes;
        {
            QVariantMap viewMode;
            viewMode["title"] = tr("Auto");
            viewMode["mapType"] = QLatin1String("Microsoft.Maps.MapTypeId.auto");
            viewModes << viewMode;
        }
        {
            QVariantMap viewMode;
            viewMode["title"] = tr("Aerial");
            viewMode["mapType"] = QLatin1String("Microsoft.Maps.MapTypeId.aerial");
            viewModes << viewMode;
        }
        {
            QVariantMap viewMode;
            viewMode["title"] = tr("Birds Eye");
            viewMode["mapType"] = QLatin1String("Microsoft.Maps.MapTypeId.birdsEye");
            viewModes << viewMode;
        }
        {
            QVariantMap viewMode;
            viewMode["title"] = tr("Collins Bart");
            viewMode["mapType"] = QLatin1String("Microsoft.Maps.MapTypeId.collinsBart");
            viewModes << viewMode;
        }
        {
            QVariantMap viewMode;
            viewMode["title"] = tr("Mercator");
            viewMode["mapType"] = QLatin1String("Microsoft.Maps.MapTypeId.mercator");
            viewModes << viewMode;
        }
        {
            QVariantMap viewMode;
            viewMode["title"] = tr("Ordnance Survey");
            viewMode["mapType"] = QLatin1String("Microsoft.Maps.MapTypeId.ordnanceSurvey");
            viewModes << viewMode;
        }
        {
            QVariantMap viewMode;
            viewMode["title"] = tr("Road");
            viewMode["mapType"] = QLatin1String("Microsoft.Maps.MapTypeId.road");
            viewModes << viewMode;
        }
        entry["viewModes"] = viewModes;

        m_providerData.insert(BingMaps, entry);
    }

    // OpenLayers
    {
        QVariantMap entry;
        entry["scriptFile"] = QLatin1String("local:///assets/openlayers_map.js");
        entry["mapUrl"] = QLatin1String("http://openlayers.org/api/OpenLayers.js");

        QVariantList viewModes;
        {
            QVariantMap viewMode;
            viewMode["title"] = tr("Default");
            viewMode["mapType"] = QLatin1String("dummy");
            viewModes << viewMode;
        }
        entry["viewModes"] = viewModes;

        m_providerData.insert(OpenLayers, entry);
    }
}

//! [2]
void WebMaps::nextViewMode()
{
    const QVariantMap &entry = m_providerData[m_currentProvider];
    const QVariantList viewModes = entry["viewModes"].toList();

    m_currentViewModeIndex = ((m_currentViewModeIndex + 1) % viewModes.count());

    emit viewModeChanged();
}
//! [2]

WebMaps::Provider WebMaps::currentProvider() const
{
    return m_currentProvider;
}

//! [3]
void WebMaps::setCurrentProvider(Provider provider)
{
    if (m_currentProvider == provider)
        return;

    m_currentProvider = provider;
    m_currentViewModeIndex = 0;

    emit currentProviderChanged();
    emit viewModeChanged();
}
//! [3]

//! [4]
QString WebMaps::pageContent() const
{
    const QVariantMap &entry = m_providerData[m_currentProvider];

    QFile file("app/native/assets/map.html");
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Unable to open map template file";
        return QString();
    }

    // Replace placeholders with data from current provider
    QByteArray content = file.readAll();
    content.replace("$$SCRIPT_FILE$$", entry["scriptFile"].toByteArray());
    content.replace("$$MAP_URL$$", entry["mapUrl"].toByteArray());

    return QString::fromUtf8(content);
}
//! [4]

//! [5]
QString WebMaps::viewModeTitle() const
{
    const QVariantMap &entry = m_providerData[m_currentProvider];
    const QVariantList viewModes = entry["viewModes"].toList();

    return viewModes[m_currentViewModeIndex].toMap()["title"].toString();
}

QString WebMaps::viewMode() const
{
    const QVariantMap &entry = m_providerData[m_currentProvider];
    const QVariantList viewModes = entry["viewModes"].toList();

    return viewModes[m_currentViewModeIndex].toMap()["mapType"].toString();
}
//! [5]
