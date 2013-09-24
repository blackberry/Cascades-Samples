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

#ifndef PIZZERIASEARCHER_HPP
#define PIZZERIASEARCHER_HPP

#include <bb/cascades/GroupDataModel>
#include <bb/data/DataSource>

#include <QtCore/QObject>

/**
 * @short A helper class to search pizzerias on the internet.
 *
 * This class encapsulates the search for pizzerias in a given region
 * via the Yahoo REST webservice and provides the results in a data model.
 */
//! [0]
class PizzeriaSearcher : public QObject
{
    Q_OBJECT

    // The list of found pizzerias
    Q_PROPERTY(bb::cascades::GroupDataModel* model READ model CONSTANT)

    // The zip code of the region to search for pizzerias
    Q_PROPERTY(QString zipCode READ zipCode WRITE setZipCode NOTIFY zipCodeChanged)

public:
    PizzeriaSearcher(QObject *parent = 0);

Q_SIGNALS:
    void zipCodeChanged();

private Q_SLOTS:
    void dataLoaded(const QVariant &data);

private:
    bb::cascades::GroupDataModel* model() const;

    QString zipCode() const;
    void setZipCode(const QString &zipCode);


    bb::cascades::GroupDataModel *m_model;
    bb::data::DataSource *m_dataSource;
    QString m_zipCode;
};
//! [0]

#endif
