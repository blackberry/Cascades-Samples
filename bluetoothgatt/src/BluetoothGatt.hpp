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

#ifndef BLUETOOTHGATT_HPP
#define BLUETOOTHGATT_HPP

#include <QObject>

#include <bb/cascades/ArrayDataModel>

#include <btgatt.h>
#include <btle.h>
#include <btdevice.h>

class CharacteristicsEditor;

//! [0]
class BluetoothGatt : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString activeDevice READ activeDevice WRITE setActiveDevice NOTIFY activeDeviceChanged)
    Q_PROPERTY(QString activeService READ activeService WRITE setActiveService NOTIFY activeServiceChanged)
    Q_PROPERTY(QString activeCharacteristic READ activeCharacteristic WRITE setActiveCharacteristic NOTIFY activeCharacteristicChanged)

    Q_PROPERTY(bb::cascades::DataModel* devicesModel READ devicesModel CONSTANT)
    Q_PROPERTY(bb::cascades::DataModel* servicesModel READ servicesModel CONSTANT)
    Q_PROPERTY(bb::cascades::DataModel* characteristicsModel READ characteristicsModel CONSTANT)

    Q_PROPERTY(CharacteristicsEditor* editor READ editor CONSTANT)
    Q_PROPERTY(QString errorMessage READ errorMessage NOTIFY errorMessageChanged)

public:
    BluetoothGatt(QObject *parent = 0);
    virtual ~BluetoothGatt();

    // called by callbacks from the bluetooth library
    void gattServiceConnected(const QString &bdaddr, const QString &service, int instance, int err, uint16_t connInt, uint16_t latency, uint16_t superTimeout );
    void processBluetoothEvent(const int event, void *data);

Q_SIGNALS:
    // The change notification signals of the properties
    void activeDeviceChanged();
    void activeServiceChanged();
    void activeCharacteristicChanged();
    void errorMessageChanged();

    void bluetoothEvent(const int event, void *data);

public slots:
    bool viewCharacteristics(int row);
    void viewCharacteristicsEditor(int row);
    void viewServices(int row);
    void refreshDevices();

    void disconnectServices();
    void resetEditor();
    void resetCharacteristicsList();
    void handleBluetoothEvent(const int event, void *data);
    void handleCharacteristicNotificationsEnabledChanged(bool);

private:
    // The accessor methods for the properties
    QString activeDevice() const;
    void setActiveDevice(const QString&);
    QString activeDeviceName() const;
    void setActiveDeviceName(const QString&);
    QString activeService() const;
    void setActiveService(const QString&);
    QString activeCharacteristic() const;
    void setActiveCharacteristic(const QString&);

    bb::cascades::DataModel* devicesModel() const;
    bb::cascades::DataModel* servicesModel() const;
    bb::cascades::DataModel* characteristicsModel() const;
    CharacteristicsEditor* editor() const;
    QString errorMessage() const;

    void setErrorMessage(const QString &errorMessage);

    void gattServiceDisconnected(const QString &deviceAddress, const QString &uuid, int instance, int reason);
    void gattNotification(int instance, uint16_t handle, const uint8_t *val, uint16_t len);

    QString m_activeDevice;
    QString m_activeDeviceName;
    QString m_activeService;
    QString m_activeCharacteristic;

    bb::cascades::ArrayDataModel *m_devices;
    bb::cascades::ArrayDataModel *m_services;
    bb::cascades::ArrayDataModel *m_characteristics;

    bt_gatt_callbacks_t m_gatt_cb;
    int m_gattInstance;
    bt_gatt_characteristic_t *m_characteristicList;
    bt_gatt_characteristic_t *m_characteristicStruct;
    int m_characteristicListSize;

    CharacteristicsEditor *m_editor;
    QString m_errorMessage;
};
//! [0]

#endif
