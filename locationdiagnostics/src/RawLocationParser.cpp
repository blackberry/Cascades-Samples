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

#include "RawLocationParser.hpp"

#include <QtCore/qnumeric.h>

RawLocationParser::RawLocationParser(const QVariant & replyData)
    : m_replyData(replyData)
{
}

double RawLocationParser::latitude() const
{
    return parseDouble(m_replyData, "latitude");
}

double RawLocationParser::longitude() const
{
    return parseDouble(m_replyData, "longitude");
}

double RawLocationParser::altitude() const
{
    return parseDouble(m_replyData, "altitude");
}

double RawLocationParser::horizontalAccuracy() const
{
    return parseDouble(m_replyData, "accuracy");
}

double RawLocationParser::verticalAccuracy() const
{
    return parseDouble(m_replyData, "altitudeAccuracy");
}

double RawLocationParser::heading() const
{
    return parseDouble(m_replyData, "heading");
}

double RawLocationParser::speed() const
{
    return parseDouble(m_replyData, "speed");
}

double RawLocationParser::ttff() const
{
    return parseDouble(m_replyData, "ttff");
}

double RawLocationParser::gpsWeek() const
{
    return parseDouble(m_replyData, "gpsWeek");
}

double RawLocationParser::gpsTow() const
{
    return parseDouble(m_replyData, "gpsTow");
}

double RawLocationParser::utc() const
{
    return parseDouble(m_replyData, "utc");
}

double RawLocationParser::hdop() const
{
    return parseDouble(m_replyData, "hdop");
}

double RawLocationParser::vdop() const
{
    return parseDouble(m_replyData, "vdop");
}

double RawLocationParser::pdop() const
{
    return parseDouble(m_replyData, "pdop");
}

bool RawLocationParser::propagated() const
{
    return parseBool(m_replyData, "propagated");
}

QString RawLocationParser::positionMethod() const
{
    const QString fixType = parseString(m_replyData, "fix_type");
    const QString provider = parseString(m_replyData, "provider");

    return QString::fromLatin1("%1 [%2]").arg(fixType).arg(provider);
}

QString RawLocationParser::error() const
{
    const QString err = parseString(m_replyData, "err");
    const QString errstr = parseString(m_replyData, "errstr");

    return QString::fromLatin1("%1: %2").arg(err).arg(errstr);
}

int RawLocationParser::numberOfSatellites() const
{
    QVariantList satellites;

    const bool sizeFound = parseList(&satellites, m_replyData, "satellites");
    if (sizeFound == false) {
        return 0;
    }

    return satellites.size();
}

double RawLocationParser::satelliteId(int satIndex) const
{
    QVariantList satellites;
    if (parseList(&satellites, m_replyData, "satellites") == false) {
        return qQNaN();
    }

    const QVariant sat = satellites.at(satIndex);
    return parseDouble(sat, "id");
}

double RawLocationParser::satelliteCarrierToNoiseRatio(int satIndex) const
{
    QVariantList satellites;
    if (parseList(&satellites, m_replyData, "satellites") == false) {
        return qQNaN();
    }

    const QVariant sat = satellites.at(satIndex);
    return parseDouble(sat, "cno");
}

bool RawLocationParser::satelliteEphemerisAvailable(int satIndex) const
{
    QVariantList satellites;
    if (parseList(&satellites, m_replyData, "satellites") == false) {
        return false;
    }

    const QVariant sat = satellites.at(satIndex);
    return parseBool(sat, "ephemeris");
}

double RawLocationParser::satelliteAzimuth(int satIndex) const
{
    QVariantList satellites;
    if (parseList(&satellites, m_replyData, "satellites") == false) {
        return qQNaN();
    }

    const QVariant sat = satellites.at(satIndex);
    return parseDouble(sat, "azimuth");
}

double RawLocationParser::satelliteElevation(int satIndex) const
{
    QVariantList satellites;
    if (parseList(&satellites, m_replyData, "satellites") == false) {
        return qQNaN();
    }

    const QVariant sat = satellites.at(satIndex);
    return parseDouble(sat, "elevation");
}

bool RawLocationParser::satelliteTracked(int satIndex) const
{
    QVariantList satellites;
    if (parseList(&satellites, m_replyData, "satellites") == false) {
        return false;
    }

    const QVariant sat = satellites.at(satIndex);
    return parseBool(sat, "tracked");
}

bool RawLocationParser::satelliteUsed(int satIndex) const
{
    QVariantList satellites;
    if (parseList(&satellites, m_replyData, "satellites") == false) {
        return false;
    }

    const QVariant sat = satellites.at(satIndex);
    return parseBool(sat, "used");
}

bool RawLocationParser::satelliteAlmanac(int satIndex) const
{
    QVariantList satellites;
    if (parseList(&satellites, m_replyData, "satellites") == false) {
        return false;
    }

    const QVariant sat = satellites.at(satIndex);
    return parseBool(sat, "almanac");
}

double RawLocationParser::parseDouble(const QVariant & replyData, const QString & key) const
{
    // replyData is a QVariantMap holding all of the reply parameters
    const QVariantMap positionData = replyData.toMap();

    const QVariant val = positionData.value(key);
    if (val.isValid() && val.canConvert<double>()) {
        return val.toDouble();
    }

    return qQNaN();
}

bool RawLocationParser::parseBool(const QVariant & replyData, const QString & key) const
{
    // replyData is a QVariantMap holding all of the reply parameters
    const QVariantMap positionData = replyData.toMap();

    const QVariant val = positionData.value(key);
    if (val.isValid() && val.canConvert<bool>()) {
        return val.toBool();
    }

    return false;
}

QString RawLocationParser::parseString(const QVariant & replyData, const QString & key) const
{
    // replyData is a QVariantMap holding all of the reply parameters
    const QVariantMap positionData = replyData.toMap();

    const QVariant val = positionData.value(key);
    if (val.isValid() && val.canConvert<QString>()) {
        return val.toString();
    }

    return "";
}

bool RawLocationParser::parseList(QVariantList *out, const QVariant & replyData, const QString & key) const
{
    // replyData is a QVariantMap holding all of the reply parameters
    const QVariantMap positionData = replyData.toMap();

    const QVariant val = positionData.value(key);
    if (val.isValid() && val.canConvert<QVariantList>()) {
        *out = val.toList();
        return true;
    }

    return false;
}

