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

#ifndef REMOTEDEVICEINFO_HPP
#define REMOTEDEVICEINFO_HPP

#include <QObject>

#include <bb/cascades/GroupDataModel>

/**
 * @short A class that encapsulates the retrieval of information about a remote bluetooth device.
 *
 * The RemoteDeviceInfo class provides properties to access the various information of an remote
 * bluetooth device.
 */
//! [0]
class RemoteDeviceInfo : public QObject
{
    Q_OBJECT

    // The information of the local device as properties
    Q_PROPERTY(bb::cascades::DataModel* model READ model CONSTANT)
    Q_PROPERTY(QString name READ name NOTIFY changed)
    Q_PROPERTY(QString address READ address NOTIFY changed)
    Q_PROPERTY(QString deviceClass READ deviceClass NOTIFY changed)
    Q_PROPERTY(QString deviceType READ deviceType NOTIFY changed)
    Q_PROPERTY(QString encrypted READ encrypted NOTIFY changed)
    Q_PROPERTY(QString paired READ paired NOTIFY changed)
    Q_PROPERTY(QString trusted READ trusted NOTIFY changed)
    Q_PROPERTY(QString rssi READ rssi NOTIFY changed)
    Q_PROPERTY(QString minimumConnectionInterval READ minimumConnectionInterval NOTIFY changed)
    Q_PROPERTY(QString maximumConnectionInterval READ maximumConnectionInterval NOTIFY changed)
    Q_PROPERTY(QString latency READ latency NOTIFY changed)
    Q_PROPERTY(QString supervisoryTimeout READ supervisoryTimeout NOTIFY changed)
    Q_PROPERTY(QString appearance READ appearance NOTIFY changed)
    Q_PROPERTY(QString flags READ flags NOTIFY changed)
    Q_PROPERTY(QString connectable READ connectable NOTIFY changed)

public:
    // Creates a new RemoteDeviceInfo object
    RemoteDeviceInfo(QObject *parent = 0);

    // Updates the properties of the object for the device with the given address
    void update(const QString &deviceAddress);

Q_SIGNALS:
    // The change notification signal
    void changed();

private:
    // The accessor methods of the properties
    bb::cascades::DataModel* model() const;
    QString name() const;
    QString address() const;
    QString deviceClass() const;
    QString deviceType() const;
    QString encrypted() const;
    QString paired() const;
    QString trusted() const;
    QString rssi() const;
    QString minimumConnectionInterval() const;
    QString maximumConnectionInterval() const;
    QString latency() const;
    QString supervisoryTimeout() const;
    QString appearance() const;
    QString flags() const;
    QString connectable() const;

    // The property values
    bb::cascades::GroupDataModel* m_model;
    QString m_name;
    QString m_address;
    QString m_deviceClass;
    QString m_deviceType;
    QString m_encrypted;
    QString m_paired;
    QString m_trusted;
    QString m_rssi;
    QString m_minimumConnectionInterval;
    QString m_maximumConnectionInterval;
    QString m_latency;
    QString m_supervisoryTimeout;
    QString m_appearance;
    QString m_flags;
    QString m_connectable;
};
//! [0]

#endif
