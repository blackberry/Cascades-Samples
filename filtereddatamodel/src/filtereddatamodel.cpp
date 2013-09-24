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

#include "filtereddatamodel.hpp"

//! [0]
FilteredDataModel::FilteredDataModel(bb::cascades::DataModel *sourceModel, QObject *parent)
    : bb::cascades::DataModel(parent)
    , m_sourceDataModel(sourceModel)
    , m_expandedIndex(-1) // no header expanded
{
}
//! [0]

/*
 * Return true if we are filtering this indexPath.
 * Return false if we are using the underlying data as-is.
 */
//! [1]
bool FilteredDataModel::isFiltered(const QVariantList& indexPath) const
{
    return indexPath.size() == 1 &&
           indexPath[0].toInt() != m_expandedIndex;
}
//! [1]

/*
 * Return the number of children.
 * Defer to the underlying data model unless the header is filtered.
 * Note: assumes m_sourceDataModel is initialized
 */
//! [2]
int FilteredDataModel::childCount(const QVariantList& indexPath)
{
    if (isFiltered(indexPath)) {
        // Unexpanded header
        return 0;
    }

    return m_sourceDataModel->childCount(indexPath); // pointer always initialized
}
//! [2]

/*
 * Return the number of children.
 *
 * Logically this routine is equivalent to:
 *   return childCount(indexPath) != 0
 * But with some underlying data models it may be expensive to ask
 * for the child count but fast to check if there are any children at all.
 * So mimic the code of childCOunt to detect our exception,
 * and pass the request along for everything else.
 */
//! [3]
bool FilteredDataModel::hasChildren(const QVariantList& indexPath)
{
    if (isFiltered(indexPath)) {
        // Unexpanded header
        return false;
    }

    return m_sourceDataModel->hasChildren(indexPath); // pointer always initialized
}
//! [3]

/*
 * Return the data.
 * The ListView will only call this for valid data, so just
 * forward the request to the underlying data model.
 * We could add defensive code here to ensure we only return
 * underlying data for unfiltered data.
 */
//! [4]
QVariant FilteredDataModel::data(const QVariantList& indexPath)
{
    if (indexPath.size() == 1) { // header item
        // Enrich the original data of the source model with additional data about expanded state
        QVariantMap data;
        data["data"] = m_sourceDataModel->data(indexPath);
        data["expanded"] = (indexPath[0] == m_expandedIndex);

        return data;
    } else {
        // Pass through the data from the source model
        return m_sourceDataModel->data(indexPath);
    }
}
//! [4]

/*
 * Return the item type.
 * The ListView will only call this for valid data, so just
 * forward the request to the underlying data model.
 * We could add defensive code here to ensure we only return
 * underlying data for unfiltered data.
 */
//! [5]
QString FilteredDataModel::itemType(const QVariantList& indexPath)
{
    return m_sourceDataModel->itemType(indexPath); // pointer always initialized
}
//! [5]

/*
 * Expand or collapse the specified header.
 * We only support one header expanded at a time, so expanding
 * a different header will collapse the previous.
 * Requests that keep us in the current state are ignored.
 */
//! [6]
void FilteredDataModel::expandHeader(int headerIndex, bool expand)
{
    if (!expand) {
        if (headerIndex == m_expandedIndex) {
            // Collapse the header
            setExpandedHeader(-1);
        }
    } else {
        setExpandedHeader(headerIndex);
    }
}
//! [6]

//! [7]
bool FilteredDataModel::isHeaderExpanded(int headerIndex) const
{
    return (headerIndex == m_expandedIndex);
}
//! [7]

/*
 * Set the currently expanded header (or none if index == -1)
 * Inform the ListView that we made a possibly large change,
 * but only if we do make a change.
 */
//! [8]
void FilteredDataModel::setExpandedHeader(int index)
{
    if (m_expandedIndex != index) {
        // Only emit if we actually make a change
        m_expandedIndex = index;
        emit itemsChanged(bb::cascades::DataModelChangeType::AddRemove);
    }
}
//! [8]
