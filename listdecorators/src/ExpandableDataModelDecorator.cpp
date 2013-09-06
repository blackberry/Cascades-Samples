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

#include "ExpandableDataModelDecorator.hpp"
#include <bb/cascades/DataModelChangeType>

ExpandableDataModelDecorator::ExpandableDataModelDecorator(QObject* parent)
    : bb::cascades::datamanager::DataModelDecorator(parent)
    , m_expandedIndex(-1)
{
}

ExpandableDataModelDecorator::~ExpandableDataModelDecorator() {
}

//! [0]
QVariant ExpandableDataModelDecorator::data(const QVariantList& indexPath) {
    QVariant data = bb::cascades::datamanager::DataModelDecorator::data(
            indexPath);
    if (data.isValid()) {
        QVariantMap map = data.value<QVariantMap>();
        if (indexPath.size() == 1) {
            map["expanded"] = (indexPath[0] == m_expandedIndex);
        }
        return map;
    }
    return data;
}
//! [0]
//! [1]
bool ExpandableDataModelDecorator::isExpandable(
        const QVariantList& indexPath) const {
    return indexPath.size() == 1 && indexPath[0].toInt() != m_expandedIndex;
}

int ExpandableDataModelDecorator::childCount(const QVariantList& indexPath) {
    if (isExpandable(indexPath)) {
        return 0;
    }
    return bb::cascades::datamanager::DataModelDecorator::childCount(indexPath);
}

bool ExpandableDataModelDecorator::hasChildren(const QVariantList& indexPath) {
    if (isExpandable(indexPath)) {
        return false;
    }
    return bb::cascades::datamanager::DataModelDecorator::hasChildren(indexPath);
}
//! [1]
//! [2]
void ExpandableDataModelDecorator::expandHeader(const QVariantList& indexPath,
        bool expand) {
    if (indexPath.size() == 1 && expand) {
        int index = indexPath[0].toInt();
        if (index == m_expandedIndex) {
            m_expandedIndex = -1;
            emit itemsChanged(bb::cascades::DataModelChangeType::AddRemove);
        } else {
            m_expandedIndex = index;
            emit itemsChanged(bb::cascades::DataModelChangeType::AddRemove);
        }
    }
}
//! [2]
