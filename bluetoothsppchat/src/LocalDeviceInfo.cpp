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

#include "LocalDeviceInfo.hpp"

#include <btapi/btdevice.h>
#include <btapi/btspp.h>

LocalDeviceInfo::LocalDeviceInfo(QObject *parent)
    : QObject(parent)
{
}

//! [0]
void LocalDeviceInfo::update()
{
    char buffer[128];
    const int bufferSize = sizeof(buffer);

    bool ok = false;

    const QString unknown = tr("Unknown");
    const QString notAvailable = tr("N/A");

    ok = (bt_ldev_get_friendly_name(buffer, bufferSize) == 0);
    m_name = (ok ? QString::fromLatin1(buffer) : unknown);

    ok = (bt_ldev_get_address(buffer) == 0);
    m_address = (ok ? QString::fromLatin1(buffer) : unknown);

    const int code = bt_ldev_get_device_class(BT_COD_DEVICECLASS);
    if (code >= 0) {
        m_deviceClass.sprintf("0x%x", code);
    } else {
        m_deviceClass = unknown;
    }

    m_discoverable = ((bt_ldev_get_discoverable() == BT_DISCOVERABLE_GIAC) ? tr("true") : tr("false"));
    m_enabled = (bt_ldev_get_power() ? tr("true") : tr("false"));

    const int deviceType = bt_ldev_get_type();
    m_deviceType = ((deviceType == BT_DEVICE_TYPE_LE_PUBLIC || deviceType == BT_DEVICE_TYPE_LE_PRIVATE) ?  tr("Low energy") : tr("Regular"));

    ok = (bt_ldev_get_property(BT_PROP_API_VERSION, buffer, bufferSize) == 0);
    m_apiVersion = (ok ? QString::fromLatin1(buffer) : notAvailable);

    ok = (bt_ldev_get_property(BT_PROP_MASTER_SLAVE_SWITCH, buffer, bufferSize) == 0);
    m_masterSlaveAllowed = (ok ? QString::fromLatin1(buffer) : notAvailable);

    ok = (bt_ldev_get_property(BT_PROP_MAX_CONNECTED_DEVICES, buffer, bufferSize) == 0);
    m_maximumConnections = (ok ? QString::fromLatin1(buffer) : notAvailable);

    ok = (bt_ldev_get_property(BT_PROP_MAX_L2CAP_RCVMTU, buffer, bufferSize) == 0);
    m_maximumL2capMtu = (ok ? QString::fromLatin1(buffer) : notAvailable);

    ok = (bt_ldev_get_property(BT_PROP_MAX_RFCOMM_RCVMTU, buffer, bufferSize) == 0);
    m_maximumRfcommMtu = (ok ? QString::fromLatin1(buffer) : notAvailable);

    ok = (bt_ldev_get_property(BT_PROP_CONNECTED_INQUIRY_SCAN, buffer, bufferSize) == 0);
    m_incomingInquiryScan = (ok ? QString::fromLatin1(buffer) : notAvailable);

    ok = (bt_ldev_get_property(BT_PROP_CONNECTED_PAGE_SCAN, buffer, bufferSize) == 0);
    m_incomingPageScan = (ok ? QString::fromLatin1(buffer) : notAvailable);

    ok = (bt_ldev_get_property(BT_PROP_CONNECTED_INQUIRY, buffer, bufferSize) == 0);
    m_outgoingInquiryScan = (ok ? QString::fromLatin1(buffer) : notAvailable);

    ok = (bt_ldev_get_property(BT_PROP_CONNECTED_PAGE, buffer, bufferSize) == 0);
    m_outgoingPageScan = (ok ? QString::fromLatin1(buffer) : notAvailable);

    emit changed();
}
//! [0]

QString LocalDeviceInfo::name() const
{
    return m_name;
}

QString LocalDeviceInfo::address() const
{
    return m_address;
}

QString LocalDeviceInfo::deviceClass() const
{
    return m_deviceClass;
}

QString LocalDeviceInfo::discoverable() const
{
    return m_discoverable;
}

QString LocalDeviceInfo::enabled() const
{
    return m_enabled;
}

QString LocalDeviceInfo::deviceType() const
{
    return m_deviceType;
}

QString LocalDeviceInfo::apiVersion() const
{
    return m_apiVersion;
}

QString LocalDeviceInfo::masterSlaveAllowed() const
{
    return m_masterSlaveAllowed;
}

QString LocalDeviceInfo::maximumConnections() const
{
    return m_maximumConnections;
}

QString LocalDeviceInfo::maximumL2capMtu() const
{
    return m_maximumL2capMtu;
}

QString LocalDeviceInfo::maximumRfcommMtu() const
{
    return m_maximumRfcommMtu;
}

QString LocalDeviceInfo::incomingInquiryScan() const
{
    return m_incomingInquiryScan;
}

QString LocalDeviceInfo::incomingPageScan() const
{
    return m_incomingPageScan;
}

QString LocalDeviceInfo::outgoingInquiryScan() const
{
    return m_outgoingInquiryScan;
}

QString LocalDeviceInfo::outgoingPageScan() const
{
    return m_outgoingPageScan;
}
