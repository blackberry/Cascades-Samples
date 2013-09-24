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

#ifndef DATAQUALITYDATAQUERYDECORATOR_HPP_
#define DATAQUALITYDATAQUERYDECORATOR_HPP_

#include <bb/cascades/datamanager/DataItem>
#include <bb/cascades/datamanager/DataQueryDecorator>

/**
 * This class instruments the data with image locations based on the
 * data's quality field value, before they are returned from the query functions.
 */
//! [0]
class DataQualityDataQueryDecorator: public bb::cascades::datamanager::DataQueryDecorator {
    Q_OBJECT

public:
    DataQualityDataQueryDecorator(QObject* parent = 0);
    virtual ~DataQualityDataQueryDecorator();

    /**
     * This method injects the proper image path, into the data, based
     * on the data quality field value.
     * Overwritten function to perform special processing on the results.
     *
     * @see DataQueryDecorator
     */
    void processResults(QList<bb::cascades::datamanager::DataItem>* results);
};
//! [0]
#endif /* DATAQUALITYDATAQUERYDECORATOR_HPP_ */
