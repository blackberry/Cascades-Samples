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

#ifndef LOCALDEVICEINFO_HPP
#define LOCALDEVICEINFO_HPP

#include <QObject>

/**
 * @short A class that encapsulates the retrieval of information about the local bluetooth device.
 *
 * The LocalDeviceInfo class provides properties to access the various information of the local
 * bluetooth device.
 */
//! [0]
class LocalDeviceInfo : public QObject
{
    Q_OBJECT

    // The information of the local device as properties
    Q_PROPERTY(QString name READ name NOTIFY changed)
    Q_PROPERTY(QString address READ address NOTIFY changed)
    Q_PROPERTY(QString deviceClass READ deviceClass NOTIFY changed)
    Q_PROPERTY(QString discoverable READ discoverable NOTIFY changed)
    Q_PROPERTY(QString enabled READ enabled NOTIFY changed)
    Q_PROPERTY(QString deviceType READ deviceType NOTIFY changed)
    Q_PROPERTY(QString apiVersion READ apiVersion NOTIFY changed)
    Q_PROPERTY(QString masterSlaveAllowed READ masterSlaveAllowed NOTIFY changed)
    Q_PROPERTY(QString maximumConnections READ maximumConnections NOTIFY changed)
    Q_PROPERTY(QString maximumL2capMtu READ maximumL2capMtu NOTIFY changed)
    Q_PROPERTY(QString maximumRfcommMtu READ maximumRfcommMtu NOTIFY changed)
    Q_PROPERTY(QString incomingInquiryScan READ incomingInquiryScan NOTIFY changed)
    Q_PROPERTY(QString incomingPageScan READ incomingPageScan NOTIFY changed)
    Q_PROPERTY(QString outgoingInquiryScan READ outgoingInquiryScan NOTIFY changed)
    Q_PROPERTY(QString outgoingPageScan READ outgoingPageScan NOTIFY changed)

public:
    // Creates a new LocalDeviceInfo object
    LocalDeviceInfo(QObject *parent = 0);

    // Updates the properties of the object
    void update();

Q_SIGNALS:
    // The change notification signal
    void changed();

private:
    // The accessor methods of the properties
    QString name() const;
    QString address() const;
    QString deviceClass() const;
    QString discoverable() const;
    QString enabled() const;
    QString deviceType() const;
    QString apiVersion() const;
    QString masterSlaveAllowed() const;
    QString maximumConnections() const;
    QString maximumL2capMtu() const;
    QString maximumRfcommMtu() const;
    QString incomingInquiryScan() const;
    QString incomingPageScan() const;
    QString outgoingInquiryScan() const;
    QString outgoingPageScan() const;

    // The property values
    QString m_name;
    QString m_address;
    QString m_deviceClass;
    QString m_discoverable;
    QString m_enabled;
    QString m_deviceType;
    QString m_apiVersion;
    QString m_masterSlaveAllowed;
    QString m_maximumConnections;
    QString m_maximumL2capMtu;
    QString m_maximumRfcommMtu;
    QString m_incomingInquiryScan;
    QString m_incomingPageScan;
    QString m_outgoingInquiryScan;
    QString m_outgoingPageScan;
};
//! [0]

#endif
