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

#ifndef DEVICELISTING_HPP
#define DEVICELISTING_HPP

#include <bb/cascades/GroupDataModel>

#include <QObject>

/**
 * @short A class that encapsulates the listing of paired and remote bluetooth devices.
 */
//! [0]
class DeviceListing : public QObject
{
    Q_OBJECT

    // Provides the discovered bluetooth devices as data model
    Q_PROPERTY(bb::cascades::DataModel* model READ model CONSTANT)

public:
    // Creates a new DeviceListing object
    DeviceListing(QObject *parent = 0);

public Q_SLOTS:
    // Updates the model property with the available devices
    void update();

    // Updates the model property with the available devices but runs a discovery first
    void discover();

private:
    // The accessor method of the property
    bb::cascades::DataModel* model() const;

    // The property value
    bb::cascades::GroupDataModel* m_model;
};
//! [0]

#endif
