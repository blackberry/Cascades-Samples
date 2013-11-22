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

#include "PizzeriaSearcher.hpp"

using namespace bb::cascades;
using namespace bb::data;

//! [1]
PizzeriaSearcher::PizzeriaSearcher(QObject *parent)
    : QObject(parent)
    , m_model(new GroupDataModel(this))
    , m_dataSource(new DataSource(this))
{
    // Disable item grouping in the data model
    m_model->setGrouping(ItemGrouping::None);

    // Set the XPath expression to extract the result items from the XML document
    m_dataSource->setQuery(QLatin1String("/ResultSet/Result"));

    // Invoke out dataLoaded slot whenever the data source has loaded and parsed the XML document
    bool ok = connect(m_dataSource, SIGNAL(dataLoaded(QVariant)), this, SLOT(dataLoaded(QVariant)));
    Q_ASSERT(ok);
    Q_UNUSED(ok);
}
//! [1]

//! [2]
void PizzeriaSearcher::dataLoaded(const QVariant &data)
{
    // Clear the model ...
    m_model->clear();

    // ... and fill it with the search result data
    m_model->insertList(data.toList());
}
//! [2]

GroupDataModel* PizzeriaSearcher::model() const
{
    return m_model;
}

QString PizzeriaSearcher::zipCode() const
{
    return m_zipCode;
}

//! [3]
void PizzeriaSearcher::setZipCode(const QString &zipCode)
{
    if (m_zipCode == zipCode)
        return;

    m_zipCode = zipCode;
    emit zipCodeChanged();
    m_dataSource->setSource(QUrl("asset:///pizzaplaces.xml"));
    m_dataSource->load();
}
//! [3]
