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
#include "applicationheadless.hpp"

#include <bb/Application>
#include <bb/device/Led>
#include <bb/device/LedColor>
#include <bb/system/InvokeManager>
#include <bb/system/InvokeRequest>
#include <QDebug>

//! [0]
const QString ApplicationHeadless::m_author = "Example Inc."; // for creating settings
const QString ApplicationHeadless::m_appName = "HeadlesServiceApp"; // for creating settings

// keys for setting file
const QString ApplicationHeadless::m_serviceStatus = "ServiceStatus";
const QString ApplicationHeadless::m_flashNumber = "FlashCount";
const QString ApplicationHeadless::m_remainingCount = "RemainingFlashCount";
const QString ApplicationHeadless::m_ledActive = "ActiveLed";
const QString ApplicationHeadless::m_reset = "Reset";
//! [0]
using namespace bb::device;
using namespace bb::system;

//! [1]
ApplicationHeadless::ApplicationHeadless()
    : QObject()
    , m_invokeManager(new InvokeManager(this))
    , m_led(0)
    , m_flashCount(20)
    , m_settingsWatcher(new QFileSystemWatcher(this))
{
    QMetaObject::invokeMethod(this, "init", Qt::QueuedConnection);
    // log the service PID
    qDebug() << "PID------------" << QString::number(QCoreApplication::applicationPid());
}
//! [1]
//! [3]
void ApplicationHeadless::onInvoked(const bb::system::InvokeRequest& request)
{
    qDebug() << "##service got invoked: " << request.action();

    // start led flashing once the start request is received
    if (request.action().compare("bb.action.system.STARTED") == 0) {
        m_led->flash(m_flashCount);
    } else {
        // write service running status to qsettings
        QSettings settings(m_author, m_appName);
        settings.setValue(m_serviceStatus, request.action());
    }
}
//! [3]
//! [4]
void ApplicationHeadless::flashCountChanged(int x)
{
    qDebug() << "---------" + QString::number(x);
    QSettings settings(m_author, m_appName);
    settings.setValue(m_remainingCount, x);
}

void ApplicationHeadless::activeUpdate(bool active)
{
    qDebug() << "---active: " << active;
    QSettings settings(m_author, m_appName);
    settings.value(m_ledActive, active);
    if(!active) {
        QSettings settings(m_author, m_appName);
        settings.setValue(m_remainingCount, m_led->remainingFlashCount());
    }
}
//! [4]
//! [5]
void ApplicationHeadless::settingsChanged(const QString & path)
{
    QSettings settings(m_author, m_appName);
    if (settings.value(m_reset).toBool()) {
        settings.setValue(m_reset, false);
        settings.setValue(m_flashNumber, m_flashCount);
        settings.setValue(m_remainingCount, m_flashCount);
        settings.value(m_ledActive, false);
        m_led->cancel();
        disconnect(m_led, SIGNAL(remainingFlashCountChanged(int)), this, SLOT(flashCountChanged(int)));
        disconnect(m_led, SIGNAL(activeChanged(bool)), this, SLOT(activeUpdate(bool)));
        delete m_led;
        m_led = new Led(LedColor::Blue, this);
        bool ok = connect(m_led, SIGNAL(remainingFlashCountChanged(int)), this, SLOT(flashCountChanged(int)));
        Q_ASSERT(ok);
        ok = connect(m_led, SIGNAL(activeChanged(bool)), this, SLOT(activeUpdate(bool)));
        Q_ASSERT(ok);
        Q_UNUSED(ok);
        m_led->flash(m_flashCount);
    }
}
//! [5]
//! [2]
void ApplicationHeadless::init()
{
    m_led = new Led(LedColor::Blue, this);

    // set the initial qsettings keys/values upon startup
    QSettings settings(m_author, m_appName);

    settings.setValue(m_serviceStatus, "running");
    settings.setValue(m_flashNumber, m_flashCount);
    settings.setValue(m_remainingCount, m_flashCount);
    // Force the creation of the settings file so that we can watch it for changes.
    settings.sync();

    // Watcher for changes in the settings file.
    m_settingsWatcher->addPath(settings.fileName());

    // Do all the necessary signal/slot connections to be invokable to receive led updates.
    bool ok = connect(m_invokeManager, SIGNAL(invoked(const bb::system::InvokeRequest&)), this, SLOT(onInvoked(const bb::system::InvokeRequest&)));
    Q_ASSERT (ok);

    ok = connect(m_led, SIGNAL(remainingFlashCountChanged(int)), this, SLOT(flashCountChanged(int)));
    Q_ASSERT(ok);

    ok = connect(m_led, SIGNAL(activeChanged(bool)), this, SLOT(activeUpdate(bool)));
    Q_ASSERT(ok);

    ok = connect(m_settingsWatcher, SIGNAL(fileChanged(const QString&)), this, SLOT(settingsChanged(const QString&)));
    Q_ASSERT(ok);
    Q_UNUSED(ok);
}
//! [2]
