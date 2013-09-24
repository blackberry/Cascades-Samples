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

#include "CharacteristicsEditor.hpp"

#include "TypedArrayDataModel.hpp"
#include "Util.hpp"
#include "parse.h"

#include <arpa/inet.h>
#include <errno.h>

#define MIN_GATT_MTU (16)

// Turn a binary buffer into a string of printable, hexadecimal characters
static QString bufferToString(const uint8_t *buffer, int byteCount)
{
    QString result;

    QString temp;
    for (int i = 0; i < byteCount; i++) {
        if (i > 0 && (i % 2) == 0)
            result.append(' ');

        result.append(temp.sprintf("%.2x", buffer[i]));
    }

    return result;
}

static inline int hexValue(char inChar)
{
    if (isxdigit(inChar)) {
        if (isdigit(inChar)) {
            return (inChar - '0');
        } else {
            return (toupper(inChar) - 'A' + 10);
        }
    } else {
        return -1;
    }
}

// Turn the string of printable, hexadecimal characters into a binary buffer.
static int stringToBuffer(const QString &stringData, uint8_t *buffer, int bufferLength)
{
    int consumed = 0;
    for (int i = 0; i < bufferLength; i++) {
        const int hex = hexValue(stringData.at(i).toAscii());
        if (hex >= 0) {
            if ((consumed % 2) == 0) {
                buffer[(consumed / 2)] = hex << 4;
            } else {
                buffer[(consumed / 2)] |= hex;
            }

            consumed++;
        }
    }

    // Round up the number of bytes we consumed to a multiple of 2.
    if ((consumed % 2) != 0)
        ++consumed;

    return consumed;
}


CharacteristicsEditor::CharacteristicsEditor(QObject *parent)
    : QObject(parent)
    , m_descriptorsModel(new TypedArrayDataModel())
    , m_gattInstance(0)
    , m_characteristicStruct(0)
    , m_characteristicMTU(0)
    , m_characteristicProperties(0)
{
    m_descriptorsModel->setParent(this);
}

void CharacteristicsEditor::setGattInstance(int gattInstance)
{
    m_gattInstance = gattInstance;
}

void CharacteristicsEditor::setCharacteristic(bt_gatt_characteristic_t *characteristic)
{
    m_characteristicStruct = characteristic;

    m_characteristicUUID = m_characteristicStruct->uuid;
    m_characteristicHandle = QString::number(m_characteristicStruct->handle);
    m_characteristicName = Util::parse_characteristic_uuid(m_characteristicStruct->uuid);
    m_characteristicValueHandle = QString::number(m_characteristicStruct->value_handle);
    m_characteristicProperties = m_characteristicStruct->properties;
    m_characteristicFlags.sprintf("0x%.2x", m_characteristicProperties);
    m_characteristicValue.clear();
    m_characteristicValueText.clear();
    m_characteristicNotificationsEnabled = false;
    m_characteristicIndicationsEnabled = false;

    int count = bt_gatt_descriptors_count(m_gattInstance, m_characteristicStruct);
    if (count == -1) {
        qWarning() << "GATT descriptors count failed:" << errno << "(" << strerror(errno) << ")";
        bt_gatt_disconnect_instance(m_gattInstance);
        return;
    }

    bt_gatt_descriptor_t *descriptorList = 0;
    if (count > 0) {
        descriptorList = (bt_gatt_descriptor_t*)alloca(count * sizeof(bt_gatt_descriptor_t));
        if (!descriptorList) {
            setErrorMessage("GATT descriptors: Not enough memory");
            bt_gatt_disconnect_instance(m_gattInstance);
            return;
        }

        /* BEGIN WORKAROUND - Temporary fix to address race condition */
        int number = 0;
        do {
            number = bt_gatt_descriptors(m_gattInstance, m_characteristicStruct, descriptorList, count);
        } while ((number == -1) && (errno == EBUSY));

        count = number;
        /* END WORKAROUND */
    }

    if (count == -1) {
        setErrorMessage(QString("GATT descriptors failed: %1 (%2)").arg(errno).arg(strerror(errno)));
        bt_gatt_disconnect_instance(m_gattInstance);
        return;
    }

    m_characteristicMTU = bt_gatt_get_mtu(m_gattInstance);
    if (m_characteristicMTU < MIN_GATT_MTU)
        m_characteristicMTU = MIN_GATT_MTU;

    uint8_t *descriptorBuffer = (uint8_t *)alloca(m_characteristicMTU);
    if (!descriptorBuffer) {
        setErrorMessage("GATT descriptors: Not enough memory");
        bt_gatt_disconnect_instance(m_gattInstance);
        return;
    }

    for (int i = 0; i < count; i++) {
        QVariantMap map;

        map["uuid"] = QString(descriptorList[i].uuid).arg(i);
        map["name"] = QString(Util::parse_descriptor_uuid( descriptorList[i].uuid )).arg(i);
        map["handle"] = descriptorList[i].handle;

        QString descriptorString;
        uint8_t more = 1;
        int byteCount;
        for (int offset = 0; more; offset += byteCount) {
            byteCount = bt_gatt_read_value(m_gattInstance, descriptorList[i].handle, offset, descriptorBuffer, m_characteristicMTU, &more);
            if (byteCount < 0) {
                setErrorMessage(QString("Unable to read descriptor value: %1 (%2)").arg(errno).arg(strerror(errno)));
                break;
            }

            descriptorString += bufferToString(descriptorBuffer, byteCount);
        }

        map["value"] = descriptorString;

        m_descriptorsModel->append(map);

        // Check for the Client Characteristic Configuration - are notifications on already?
        if (strcmp(descriptorList[i].uuid, "0x2902" ) == 0) {
            // Parse the value into host byte order
            uint16_t value = 0;
            if (sscanf(descriptorString.toAscii().constData(), "%04hx", &value) == 1) {
                value = htons(value);
                setCharacteristicNotificationsEnabled(value & 0x01);
                setCharacteristicIndicationsEnabled(value & 0x02);
            }
        }
    }
}

void CharacteristicsEditor::updateCharacteristicValue(const uint8_t *value, uint16_t length)
{
    const QString textValue = bufferToString(value, length);

    setCharacteristicValue(textValue);
    setCharacteristicValueText(textValue);
}

void CharacteristicsEditor::readCharacteristicValue()
{
    if ((m_characteristicProperties & GATT_CHARACTERISTIC_PROP_READ) == 0) {
        setErrorMessage("GATT descriptors: Read not permitted");
        return;
    }

    uint8_t *characteristicBuffer = (uint8_t *)alloca(m_characteristicMTU);
    if (!characteristicBuffer) {
        setErrorMessage("GATT characteristic: Not enough memory");
        bt_gatt_disconnect_instance(m_gattInstance);
        return;
    }

    QString descriptorString;

    int byteCount = 0;
    uint8_t more = 1;
    for (int offset = 0; more; offset += byteCount) {
        byteCount = bt_gatt_read_value(m_gattInstance, m_characteristicValueHandle.toUShort(), offset, characteristicBuffer, m_characteristicMTU, &more);
        if (byteCount < 0) {
            setErrorMessage(QString("Unable to read characteristic value: %1 (%2)").arg(errno).arg(strerror(errno)));
            break;
        }

        descriptorString += bufferToString(characteristicBuffer, byteCount);
    }

    setCharacteristicValue(descriptorString);
    setCharacteristicValueText(descriptorString);
}

void CharacteristicsEditor::writeCharacteristicValue(bool withResponse)
{
    if ((withResponse && (m_characteristicProperties & GATT_CHARACTERISTIC_PROP_WRITE) == 0) ||
        (!withResponse && (m_characteristicProperties & GATT_CHARACTERISTIC_PROP_WRITE_NORESP) == 0)) {
        setErrorMessage("GATT descriptors: Write not permitted");
        return;
    }

    const int characteristicLen = m_characteristicValue.size();
    uint8_t *characteristicBuffer = (uint8_t *)alloca(characteristicLen / 2 + 1);
    if (!characteristicBuffer) {
        setErrorMessage("GATT characteristic: Not enough memory");
        bt_gatt_disconnect_instance(m_gattInstance);
        return;
    }

    const int consumed = stringToBuffer(m_characteristicValue, characteristicBuffer, characteristicLen);

    if (consumed > 0) {
        int byteCount;
        if (withResponse) {
            byteCount = bt_gatt_write_value(m_gattInstance, m_characteristicValueHandle.toUShort(), 0, characteristicBuffer, (consumed / 2));
        } else {
            byteCount = bt_gatt_write_value_noresp(m_gattInstance, m_characteristicValueHandle.toUShort(), 0, characteristicBuffer, (consumed / 2));
        }

        if (byteCount < 0) {
            setErrorMessage(QString("Unable to write characteristic value: %1 (%2)").arg(errno).arg(strerror(errno)));
        }
    }
}

void CharacteristicsEditor::readCharacteristicDescriptor(int row)
{
    if (row < 0 || row >= m_descriptorsModel->size())
        return;

    uint8_t *descriptorBuffer;
    if (m_characteristicMTU < MIN_GATT_MTU) {
        m_characteristicMTU = bt_gatt_get_mtu(m_gattInstance);
        if (m_characteristicMTU < MIN_GATT_MTU) {
            m_characteristicMTU = MIN_GATT_MTU;
        }
    }

    descriptorBuffer = (uint8_t *)alloca(m_characteristicMTU);
    if (0 == descriptorBuffer) {
        setErrorMessage("GATT descriptors: Not enough memory");
        bt_gatt_disconnect_instance(m_gattInstance);
        return;
    }

    QVariantMap currentMap = m_descriptorsModel->value(row).toMap();

    QString descriptorString;

    int byteCount = 0;
    uint8_t more = 1;
    for (int offset = 0; more; offset += byteCount) {
        byteCount = bt_gatt_read_value(m_gattInstance, currentMap["handle"].toInt(), offset, descriptorBuffer, m_characteristicMTU, &more);
        if (byteCount < 0) {
            setErrorMessage(QString("Unable to read descriptor value: %1 (%2)").arg(errno).arg(strerror(errno)));
            break;
        }

        descriptorString += bufferToString(descriptorBuffer, byteCount);
    }

    currentMap["value"] = descriptorString;

    m_descriptorsModel->replace(row, currentMap);
}

void CharacteristicsEditor::writeCharacteristicDescriptor(int row, const QString &value)
{
    if (row < 0 || row >= m_descriptorsModel->size())
        return;

    QVariantMap currentMap = m_descriptorsModel->value(row).toMap();

    const int descriptorLen = value.size();

    uint8_t *descriptorBuffer = (uint8_t *)alloca( value.size() / 2 + 1 );
    if (!descriptorBuffer) {
        setErrorMessage("GATT descriptor: Not enough memory");
        bt_gatt_disconnect_instance(m_gattInstance);
        return;
    }

    const int consumed = stringToBuffer(value, descriptorBuffer, descriptorLen);

    if (consumed > 0) {
        const int byteCount = bt_gatt_write_value(m_gattInstance, currentMap["handle"].toInt(), 0, descriptorBuffer, (consumed / 2) );
        if (byteCount < 0) {
            setErrorMessage(QString("Unable to write descriptor value: %1 (%2)").arg(errno).arg(strerror(errno)));
        } else {
            currentMap["value"] = value;

            m_descriptorsModel->replace(row, currentMap);
        }
    }
}

QString CharacteristicsEditor::characteristicUUID() const
{
    return m_characteristicUUID;
}

QString CharacteristicsEditor::characteristicValueHandle() const
{
    return m_characteristicValueHandle;
}

QString CharacteristicsEditor::characteristicValue() const
{
    return m_characteristicValue;
}

void CharacteristicsEditor::setCharacteristicValue(const QString &in)
{
    if (in != m_characteristicValue) {
        m_characteristicValue = in;
        emit characteristicValueChanged();
    }
}

QString CharacteristicsEditor::characteristicValueText() const
{
    return m_characteristicValueText;
}

void CharacteristicsEditor::setCharacteristicValueText(const QString &in)
{
    m_characteristicValueText.clear();
    char out[1024];
    int characteristicLen = in.size();
    int i, consumed = 0;
    uint8_t *data = (uint8_t *)alloca((characteristicLen / 2) + 1);

    memset(data, 0, (characteristicLen / 2) + 1);

    if (0 == data) return;

    for(i = 0; i < characteristicLen; i++ ) {
        int hex = hexValue( in.at( i ).toAscii() );
        if( hex >= 0 ) {
            if( ( consumed % 2 ) == 0 ) {
                data[consumed/2] = hex << 4;
            } else {
                data[consumed/2] |= hex;
            }
            consumed++;
        }
    }

    int len = parse_characteristic_uuid_buffer(m_characteristicUUID.toAscii().constData(), data, consumed / 2, out, sizeof(out));

    /* Could not parse entry */
    if (len < 0) {

        for (i = 0; i < consumed / 2; i++) {
            m_characteristicValueText.append( isprint( data[i] )? data[i] : '.' );
        }
    } else {
        m_characteristicValueText.append(out);
    }

    emit characteristicValueTextChanged();
}

QString CharacteristicsEditor::characteristicHandle() const
{
    return m_characteristicHandle;
}

QString CharacteristicsEditor::characteristicFlags() const
{
    return m_characteristicFlags;
}

bool CharacteristicsEditor::characteristicNotificationsEnabled() const
{
    return m_characteristicNotificationsEnabled;
}

void CharacteristicsEditor::setCharacteristicNotificationsEnabled(bool in)
{
    if (in != m_characteristicNotificationsEnabled) {
        if ((m_characteristicProperties & GATT_CHARACTERISTIC_PROP_NOTIFY) == 0) {
            setErrorMessage("Notification changes not allowed");
            emit characteristicNotificationsEnabledChanged(m_characteristicNotificationsEnabled);
            return;
        }
        m_characteristicNotificationsEnabled = in;
        emit characteristicNotificationsEnabledChanged(m_characteristicNotificationsEnabled);
    }
    qDebug() << "Notifications" << in;
}

bool CharacteristicsEditor::characteristicNotificationsAllowed() const
{
    return (m_characteristicProperties & GATT_CHARACTERISTIC_PROP_NOTIFY);
}

bool CharacteristicsEditor::characteristicIndicationsEnabled() const
{
    return m_characteristicIndicationsEnabled;
}

void CharacteristicsEditor::setCharacteristicIndicationsEnabled(bool in)
{
    if (in != m_characteristicIndicationsEnabled) {
        if ( ( m_characteristicProperties & GATT_CHARACTERISTIC_PROP_INDICATE ) == 0 ) {
            setErrorMessage("Indication changes not allowed");
            emit characteristicIndicationsEnabledChanged();
            return;
        }
        m_characteristicIndicationsEnabled = in;
        emit characteristicIndicationsEnabledChanged();
    }
    qDebug() << "Indications" << in;
}

bool CharacteristicsEditor::characteristicIndicationsAllowed() const
{
    return ( m_characteristicProperties & GATT_CHARACTERISTIC_PROP_INDICATE );
}

QString CharacteristicsEditor::characteristicName() const
{
    return m_characteristicName;
}

bb::cascades::DataModel* CharacteristicsEditor::descriptorsModel() const
{
    return m_descriptorsModel;
}

QString CharacteristicsEditor::errorMessage() const
{
    return m_errorMessage;
}

void CharacteristicsEditor::setErrorMessage(const QString &errorMessage)
{
    m_errorMessage = errorMessage;
    emit errorMessageChanged();
}
