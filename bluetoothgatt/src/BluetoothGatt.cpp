/* Copyright (c) 2013 Research In Motion Limited.
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

#include "BluetoothGatt.hpp"

#include "CharacteristicsEditor.hpp"
#include "TypedArrayDataModel.hpp"
#include "Util.hpp"

#include <QVariant>
#include <QMap>

#include <errno.h>
#include <unistd.h>
#include <arpa/inet.h>

using namespace bb::cascades;

#define BT_CONTROLLER_EVENT         1
#define BT_GATT_CONNECT_EVENT       2
#define BT_GATT_DISCONNECT_EVENT    3
#define BT_GATT_NOTIFICATION_EVENT  4

typedef struct {
    int event;
    char *bt_addr;
    char *event_data;
} btController_t;

typedef struct {
    char *bdaddr;
    char *service;
    int instance;
    int err;
    uint16_t connInt;
    uint16_t latency;
    uint16_t superTimeout;
} btGatt_t;

typedef struct {
    int instance;
    uint16_t handle;
    uint8_t *val;
    uint16_t len;
} btNotification_t;

//! [0]
BluetoothGatt *s_globalBluetoothGatt = 0;
//! [0]

//! [1]
bool processBluetoothEvent(const int event, void *data)
{
    if (s_globalBluetoothGatt) {
        s_globalBluetoothGatt->processBluetoothEvent(event, data);
        return true;
    }
    return false;
}
//! [1]

/**
 * Callback for events triggered by btdevice and btgatt APIs.
 */
//! [2]
void bt_controller_cb(const int event, const char *bt_addr, const char *event_data)
{
    btController_t *ctrl = (btController_t *)calloc(1, sizeof(btController_t));

    if (!ctrl)
        return;

    ctrl->event = event;
    ctrl->bt_addr = strdup(bt_addr);
    ctrl->event_data = strdup(event_data);

    if ((0 == ctrl->bt_addr) || (!processBluetoothEvent(BT_CONTROLLER_EVENT, (void*)ctrl))) {
        free(ctrl->bt_addr);
        free(ctrl->event_data);
        free(ctrl);
    }
}
//! [2]

//! [3]
void gatt_service_connected_cb(const char *bdaddr, const char *service, int instance, int err, uint16_t connInt, uint16_t latency, uint16_t superTimeout, void *userData)
{
    if ((0 == bdaddr) || (0 == service) || (0 == userData))
        return;

    btGatt_t *gatt = (btGatt_t *)calloc(1, sizeof(btGatt_t));

    if (!gatt)
        return;

    gatt->bdaddr = strdup(bdaddr);
    gatt->service = strdup(service);
    gatt->instance = instance;
    gatt->err = err;
    gatt->connInt = connInt;
    gatt->latency = latency;
    gatt->superTimeout = superTimeout;

    if ((0 == gatt->bdaddr) || (0 == gatt->service) || (!processBluetoothEvent(BT_GATT_CONNECT_EVENT, (void*) gatt))) {
        free(gatt->bdaddr);
        free(gatt->service);
        free(gatt);
    }
}
//! [3]

/**
 * IMPORTANT:  This callback is NOT triggered when your app requests a service disconnect,
 *      but rather only when a service is disconnected WITHOUT you requesting it.
 *
 *      This behavior differs from the gatt_service_connected_cb() since bt_gatt_connect_service()
 *      is designed to be asynchronous, while bt_gatt_disconnect_service() is not.
 */
//! [4]
void gatt_service_disconnected_cb(const char *bdaddr, const char *service, int instance, int reason, void *userData)
{
    if ((0 == bdaddr) || (0 == service) || (0 == userData))
        return;

    btGatt_t *gatt = (btGatt_t *) calloc(1, sizeof(btGatt_t));

    if (!gatt)
        return;

    gatt->bdaddr = strdup(bdaddr);
    gatt->service = strdup(service);
    gatt->instance = instance;
    gatt->err = reason;

    if ((0 == gatt->bdaddr) || (0 == gatt->service) || (!processBluetoothEvent(BT_GATT_DISCONNECT_EVENT, (void*) gatt))) {
        free(gatt->bdaddr);
        free(gatt->service);
        free(gatt);
    }
}
//! [4]

//! [5]
static void notifications_cb(int instance, uint16_t handle, const uint8_t *val, uint16_t len, void*)
{
    qDebug() << "Notification callback";
    if ( 0 == val )
        return;

    btNotification_t *notify = (btNotification_t *)calloc(1, sizeof(btNotification_t));

    if (0 == notify)
        return;

    notify->instance = instance;
    notify->handle = handle;
    notify->val = (uint8_t*)calloc(len, sizeof(uint8_t));
    if (0 == notify->val) {
        free(notify);
        return;
    }
    memcpy(notify->val, val, len);
    notify->len = len;
    if ((0 == notify->val) || (!processBluetoothEvent(BT_GATT_NOTIFICATION_EVENT, (void*) notify))) {
        free(notify->val);
        free(notify);
    }
}
//! [5]


//! [6]
BluetoothGatt::BluetoothGatt(QObject *parent)
    : QObject(parent)
    , m_devices(new TypedArrayDataModel())
    , m_services(new TypedArrayDataModel())
    , m_characteristics(new TypedArrayDataModel())
    , m_characteristicList(0)
    , m_characteristicStruct(0)
    , m_editor(new CharacteristicsEditor(this))
{
    s_globalBluetoothGatt = this;

    qmlRegisterType<CharacteristicsEditor>();

    m_devices->setParent(this);
    m_services->setParent(this);
    m_characteristics->setParent(this);

    // Initialize the btdevice and SPP library APIs.
    if (bt_device_init(bt_controller_cb) != EOK) {
        qWarning("Unable to initialize bluetooth device");
    }

    m_gatt_cb.connected = gatt_service_connected_cb;
    m_gatt_cb.disconnected = gatt_service_disconnected_cb;

    if (bt_gatt_init(&m_gatt_cb) != EOK) {
        /* TODO: Add error to UI and abort app */
        qWarning() << "GATT initialization failure" << errno;
    }

    if (bt_le_init(0) != EOK) {
        /* TODO: Add error to UI and abort app */
        qWarning() << "LE initialization failure " << errno;
    }

    connect(this, SIGNAL(bluetoothEvent(const int, void*) ), this, SLOT(handleBluetoothEvent(const int, void *)));
    connect(m_editor, SIGNAL(characteristicNotificationsEnabledChanged(bool)), this, SLOT(handleCharacteristicNotificationsEnabledChanged(bool)));

    refreshDevices();
}
//! [6]

//! [7]
BluetoothGatt::~BluetoothGatt()
{
    // De-initialize the btdevice library.
    bt_device_deinit();

    bt_gatt_deinit();
    bt_le_deinit();

    s_globalBluetoothGatt = 0;
}
//! [7]

/**
 * processBluetoothEvent - generates a bluetoothEvent signal to indicate a btdevice event has occurred.
 */
void BluetoothGatt::processBluetoothEvent(const int event, void *data)
{
    emit bluetoothEvent(event, data);
}

//! [8]
void BluetoothGatt::handleBluetoothEvent(const int event, void *data)
{
    switch (event) {
    case BT_CONTROLLER_EVENT:
        if (data) {
            btController_t *ctrl = (btController_t*) data;
            switch (ctrl->event) {
            case BT_EVT_RADIO_SHUTDOWN:
            case BT_EVT_RADIO_INIT:
                break;
            case BT_EVT_ACCESS_CHANGED:
                break;
            default:
                qWarning() << "Unknown event" << ctrl->event << "/" << ctrl->bt_addr;
                break;
            }

            free(ctrl->bt_addr);
            free(ctrl->event_data);
            free(ctrl);
        }
        break;
    case BT_GATT_CONNECT_EVENT:
        if (data) {
            btGatt_t *gatt = (btGatt_t*)data;
            gattServiceConnected(gatt->bdaddr, gatt->service, gatt->instance, gatt->err, gatt->connInt, gatt->latency, gatt->superTimeout);
            free(gatt->bdaddr);
            free(gatt->service);
            free(gatt);
        }
        break;
    case BT_GATT_DISCONNECT_EVENT:
        if (data) {
            btGatt_t *gatt = (btGatt_t*)data;
            gattServiceDisconnected(gatt->bdaddr, gatt->service, gatt->instance, gatt->err);
            free(gatt->bdaddr);
            free(gatt->service);
            free(gatt);
        }
        break;
    case BT_GATT_NOTIFICATION_EVENT:
        if (data) {
            btNotification_t *notify = (btNotification_t*)data;
            gattNotification(notify->instance, notify->handle, notify->val, notify->len);
            free(notify->val);
            free(notify);
        }
        break;
    default:
        break;
    }
}
//! [8]

//! [9]
void BluetoothGatt::handleCharacteristicNotificationsEnabledChanged(bool enabled)
{
    bt_gatt_enable_notify(m_gattInstance, m_characteristicStruct, enabled);
}
//! [9]

//! [10]
void BluetoothGatt::disconnectServices()
{
    for (int i = 0; i < m_services->size(); i++) {
        const QVariantMap service = m_services->value(i).toMap();
        const int rc = bt_gatt_disconnect_service(activeDevice().toAscii().constData(), service["uuid"].toString().toAscii().constData());
        gattServiceDisconnected(activeDevice(), service["uuid"].toString(), 0, rc);
    }
}
//! [10]

//! [11]
void BluetoothGatt::resetEditor()
{
    m_characteristicStruct = 0;
}
//! [11]

//! [12]
void BluetoothGatt::resetCharacteristicsList()
{
    if (m_characteristicList) {
        free(m_characteristicList);
        m_characteristicList = 0;
    }
}
//! [12]

//! [13]
void BluetoothGatt::refreshDevices()
{
    m_devices->clear();

    bt_remote_device_t **remote_device_array;
    char tempbuff[128];
    bt_remote_device_t *next_remote_device = 0;
    int device_type = 0;

    // Retrieve and show all paired devices.
    remote_device_array = bt_disc_retrieve_devices(BT_DISCOVERY_PREKNOWN, 0);
    if (remote_device_array) {
        for (int i = 0; 0 != (next_remote_device = remote_device_array[i]); ++i) {
            device_type = bt_rdev_get_type(next_remote_device);
            if (device_type == BT_DEVICE_TYPE_LE_PUBLIC || device_type == BT_DEVICE_TYPE_LE_PRIVATE) {
                QVariantMap map;
                map["type"] = "item";
                bt_rdev_get_friendly_name(next_remote_device, tempbuff, 128);
                map["name"] = tempbuff;
                bt_rdev_get_addr(next_remote_device, tempbuff);
                map["address"] = tempbuff;
                map["paired"] = true;
                m_devices->append(map);
            }
        }
        bt_rdev_free_array(remote_device_array);
    }
}
//! [13]

//! [14]
void BluetoothGatt::viewServices(int which)
{
    if (which < 0 || which >= m_devices->size())
        return;

    const QVariantMap device = m_devices->value(which).toMap();
    if (device.isEmpty())
        return;

    setActiveDevice(device["address"].toString());
    setActiveDeviceName(device["name"].toString());

    m_services->clear();

    bt_remote_device_t *remote_device = bt_rdev_get_device(device["address"].toString().toAscii());

    if (!remote_device)
        return;

    int device_type = 0;
    char **services_array = 0;

    /**
     * NOTE:  For low energy devices, you should NOT perform a bt_rdev_refresh_services() here,
     *      as the service advertisements may have dropped since pairing.
     */

    //  Display all known basic device information.
    device_type = bt_rdev_get_type(remote_device);

    //  Display any found Bluetooth low energy services and connect to each.
    if (device_type == BT_DEVICE_TYPE_LE_PUBLIC || device_type == BT_DEVICE_TYPE_LE_PRIVATE) {
        if ((services_array = bt_rdev_get_services_gatt(remote_device))) {
            int i;
            bt_gatt_conn_parm_t conParm;
            for (i = 0; 0 != services_array[i]; i++) {
                QVariantMap map;
                if (strncasecmp(services_array[i], "0x", 2) == 0 ){
                    map["uuid"] = &(services_array[i][2]);
                } else {
                    map["uuid"] = services_array[i];
                }
                map["name"] = Util::parse_service_uuid( services_array[i] );
                map["connected"] = false;
                m_services->append(map);

                conParm.minConn = 0x30;
                conParm.maxConn = 0x50;
                conParm.latency = 0;
                conParm.superTimeout = 50;
                qDebug() << "Connecting service" << QString(services_array[i]);
                if (bt_gatt_connect_service(activeDevice().toAscii().constData(), services_array[i], 0, &conParm, this) < 0) {
                    setErrorMessage(QString("GATT connect service request failed: %1 (%2)").arg(errno).arg(strerror(errno)));
                }
            }

            bt_rdev_free_services(services_array);
        }
    }

    bt_rdev_free(remote_device);
}
//! [14]

//! [15]
void BluetoothGatt::viewCharacteristics(int row)
{
    if (row < 0 || row >= m_services->size())
        return;

    const QVariantMap service = m_services->value(row).toMap();

    if (!service["connected"].toBool()) {
        setErrorMessage(QString("%1: service not connected: %2").arg(activeDeviceName()).arg(service["uuid"].toString()));
        return;
    }

    setActiveService(service["uuid"].toString());

    m_gattInstance = service["instance"].toInt();

    if (m_characteristicList) {
        free(m_characteristicList);
        m_characteristicList = 0;
    }
    m_characteristicListSize = bt_gatt_characteristics_count(m_gattInstance);
    if (m_characteristicListSize == -1) {
        qWarning() << "GATT characteristics count failed:" << errno << "(" << strerror(errno) << ")";
        bt_gatt_disconnect_instance(m_gattInstance);
        return;
    }

    if (m_characteristicListSize == 0) {
        setErrorMessage("GATT Characteristic count returned 0");
        bt_gatt_disconnect_instance(m_gattInstance);
        return;
    }

    m_characteristicList = (bt_gatt_characteristic_t*)malloc(m_characteristicListSize * sizeof(bt_gatt_characteristic_t));
    if (!m_characteristicList) {
        setErrorMessage("GATT characteristics: Not enough memory");
        bt_gatt_disconnect_instance(m_gattInstance);
        return;
    }

    /* BEGIN WORKAROUND - Temporary fix to address race condition */
    int number = 0;
    do {
        number = bt_gatt_characteristics(m_gattInstance, m_characteristicList, m_characteristicListSize);

    } while ((number == -1) && (errno== EBUSY));

    m_characteristicListSize = number;
    /* END WORKAROUND */

    if (m_characteristicListSize == -1) {
        setErrorMessage(QString("GATT characteristics failed: %1 (%2)").arg(errno).arg(strerror(errno)));
        bt_gatt_disconnect_instance(m_gattInstance);
        return;
    }

    qDebug() << "GATT characteristics: Retreived" << m_characteristicListSize << "successfully";

    m_characteristics->clear();

    /**
     *  Get the characteristics of the GATT service here and add them to the model.
     */
    for (int i = 0; i < m_characteristicListSize; i++) {
        QVariantMap map;
        map["name"] = Util::parse_characteristic_uuid(m_characteristicList[i].uuid);
        map["uuid"] = m_characteristicList[i].uuid;
        qDebug() << "Properties" << m_characteristicList[i].properties;

        m_characteristics->append(map);
    }

    bt_gatt_reg_notifications(m_gattInstance, notifications_cb);
}
//! [15]

//! [16]
void BluetoothGatt::gattServiceConnected(const QString&, const QString &_serviceUuid, int instance, int err, uint16_t, uint16_t, uint16_t)
{
    if (err != EOK) {
        setErrorMessage(QString("GATT service connection failed %1 (%2)").arg(err).arg(strerror(err)));
        return;
    }

    QString serviceUuid(_serviceUuid);
    if (serviceUuid.startsWith("0x"))
        serviceUuid = serviceUuid.mid(2);

    for (int i = 0; i < m_services->size(); i++) {
        QVariantMap service = m_services->value(i).toMap();
        if (serviceUuid == service["uuid"].toString()) {
            service["connected"] = true;
            service["instance"] = instance;
            m_services->replace(i, service);
            qDebug() << "GATT service connected:" << serviceUuid;
            break;
        }
    }
}
//! [16]

//! [17]
void BluetoothGatt::gattServiceDisconnected(const QString&, const QString &_serviceUuid, int, int err )
{
    if (err != EOK) {
        setErrorMessage(QString("GATT service disconnection failed %1 (%2)").arg(err).arg(strerror(err)));
        return;
    }

    QString serviceUuid(_serviceUuid);
    if (serviceUuid.startsWith("0x"))
        serviceUuid = serviceUuid.mid(2);

    for (int i = 0; i < m_services->size(); i++) {
        QVariantMap service = m_services->value(i).toMap();
        if (serviceUuid == service["uuid"].toString()) {
            service["connected"] = false;
            m_services->replace(i, service);
            qDebug() << "GATT service disconnected:" << serviceUuid;
            break;
        }
    }
}
//! [17]

//! [18]
void BluetoothGatt::gattNotification(int instance, uint16_t handle, const uint8_t *val, uint16_t len)
{
    if (instance == m_gattInstance && m_characteristicStruct && handle == m_characteristicStruct->value_handle) {
        qDebug("Bluetooth Gatt Sample:\tCharacteristic notification:  val @%lx, len %d.\n", (long)val, len);

        m_editor->updateCharacteristicValue(val, len);
    } else {
        qDebug("Bluetooth Gatt Sample:\tNon-characteristic notification:  instance %d, handle %d, val @%lx, len %d.\n", instance, handle, (long)val, len);
    }
}
//! [18]

//! [19]
void BluetoothGatt::viewCharacteristicsEditor(int row)
{
    if (row < 0 || row >= m_characteristicListSize)
        return;

    m_characteristicStruct = &(m_characteristicList[row]);

    // update characteristic editor
    m_editor->setGattInstance(m_gattInstance);
    m_editor->setCharacteristic(m_characteristicStruct);
}
//! [19]

QString BluetoothGatt::activeDevice() const
{
    return m_activeDevice;
}

void BluetoothGatt::setActiveDevice(const QString &device)
{
    if (m_activeDevice != device) {
        m_activeDevice = device;
        emit activeDeviceChanged();
    }
}

QString BluetoothGatt::activeDeviceName() const
{
    return m_activeDeviceName;
}

void BluetoothGatt::setActiveDeviceName(const QString &deviceName)
{
    if (m_activeDeviceName != deviceName) {
        m_activeDeviceName = deviceName;
    }
}

QString BluetoothGatt::activeService() const
{
    return m_activeService;
}

void BluetoothGatt::setActiveService(const QString &service)
{
    if (m_activeService != service) {
        m_activeService = service;
        emit activeServiceChanged();
    }
}

QString BluetoothGatt::activeCharacteristic() const
{
    return m_activeCharacteristic;
}

void BluetoothGatt::setActiveCharacteristic(const QString &characteristic)
{
    if (m_activeCharacteristic != characteristic) {
        m_activeCharacteristic = characteristic;
        emit activeCharacteristicChanged();
    }
}

bb::cascades::DataModel* BluetoothGatt::devicesModel() const
{
    return m_devices;
}

bb::cascades::DataModel* BluetoothGatt::servicesModel() const
{
    return m_services;
}

bb::cascades::DataModel* BluetoothGatt::characteristicsModel() const
{
    return m_characteristics;
}

CharacteristicsEditor* BluetoothGatt::editor() const
{
    return m_editor;
}

QString BluetoothGatt::errorMessage() const
{
    return m_errorMessage;
}

//! [20]
void BluetoothGatt::setErrorMessage(const QString &errorMessage)
{
    m_errorMessage = errorMessage;
    emit errorMessageChanged();
}
//! [20]
