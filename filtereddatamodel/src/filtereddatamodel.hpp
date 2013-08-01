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

#ifndef FILTEREDDATAMODEL_HPP
#define FILTEREDDATAMODEL_HPP

#include <bb/cascades/DataModel>

//! [0]
class FilteredDataModel : public bb::cascades::DataModel
{
public:
    FilteredDataModel(bb::cascades::DataModel *sourceModel, QObject *parent = 0);

    // Required interface implementation
    virtual int childCount(const QVariantList& indexPath);
    virtual bool hasChildren(const QVariantList& indexPath);
    virtual QVariant data(const QVariantList& indexPath);
    virtual QString itemType(const QVariantList& indexPath);

    void expandHeader(int headerIndex, bool selected);
    bool isHeaderExpanded(int headerIndex) const;

private:
    bool isFiltered(const QVariantList& indexPath) const;
    void setExpandedHeader(int headerIndex);

private:
    bb::cascades::DataModel* m_sourceDataModel;
    int m_expandedIndex;  // currently expanded header by index, or -1 if none expanded
};
//! [0]

#endif
