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

/*
 * FilteredDataModel.cpp
 */

#include "FilteredDataModel.h"
#include "HelloWorldDataModel.h"

FilteredDataModel::FilteredDataModel()
: m_fullDataModel(new HelloWorldDataModel) // Hard-coded to HelloWorldDataModel
, m_expandedIndex(-1) // no header expanded
{
}

FilteredDataModel::~FilteredDataModel()
{
    if (m_fullDataModel != 0) // always, in initial version of the software
    {
        delete m_fullDataModel;
    }
}

/*
 * Return true if we are filtering this indexPath.
 * Return false if we are using the underlying data as-is.
 */
bool FilteredDataModel::isFiltered(const QVariantList& indexPath) const
{
    return indexPath.size() == 1 &&
           indexPath[0].toInt() != m_expandedIndex;
}

/*
 * Return the number of children.
 * Defer to the underlying data model unless the header is filtered.
 * Note: assumes m_fullDataModel is initialized
 */
int FilteredDataModel::childCount(const QVariantList& indexPath)
{
    if (isFiltered(indexPath))
    {
        // Unexpanded header
        return 0;
    }
    return m_fullDataModel->childCount(indexPath); // pointer always initialized
}

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
bool FilteredDataModel::hasChildren(const QVariantList& indexPath)
{
    if (isFiltered(indexPath))
    {
        // Unexpanded header
        return false;
    }
    return m_fullDataModel->hasChildren(indexPath); // pointer always initialized
}

/*
 * Return the data.
 * The ListView will only call this for valid data, so just
 * forward the request to the underlying data model.
 * We could add defensive code here to ensure we only return
 * underlying data for unfiltered data.
 */
QVariant FilteredDataModel::data(const QVariantList& indexPath)
{
    return m_fullDataModel->data(indexPath); // pointer always initialized
}

/*
 * Expand or collapse the specified header.
 * We only support one header expanded at a time, so expanding
 * a different header will collapse the previous.
 * Requests that keep us in the current state are ignored.
 */
void FilteredDataModel::expandHeader(int headerIndex, bool expand)
{
    if (!expand)
    {
        if (headerIndex == m_expandedIndex)
        {
            // Collapse the header
            setExpandedHeader(-1);
        }
    }
    else
    {
        setExpandedHeader(headerIndex);
    }
}

/*
 * Set the currently expanded header (or none if index == -1)
 * Inform the ListView that we made a possibly large change,
 * but only if we do make a change.
 */
void FilteredDataModel::setExpandedHeader(int index)
{
    if (m_expandedIndex != index)
    {
        // Only emit if we actually make a change
        m_expandedIndex = index;
        emit itemsChanged(bb::cascades::DataModelChangeType::AddRemove);
    }
}

