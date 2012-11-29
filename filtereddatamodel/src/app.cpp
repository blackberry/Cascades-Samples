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

#include <bb/cascades/AbstractPane>
#include <bb/cascades/Application>
#include <bb/cascades/QmlDocument>

#include "app.hpp"

#include "filtereddatamodel.hpp"
#include "vegetablesdatamodel.hpp"

using namespace bb::cascades;

//! [0]
App::App(QObject *parent)
    : QObject(parent)
    , m_model(0)
{
    // Create the source data model
    VegetablesDataModel *vegetablesModel = new VegetablesDataModel(this);

    // Create the filtered data model and pass the source model
    m_model = new FilteredDataModel(vegetablesModel, this);

    QmlDocument *qml = QmlDocument::create("asset:///main.qml").parent(this);
    qml->setContextProperty("_model", m_model);
    qml->setContextProperty("_app", this);

    AbstractPane* root = qml->createRootObject<AbstractPane>();
    Application::instance()->setScene(root);
}
//! [0]

//! [1]
void App::selectionChanged(const QVariantList &indexPath, bool selected)
{
    if (indexPath.size() != 1 || !selected)
        return; // Not interested

    // Selected a header item!
    const int selection = indexPath[0].toInt();

    // Toggle the expanded state of the selected header
    m_model->expandHeader(selection, !m_model->isHeaderExpanded(selection));
}
//! [1]
