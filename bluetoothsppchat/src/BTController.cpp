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

#include "BTController.hpp"

#include <QtCore>

#include <btapi/btdevice.h>
#include <btapi/btspp.h>

#include <errno.h>
#include <iostream>

/**
 * A static variable that points to the unique instance of BTController
 * This is needed to have access to the BTController instance from within the btdevice API callback.
 */
//! [0]
static BTController *s_btController = 0;
//! [0]

/**
 * Callback for events triggered by btdevice APIs.
 */
//! [1]
void BTControllerCallback(const int event, const char *bt_addr, const char *event_data)
{
    if (s_btController) {
        s_btController->emitBTDeviceSignal(event, bt_addr, event_data);
    }
}
//! [1]

//! [2]
BTController::BTController(QObject* parent)
    : QObject(parent)
    , m_chatManager(new ChatManager(this))
    , m_deviceListing(new DeviceListing(this))
    , m_localDeviceInfo(new LocalDeviceInfo(this))
    , m_remoteDeviceInfo(new RemoteDeviceInfo(this))
{
    s_btController = this;

    connect(this, SIGNAL(BTDeviceSignal(int, QString, QString)),
            this, SLOT(handleBTDeviceEvent(int, QString, QString)));

    // Initialize the btdevice and SPP library APIs.
    bt_device_init(BTControllerCallback);
    bt_spp_init();

    // Initialize the list of paired and found devices, but do not run a Bluetooth search.
    m_deviceListing->update();

    // Initialize the local device information
    m_localDeviceInfo->update();
}
//! [2]

//! [3]
BTController::~BTController()
{
    // De-initialize the btdevice library.
    bt_device_deinit();

    // De-initialize the library.
    bt_spp_deinit();

    s_btController = 0;
}
//! [3]

//! [4]
void BTController::emitBTDeviceSignal(int event, const QString &btAddr, const QString &eventData)
{
    emit BTDeviceSignal(event, btAddr, eventData);
}
//! [4]

//! [5]
void BTController::handleBTDeviceEvent(int event, const QString &btAddr, const QString &eventData)
{
    switch (event) {
        case BT_EVT_RADIO_SHUTDOWN:
        case BT_EVT_RADIO_INIT:
        case BT_EVT_ACCESS_CHANGED:
            emit bluetoothActiveChanged();
            emit discoverableActiveChanged();
            m_localDeviceInfo->update();
            break;
        default:
            logQString("Unknown event " + QString::number(event) + "/" + btAddr);
            break;
    }
}
//! [5]

//! [6]
bool BTController::bluetoothActive() const
{
    return bt_ldev_get_power();
}

bool BTController::discoverableActive() const
{
    return (bt_ldev_get_discoverable() == BT_DISCOVERABLE_GIAC);
}
//! [6]

ChatManager* BTController::chatManager() const
{
    return m_chatManager;
}

DeviceListing* BTController::deviceListing() const
{
    return m_deviceListing;
}

LocalDeviceInfo* BTController::localDeviceInfo() const
{
    return m_localDeviceInfo;
}

RemoteDeviceInfo* BTController::remoteDeviceInfo() const
{
    return m_remoteDeviceInfo;
}

//! [7]
void BTController::toggleBluetoothActive()
{
    bt_ldev_set_power(!bt_ldev_get_power());

    emit bluetoothActiveChanged();
}

void BTController::toggleDiscoverableActive()
{
    // If discoverable mode is GIAC, then we are discoverable - change to connectable (but not discoverable), and vice-versa.
    if (bt_ldev_get_discoverable() == BT_DISCOVERABLE_GIAC) {
        bt_ldev_set_discoverable(BT_DISCOVERABLE_CONNECTABLE);
    } else {
        bt_ldev_set_discoverable(BT_DISCOVERABLE_GIAC);
    }

    emit discoverableActiveChanged();
}
//! [7]

//! [8]
void BTController::setRemoteDevice(const QString &address)
{
    logQString(address);

    // Update the remote address on the helper objects
    m_chatManager->setRemoteAddress(address);
    m_remoteDeviceInfo->update(address);
}
//! [8]

void BTController::logQString(const QString &msg)
{
    std::cout << "Bluetooth Diagnostics: " << msg.toLocal8Bit().constData() << std::endl;
}
