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

#ifndef PAGEDDATAMODEL_HPP
#define PAGEDDATAMODEL_HPP

#include <bb/cascades/QListDataModel>

//! [0]
class PagedDataModel : public bb::cascades::QVariantListDataModel
{
    Q_OBJECT

    Q_PROPERTY(int itemCount READ itemCount NOTIFY itemCountChanged)

public:
    PagedDataModel(QObject *parent = 0);

    Q_INVOKABLE void addItemsAt(const QVariantList &indexPath);

Q_SIGNALS:
    void itemCountChanged();

private:
    int itemCount() const;
};
//! [0]

#endif
