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

#ifndef DATAQUALITYMODELDECORATOR_HPP_
#define DATAQUALITYMODELDECORATOR_HPP_

#include <bb/cascades/datamanager/DataModelDecorator>

/**
 * This class changes the query items received to be of type "dataQualityDecorated",
 * which triggers the use of custom ListItemComponent to be used for
 * displaying this type of information.
 */
//! [0]
class DataQualityModelDecorator: public bb::cascades::datamanager::DataModelDecorator {
    Q_OBJECT

public:
    DataQualityModelDecorator(QObject* parent = 0);
    virtual ~DataQualityModelDecorator();

    /**
     * Overwritten function to perform special type processing.
     *
     * @see DataModelDecorator
     */
    virtual QString itemType(const QVariantList &indexPath);
};
//! [0]
#endif /* DATAQUALITYMODELDECORATOR_HPP_ */
