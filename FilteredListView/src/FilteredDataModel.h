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
 * FilteredDataModel.h
 */

#ifndef FILTEREDDATAMODEL_H_
#define FILTEREDDATAMODEL_H_

#include <bb/cascades/DataModel>

class FilteredDataModel : public bb::cascades::DataModel
{
public:
    FilteredDataModel();
    virtual ~FilteredDataModel();

    // Required interface implementation
    virtual int childCount(const QVariantList& indexPath);
    virtual bool hasChildren(const QVariantList& indexPath);
    virtual QVariant data(const QVariantList& indexPath);

    void expandHeader(int headerIndex, bool selected);
    bool isHeaderExpanded(int headerIndex) const {return headerIndex == m_expandedIndex;}

private:
    bool isFiltered(const QVariantList& indexPath) const;
    void setExpandedHeader(int headerIndex);

private:
    bb::cascades::DataModel* m_fullDataModel;
    int m_expandedIndex;  // currently expanded header by index, or -1 if none expanded
};

#endif /* FILTEREDDATAMODEL_H_ */
