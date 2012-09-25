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

#ifndef RAWLOCATIONPARSER_HPP
#define RAWLOCATIONPARSER_HPP

#include <QtCore/QObject>
#include <QtCore/QVariant>

/**
 * A utility class to parse the raw reply from the lower level Location Manager of the OS.
 */
class RawLocationParser
{
public:
    RawLocationParser(const QVariant & replyData);

    double latitude() const;
    double longitude() const;
    double altitude() const;
    double horizontalAccuracy() const;
    double verticalAccuracy() const;
    double heading() const;
    double speed() const;
    double ttff() const;
    double gpsWeek() const;
    double gpsTow() const;
    double utc() const;
    double hdop() const;
    double vdop() const;
    double pdop() const;
    bool propagated() const;
    QString positionMethod() const;

    QString error() const;

    int numberOfSatellites() const;
    double satelliteId(int satIndex) const;
    double satelliteCarrierToNoiseRatio(int satIndex) const;
    bool satelliteEphemerisAvailable(int satIndex) const;
    double satelliteAzimuth(int satIndex) const;
    double satelliteElevation(int satIndex) const;
    bool satelliteTracked(int satIndex) const;
    bool satelliteUsed(int satIndex) const;
    bool satelliteAlmanac(int satIndex) const;

private:
    double parseDouble(const QVariant & replyData, const QString & key) const;
    bool parseBool(const QVariant & replyData, const QString & key) const;
    QString parseString(const QVariant & replyData, const QString & key) const;
    bool parseList(QVariantList *out, const QVariant & replyData, const QString & key) const;

    QVariant m_replyData;
};

#endif
