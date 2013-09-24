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

#include "DataQualityModelDecorator.hpp"

DataQualityModelDecorator::DataQualityModelDecorator(QObject* parent)
    : bb::cascades::datamanager::DataModelDecorator(parent)
{
}

DataQualityModelDecorator::~DataQualityModelDecorator() {
}
//! [0]
QString DataQualityModelDecorator::itemType(const QVariantList& indexPath) {
    QVariant data = bb::cascades::datamanager::DataModelDecorator::data(
            indexPath);
    if (data.isValid()) {
        return QString("dataQualityDecorated");
    }
    return QString("");
}
//! [0]
