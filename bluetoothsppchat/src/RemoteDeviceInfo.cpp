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

#include "RemoteDeviceInfo.hpp"

#include <btapi/btdevice.h>
#include <btapi/btspp.h>

//! [0]
RemoteDeviceInfo::RemoteDeviceInfo(QObject *parent)
    : QObject(parent)
    , m_model(new bb::cascades::GroupDataModel(QStringList() << "uuid" << "address" << "serviceType", this))
{
    m_model->setSortingKeys(QStringList() << "serviceType");
    m_model->setGrouping(bb::cascades::ItemGrouping::ByFullValue);
}
//! [0]

//! [1]
void RemoteDeviceInfo::update(const QString &deviceAddress)
{
    bt_remote_device_t *remote_device = bt_rdev_get_device(deviceAddress.toAscii());

    if (!remote_device)
        return;

    bool ok = false;
    char buffer[128];
    const int bufferSize = sizeof(buffer);

    const QString unknown = tr("Unknown");
    const QString notAvailable = tr("N/A");

    // Display all known basic device information.
    ok = (bt_rdev_get_friendly_name(remote_device, buffer, bufferSize) == 0);
    m_name = (ok ? QString::fromLatin1(buffer) : unknown);

    m_address = deviceAddress;

    const int deviceClass = bt_rdev_get_device_class(remote_device, BT_COD_DEVICECLASS);
    if (deviceClass >= 0) {
        m_deviceClass.sprintf("0x%x", deviceClass);
    } else {
        m_deviceClass = unknown;
    }

    const int deviceType = bt_rdev_get_type(remote_device);
    m_deviceType = ((deviceType == BT_DEVICE_TYPE_LE_PUBLIC || deviceType == BT_DEVICE_TYPE_LE_PUBLIC) ? tr("Low energy") : tr("Regular"));

    m_encrypted = ((bt_rdev_is_encrypted(remote_device) >= 0) ? tr("true") : tr("false"));

    bool paired = false;
    ok = (bt_rdev_is_paired(remote_device, &paired) == 0);
    m_paired = (ok ? (paired ? tr("true") : tr("false")) : unknown);

    m_trusted = (bt_rdev_is_trusted(remote_device) ? tr("true") : tr("false"));

    int rssi = 0;
    ok = (bt_rdev_get_rssi(remote_device, &rssi) == 0);
    m_rssi = (ok ? QString::number(rssi) : unknown);

    // Display all known low energy parameters.
    uint16_t min_conn_ivl, max_conn_ivl, latency, super_tmo, appearance;
    ok = (bt_rdev_get_le_conn_params(remote_device, &min_conn_ivl, &max_conn_ivl, &latency, &super_tmo) == 0);

    m_minimumConnectionInterval = (ok ? QString::number(min_conn_ivl) : notAvailable);
    m_maximumConnectionInterval = (ok ? QString::number(max_conn_ivl) : notAvailable);
    m_latency = (ok ? QString::number(latency) : notAvailable);
    m_supervisoryTimeout = (ok ? QString::number(super_tmo) : notAvailable);

    uint8_t flags, connectable;
    ok = (bt_rdev_get_le_info(remote_device, &appearance, &flags, &connectable) == 0);

    m_appearance = (ok ? QString::number(appearance) : notAvailable);
    m_flags = (ok ? QString::number(flags) : notAvailable);
    m_connectable = (ok ? QString::number(connectable) : notAvailable);

    // Display any found regular Bluetooth services.
    char **services_array = bt_rdev_get_services(remote_device);
    if (services_array) {
        for (int i = 0; services_array[i]; i++) {
            QVariantMap map;

            const QString uuid = QString::fromLatin1(services_array[i]);

            map["uuid"] = uuid;
            map["address"] = m_address;

            if (uuid.startsWith("0x0001"))
                map["serviceType"] = tr("SDP");
            else if (uuid.startsWith("0x0003"))
                map["serviceType"] = tr("RFCOMM");
            else if (uuid.startsWith("0x0008"))
                map["serviceType"] = tr("OBEX");
            else if (uuid.startsWith("0x000c"))
                map["serviceType"] = tr("HTTP");
            else if (uuid.startsWith("0x0100"))
                map["serviceType"] = tr("L2CAP");
            else if (uuid.startsWith("0x000f"))
                map["serviceType"] = tr("BNEP");
            else if (uuid.startsWith("0x1000"))
                map["serviceType"] = tr("Service Discovery");
            else if (uuid.startsWith("0x1001"))
                map["serviceType"] = tr("Browse Group Descriptor");
            else if (uuid.startsWith("0x1002"))
                map["serviceType"] = tr("Public Browse Group");
            else if (uuid.startsWith("0x1101"))
                map["serviceType"] = tr("Serial Port");
            else if (uuid.startsWith("0x1102"))
                map["serviceType"] = tr("Public Browse Group");
            else if (uuid.startsWith("0x1105"))
                map["serviceType"] = tr("OBEX Object Push");
            else if (uuid.startsWith("0x1106"))
                map["serviceType"] = tr("OBEX File Transfer");
            else if (uuid.startsWith("0x1115"))
                map["serviceType"] = tr("Personal Area Networking");
            else if (uuid.startsWith("0x1116"))
                map["serviceType"] = tr("Network Access Point");
            else if (uuid.startsWith("0x1117"))
                map["serviceType"] = tr("Group Network");
            else
                map["serviceType"] = tr("Other");

            m_model->insert(map);
        }

        bt_rdev_free_services(services_array);
    }

    // Display any found Bluetooth low energy services.
    if (deviceType == BT_DEVICE_TYPE_LE_PUBLIC || deviceType == BT_DEVICE_TYPE_LE_PRIVATE) {
        services_array = bt_rdev_get_services_gatt(remote_device);
        if (services_array) {
            for (int i = 0; services_array[i]; i++) {
                QVariantMap map;

                map["uuid"] = QString::fromLatin1(services_array[i]);
                map["address"] = m_address;
                map["serviceType"] = tr("GATT");

                m_model->insert(map);
            }

            bt_rdev_free_services(services_array);
        }
    }

    bt_rdev_free(remote_device);

    emit changed();
}
//! [1]

bb::cascades::DataModel* RemoteDeviceInfo::model() const
{
    return m_model;
}

QString RemoteDeviceInfo::name() const
{
    return m_name;
}

QString RemoteDeviceInfo::address() const
{
    return m_address;
}

QString RemoteDeviceInfo::deviceClass() const
{
    return m_deviceClass;
}

QString RemoteDeviceInfo::deviceType() const
{
    return m_deviceType;
}

QString RemoteDeviceInfo::encrypted() const
{
    return m_encrypted;
}

QString RemoteDeviceInfo::paired() const
{
    return m_paired;
}

QString RemoteDeviceInfo::trusted() const
{
    return m_trusted;
}

QString RemoteDeviceInfo::rssi() const
{
    return m_rssi;
}

QString RemoteDeviceInfo::minimumConnectionInterval() const
{
    return m_minimumConnectionInterval;
}

QString RemoteDeviceInfo::maximumConnectionInterval() const
{
    return m_maximumConnectionInterval;
}

QString RemoteDeviceInfo::latency() const
{
    return m_latency;
}

QString RemoteDeviceInfo::supervisoryTimeout() const
{
    return m_supervisoryTimeout;
}

QString RemoteDeviceInfo::appearance() const
{
    return m_appearance;
}

QString RemoteDeviceInfo::flags() const
{
    return m_flags;
}

QString RemoteDeviceInfo::connectable() const
{
    return m_connectable;
}
