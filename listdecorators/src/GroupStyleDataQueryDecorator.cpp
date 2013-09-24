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

#include "GroupStyleDataQueryDecorator.hpp"

GroupStyleDataQueryDecorator::GroupStyleDataQueryDecorator(QObject* parent)
    : bb::cascades::datamanager::DataQueryDecorator(parent)
{
}

GroupStyleDataQueryDecorator::~GroupStyleDataQueryDecorator() {
}
//! [0]
bool GroupStyleDataQueryDecorator::isGroupable(
        bb::cascades::datamanager::HeaderDataItem &data, const QString &title) {
    QVariant payload = data.payload();
    QString header = payload.toMap()["header"].toString();
    // return result of header name equality test
    return header == title;
}
//! [0]
//! [1]
void GroupStyleDataQueryDecorator::processHeaderResults(
        QList<bb::cascades::datamanager::HeaderDataItem>* headerResults) {

    // The new header results list which will contain the grouped headers.
    QList<bb::cascades::datamanager::HeaderDataItem> *groupedResults =
            new QList<bb::cascades::datamanager::HeaderDataItem>;
    int count = headerResults->size();
    for (int i = 0; i < count; ++i) {
        //bb::cascades::datamanager::HeaderDataItem groupItem =
        //        ((*headerResults)[i]);
        QString title = ((*headerResults)[i]).payload().toMap()["header"].toString();
        int groupCount = 0;
        // Sum up child counts as long as headers are equal in name and within the list size.
        do {
            groupCount += (*headerResults)[i].childCount();
        } while ((i + 1) < count && isGroupable((*headerResults)[i + 1], title)
                && ++i);
        // Set the new child count of the grouped items and add it to the new list.
        (*headerResults)[i].setChildCount(groupCount);
        groupedResults->append((*headerResults)[i]);
    }
    headerResults = groupedResults;
}
//! [1]
