/*
 * Copyright (c) 2013 BlackBerry Limited.
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

#include "SelectionDataModelDecorator.hpp"

//! [0]
SelectionDataModelDecorator::SelectionDataModelDecorator(QObject *parent)
    : m_highlight("#36D986") // default greenish hue
{
}
//! [0]
SelectionDataModelDecorator::~SelectionDataModelDecorator() {
}

//! [1]
QVariant SelectionDataModelDecorator::data(const QVariantList& indexPath) {
    QVariant data = bb::cascades::datamanager::DataModelDecorator::data(indexPath);
    QVariantMap map = data.value<QVariantMap>();
    if (indexPath.size() == 1 && data.isValid()) {
        // Inject the highlight color into the items data if item is selected.
        map["selected"] =
                m_selectedSet.contains(map["id"].toInt()) ?
                        m_highlight : QString("#000000"); //defaults to black
    }
    return map;
}
//! [1]
//! [2]
void SelectionDataModelDecorator::clearSelections() {
    m_selectedSet.clear();
    // emit signal to refresh all item data after clearing selection set.
    emit itemsChanged(bb::cascades::DataModelChangeType::Update);
}
//! [2]

//! [3]
QString SelectionDataModelDecorator::itemType(const QVariantList& indexPath) {
    QVariant data = bb::cascades::datamanager::DataModelDecorator::data(indexPath);
    if (data.isValid()) {
        // Identify these items as ListItemComponents of type "selectable"
        return QString("selectable");
    }
    return QString("");
}
//! [3]
//! [4]
void SelectionDataModelDecorator::changeSelection(QVariantList indexPath,
        bool selected) {
    if (selected) {
        QVariant data = bb::cascades::datamanager::DataModelDecorator::data(indexPath);
        if (data.isValid()) {
            QVariantMap map = data.value<QVariantMap>();
            int id = map["id"].toInt();
            // Check whether item already selected, if not add to selection se
            // otherwise remove(unselect) from the set.
            if (!m_selectedSet.contains(id)) {
                m_selectedSet.insert(id);
            } else {
                m_selectedSet.remove(id);
            }
            // emit signal to refresh item data
            emit itemUpdated(indexPath);
        }
    }
}
//! [4]
