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

#include "DataQualityDataQueryDecorator.hpp"

DataQualityDataQueryDecorator::DataQualityDataQueryDecorator(QObject* parent) :
        bb::cascades::datamanager::DataQueryDecorator(parent) {
}

DataQualityDataQueryDecorator::~DataQualityDataQueryDecorator() {
}
//! [0]
void DataQualityDataQueryDecorator::processResults(
        QList<bb::cascades::datamanager::DataItem>* results) {

    QList<bb::cascades::datamanager::DataItem> *newResults = new QList<
            bb::cascades::datamanager::DataItem>;
    int count = results->size();
    for (int i = 0; i < count; i++) {
        QVariantMap map = (*results)[i].payload().toMap();
        QString dataQuality = map["data_quality"].toString();
        // Check the quality field value, and inject the proper image file based on the value found.
        if (dataQuality == "Correct" || dataQuality == "Complete and Correct") {
            map["dataQualityImage"] = "data_correct.png";
        } else {
            map["dataQualityImage"] = "data_incorrect.png";
        }
        // Save the newly processed data.
        (*results)[i].setPayload(map);
        newResults->append((*results)[i]);
    }
    results = newResults;
}
//! [0]
