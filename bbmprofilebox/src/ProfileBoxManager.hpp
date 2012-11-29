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

#ifndef PROFILEBOXMANAGER_HPP
#define PROFILEBOXMANAGER_HPP

#include <bb/cascades/QListDataModel>
#include <bb/platform/bbm/ProfileBox>

#include <QtCore/QObject>

namespace bb {
namespace platform {
namespace bbm {
class Context;
}
}
}

//! [0]
class ProfileBoxManager : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bb::cascades::DataModel* model READ model CONSTANT)

public:
    ProfileBoxManager(bb::platform::bbm::Context &context, QObject *parent = 0);

public Q_SLOTS:
    void show();

    void addProfileBoxItem(const QString &text, int iconId);
    void removeProfileBoxItem(const QString &itemId);

private Q_SLOTS:
    void loadProfileBoxes();
    void itemAdded(const QString &itemId);
    void itemRemoved(const QString &itemId);
    void iconRetrieved(int iconId, bb::platform::bbm::ImageType::Type type, const QByteArray &imageData);

private:
    void registerIcons();
    void registerIcon(const QString &path, int iconId);

    bb::cascades::DataModel* model() const;

    bb::platform::bbm::ProfileBox* m_profileBox;
    bb::cascades::QVariantListDataModel* m_model;
    bb::platform::bbm::Context *m_context;
};
//! [0]

#endif
