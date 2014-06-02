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
#include "applicationui.hpp"

#include <bb/cascades/Application>
#include <bb/cascades/QmlDocument>
#include <bb/cascades/AbstractPane>
#include <bb/cascades/LocaleHandler>
#include <QDebug>
#include <QTimer>

//! [0]
const QString ApplicationHeadless::m_author = "Example Inc."; // for creating settings
const QString ApplicationHeadless::m_appName = "HeadlesServiceApp"; // for creating settings

// keys for setting file
const QString ApplicationHeadless::m_serviceStatus = "ServiceStatus";
const QString ApplicationHeadless::m_flashNumber = "FlashCount";
const QString ApplicationHeadless::m_remainingCount = "RemainingFlashCount";
const QString ApplicationHeadless::m_reset = "Reset";
//! [0]
using namespace bb::cascades;

ApplicationHeadless::ApplicationHeadless()
    : QObject()
    , m_remainingFlashCount(-1)
{
    // prepare the localization
    m_pTranslator = new QTranslator(this);
    m_pLocaleHandler = new LocaleHandler(this);

    QSettings settings(m_author, m_appName);
    // Force the creation of the settings file so that we can watch it for changes.
    settings.sync();
    // Watcher for changes in the settings file.
    settingsWatcher = new QFileSystemWatcher(this);
    settingsWatcher->addPath(settings.fileName());
    connect(settingsWatcher, SIGNAL(fileChanged(const QString&)), this, SLOT(settingsChanged(const QString&)));

    // initial load
    // Create scene document from main.qml asset, the parent is set
    // to ensure the document gets destroyed properly at shut down.
    QmlDocument *qml = QmlDocument::create("asset:///main.qml").parent(this);
    // expose this class to the qml context so that we can query it for the necessary values
    // via properties, slots or invokable methods
    qml->setContextProperty("_app", this);
    // Create root object for the UI
    AbstractPane *root = qml->createRootObject<AbstractPane>();

    // Set created root object as the application scene
    Application::instance()->setScene(root);
}

//! [1]
bool ApplicationHeadless::isServiceRunning()
{
    qDebug() << "check for service running via qsettings...";
    QSettings settings(m_author, m_appName);
    if (settings.value("ServiceStatus").isNull()) {
        qDebug() << "found null value for ServiceStatus key...";
    } else {
        QString status = settings.value("ServiceStatus").toString();
        if (status == "running") {
            // update remaining flash count since service is already running
            settingsChanged("");
            return true;
        }
    }
    return false;
}

int ApplicationHeadless::flashCount()
{
    QSettings settings(m_author, m_appName);
    if (settings.contains(m_flashNumber)) {
        return settings.value(m_flashNumber).toInt();
    }
    return 0;
}
//! [1]
//! [2]
void ApplicationHeadless::resetLED()
{
    QSettings settings(m_author, m_appName);
    settings.setValue(m_reset, true);
}

void ApplicationHeadless::setRemainingFlashCount(int x)
{
    m_remainingFlashCount = x;
    qDebug() << "emitting update signal flc";
    Q_EMIT remainingFlashCountChanged();
}
//! [2]
int ApplicationHeadless::remainingFlashCount()
{
    return m_remainingFlashCount;
}
//! [3]
void ApplicationHeadless::settingsChanged(const QString & path)
{
    qDebug() << "updating flash remaining counter";
    QSettings settings(m_author, m_appName);
    setRemainingFlashCount(settings.value(m_remainingCount).toInt());
    qDebug() << "remaining count: " << settings.value(m_remainingCount).toString();
}
//! [3]
