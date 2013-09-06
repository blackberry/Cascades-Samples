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

#ifndef GROUPSTYLEDATAQUERYDECORATOR_HPP_
#define GROUPSTYLEDATAQUERYDECORATOR_HPP_

#include <bb/cascades/datamanager/DataItem>
#include <bb/cascades/datamanager/DataRevision>
#include <bb/cascades/datamanager/DataQueryDecorator>
#include <bb/cascades/datamanager/HeaderDataItem>

/**
 * This class performs grouping of header items, which displays all those
 * items with similarly named headers to appear under one header.
 */
//! [0]
class GroupStyleDataQueryDecorator: public bb::cascades::datamanager::DataQueryDecorator {
    Q_OBJECT

public:
    GroupStyleDataQueryDecorator(QObject* parent = 0);
    virtual ~GroupStyleDataQueryDecorator();

    /**
     * Helper method to test if the given headers can be grouped.
     * Returns true if names are equal, and false otherwise.
     */
    bool isGroupable(bb::cascades::datamanager::HeaderDataItem &data,
            const QString &title);

    /**
     * This method compares the header names and resets the header child count
     * as a sum of similarly named header child counts.
     * Overwritten function to perform special processing on the header results.
     *
     * @see DataQueryDecorator
     */
    virtual void processHeaderResults(
            QList<bb::cascades::datamanager::HeaderDataItem>* headerResults);

};
//! [0]
#endif /* GROUPSTYLEDATAQUERYDECORATOR_HPP_ */
