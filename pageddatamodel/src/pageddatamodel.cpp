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

#include "pageddatamodel.hpp"

//! [0]
PagedDataModel::PagedDataModel(QObject *parent)
{
    // Workaround since QVariantListDataModel is missing the parent parameter in constructor
    setParent(parent);

    // Fill the model initially with values '0' to '19'
    for (int row = 0; row < 20; ++row) {
        append(QString::number(row));
    }

    // Add the marker item to request more items
    append(tr("Click for more..."));
}
//! [0]

//! [1]
void PagedDataModel::addItemsAt(const QVariantList &indexPath)
{
    // Check whether the index path is valid
    if (indexPath.size() != 1)
        return;

    // Retrieve position of marker in list
    const int position = indexPath[0].toInt();

    // Remove the marker
    removeAt(position);

    // Append the new items
    for (int row = position; row < (position + 10); ++row) {
        append(QString::number(row));
    }

    // Add a new marker item to request more items
    append(tr("Click for more..."));

    emit itemCountChanged();
}
//! [1]

//! [2]
int PagedDataModel::itemCount() const
{
    return (size() - 1);
}
//! [2]
