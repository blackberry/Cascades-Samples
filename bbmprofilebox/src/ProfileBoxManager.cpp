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

#include "ProfileBoxManager.hpp"

#include "RegistrationHandler.hpp"

#include <bb/cascades/AbstractPane>
#include <bb/cascades/Application>
#include <bb/cascades/Image>
#include <bb/cascades/QmlDocument>
#include <bb/platform/bbm/ProfileBoxItem>

#include <QtCore/QVariantList>
#include <QtCore/QVariant>

using namespace bb::cascades;

//! [0]
ProfileBoxManager::ProfileBoxManager(bb::platform::bbm::Context &context, QObject *parent)
    : QObject(parent)
    , m_profileBox(0)
    , m_context(&context)
    , m_model(new QVariantListDataModel())
{
    m_model->setParent(this);
}
//! [0]

//! [1]
void ProfileBoxManager::addProfileBoxItem(const QString &text, int iconId)
{
    m_profileBox->requestAddItem(text, iconId);
}
//! [1]

//! [2]
void ProfileBoxManager::removeProfileBoxItem(const QString &itemId)
{
    m_profileBox->requestRemoveItem(itemId);
}
//! [2]

//! [3]
void ProfileBoxManager::show()
{
    // Create the UI
    QmlDocument *qml = QmlDocument::create("asset:///main.qml").parent(this);

    qml->setContextProperty("_profileBoxManager", this);

    AbstractPane *root = qml->createRootObject<AbstractPane>();
    Application::instance()->setScene(root);

    // Delay the loading of ProfileBox a bit
    QMetaObject::invokeMethod(this, "loadProfileBoxes", Qt::QueuedConnection);
}
//! [3]

//! [4]
void ProfileBoxManager::loadProfileBoxes()
{
    // Create the profile box object
    m_profileBox = new bb::platform::bbm::ProfileBox(m_context, this);

    // Connect all signals to get informed about updates to the profile box
    bool ok = connect(m_profileBox, SIGNAL(itemAdded(QString)), this, SLOT(itemAdded(QString)));
    Q_ASSERT(ok);
    ok = connect(m_profileBox, SIGNAL(itemRemoved(QString)), this, SLOT(itemRemoved(QString)));
    Q_ASSERT(ok);
    ok = connect(m_profileBox, SIGNAL(iconRetrieved(int, bb::platform::bbm::ImageType::Type, QByteArray)),
                 this, SLOT(iconRetrieved(int, bb::platform::bbm::ImageType::Type, QByteArray)));
    Q_ASSERT(ok);

    registerIcons();

    // Fill the model with the initial data
    foreach (const bb::platform::bbm::ProfileBoxItem &item, m_profileBox->items()) {
        // Create a model entry
        const int iconId = item.iconId();

        QVariantMap entry;
        entry["id"] = item.id();
        entry["text"] = item.text();
        entry["iconId"] = iconId;

        // Append the entry to the model
        m_model->append(entry);

        // Request the icon for this icon ID asynchronously
        m_profileBox->requestRetrieveIcon(iconId);
    }
}
//! [4]

//! [5]
void ProfileBoxManager::itemAdded(const QString &itemId)
{
    // Retrieve the new item
    const bb::platform::bbm::ProfileBoxItem item = m_profileBox->item(itemId);

    // Create a model entry for it
    const int iconId = item.iconId();

    QVariantMap entry;
    entry["id"] = itemId;
    entry["text"] = item.text();
    entry["iconId"] = iconId;

    // Append the entry to the model
    m_model->append(entry);

    // Request the icon for this icon ID asynchronously
    m_profileBox->requestRetrieveIcon(iconId);
}
//! [5]

//! [6]
void ProfileBoxManager::itemRemoved(const QString &itemId)
{
    // Search the corresponding entry in the model
    for (int pos = 0; pos < m_model->size(); ++pos) {
        if (m_model->value(pos).toMap().value("id").toString() == itemId) {
            // Remove the corresponding entry from the model
            m_model->removeAt(pos);
            return;
        }
    }
}
//! [6]

//! [7]
void ProfileBoxManager::iconRetrieved(int iconId, bb::platform::bbm::ImageType::Type, const QByteArray &imageData)
{
    const bb::cascades::Image image(imageData);

    for (int pos = 0; pos < m_model->size(); ++pos) {
        QVariantMap entry = m_model->value(pos).toMap();
        if (entry.value("iconId").toInt() == iconId) {
            entry["icon"] = QVariant::fromValue(image);
            m_model->replace(pos, entry);
        }
    }
}
//! [7]

//! [8]
void ProfileBoxManager::registerIcons()
{
    const QString imageDir(QDir::currentPath() + QLatin1String("/app/native/assets/images/"));
    registerIcon(imageDir + QLatin1String("apple.png"), 1);
    registerIcon(imageDir + QLatin1String("pear.png"), 2);
    registerIcon(imageDir + QLatin1String("orange.png"), 3);
}
//! [8]

//! [9]
void ProfileBoxManager::registerIcon(const QString& path, int iconId)
{
    // Read the icon from the file
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly))
        return;

    const QByteArray imageData = file.readAll();

    // Create the icon object and register the icon
    m_profileBox->requestRegisterIcon(iconId, bb::platform::bbm::ImageType::Png, imageData);
}
//! [9]

bb::cascades::DataModel* ProfileBoxManager::model() const
{
    return m_model;
}
