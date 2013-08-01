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
#ifndef CHARACTERISTICSEDITOR_HPP
#define CHARACTERISTICSEDITOR_HPP

#include <QObject>

#include <bb/cascades/ArrayDataModel>

#include <btgatt.h>

class CharacteristicsEditor : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString characteristicUUID READ characteristicUUID NOTIFY characteristicUUIDChanged)
    Q_PROPERTY(QString characteristicValueHandle READ characteristicValueHandle NOTIFY characteristicValueHandleChanged)
    Q_PROPERTY(QString characteristicName READ characteristicName NOTIFY characteristicNameChanged)
    Q_PROPERTY(QString characteristicHandle READ characteristicHandle NOTIFY characteristicHandleChanged)
    Q_PROPERTY(QString characteristicFlags READ  characteristicFlags NOTIFY characteristicFlagsChanged)
    Q_PROPERTY(QString characteristicValue READ characteristicValue WRITE setCharacteristicValue NOTIFY characteristicValueChanged)
    Q_PROPERTY(QString characteristicValueText READ characteristicValueText NOTIFY characteristicValueTextChanged)
    Q_PROPERTY(bool characteristicNotificationsEnabled READ characteristicNotificationsEnabled WRITE setCharacteristicNotificationsEnabled NOTIFY characteristicNotificationsEnabledChanged)
    Q_PROPERTY(bool characteristicNotificationsAllowed READ characteristicNotificationsAllowed)
    Q_PROPERTY(bool characteristicIndicationsEnabled READ characteristicIndicationsEnabled WRITE setCharacteristicIndicationsEnabled NOTIFY characteristicIndicationsEnabledChanged)
    Q_PROPERTY(bool characteristicIndicationsAllowed READ characteristicIndicationsAllowed)
    Q_PROPERTY(bb::cascades::DataModel* descriptorsModel READ descriptorsModel CONSTANT)
    Q_PROPERTY(QString errorMessage READ errorMessage NOTIFY errorMessageChanged)

public:
    CharacteristicsEditor(QObject *parent = 0);

    void setGattInstance(int gattInstance);
    void setCharacteristic(bt_gatt_characteristic_t *characteristic);

    void updateCharacteristicValue(const uint8_t *value, uint16_t length);

public Q_SLOTS:
    void readCharacteristicValue();
    void writeCharacteristicValue(bool withResponse);
    void readCharacteristicDescriptor(int row);
    void writeCharacteristicDescriptor(int row, const QString &value);

Q_SIGNALS:
    void characteristicUUIDChanged();
    void characteristicValueHandleChanged();
    void characteristicHandleChanged();
    void characteristicNameChanged();
    void characteristicFlagsChanged();
    void characteristicValueChanged();
    void characteristicValueTextChanged();
    void characteristicNotificationsEnabledChanged(bool);
    void characteristicIndicationsEnabledChanged();
    void errorMessageChanged();

private:
    QString characteristicUUID() const;
    QString characteristicValueHandle() const;
    QString characteristicName() const;
    QString characteristicValue() const;
    void setCharacteristicValue(const QString&);
    QString characteristicValueText() const;
    void setCharacteristicValueText(const QString&);
    QString characteristicHandle() const;
    QString characteristicFlags() const;
    bool characteristicNotificationsEnabled() const;
    void setCharacteristicNotificationsEnabled(bool);
    bool characteristicNotificationsAllowed() const;
    bool characteristicIndicationsEnabled() const;
    void setCharacteristicIndicationsEnabled(bool);
    bool characteristicIndicationsAllowed() const;
    bb::cascades::DataModel* descriptorsModel() const;
    QString errorMessage() const;

    void setErrorMessage(const QString &errorMessage);

    QString m_characteristicValue;
    QString m_characteristicValueText;
    QString m_characteristicUUID;
    QString m_characteristicName;
    QString m_characteristicValueHandle;
    QString m_characteristicHandle;
    QString m_characteristicFlags;
    bool m_characteristicNotificationsEnabled;
    bool m_characteristicIndicationsEnabled;
    bb::cascades::ArrayDataModel *m_descriptorsModel;

    int m_gattInstance;
    bt_gatt_characteristic_t *m_characteristicStruct;
    int m_characteristicMTU;
    bt_gatt_char_prop_mask m_characteristicProperties;
    QString m_errorMessage;
};

#endif
