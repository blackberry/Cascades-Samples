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

#ifndef APP_HPP
#define APP_HPP

#include <QtCore/QObject>
#include <QtCore/QVariant>

class FilteredDataModel;

//! [0]
class App : public QObject
{
    Q_OBJECT

public:
    App(QObject *parent = 0);

public Q_SLOTS:
    void selectionChanged(const QVariantList &indexPath, bool selected);

private:
    FilteredDataModel *m_model;
};
//! [0]

#endif
