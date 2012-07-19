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
#include "stampcollectorapp.h"

#include <bb/cascades/Container>
#include <bb/cascades/GroupDataModel>
#include <bb/cascades/ListView>
#include <bb/cascades/NavigationPane>
#include <bb/cascades/QmlDocument>
#include <bb/data/JsonDataAccess>

using namespace bb::data;

StampCollectorApp::StampCollectorApp()
{
    // The entire UI is a drill down list, which means that when an item is selected
    // a navigation takes place to a Content View with a large stamp image and a descriptive text.
    // The main.qml contain the navigation pane and the first page (the list).
    QmlDocument *qml = QmlDocument::create().load("main.qml");
    qml->setParent(this);
    NavigationPane *nav = qml->createRootNode<NavigationPane>();

    // We get the ListView from QML, so that we can connect to the selctionChange signal and set
    // up a DataModel for JSON data.
    ListView *stampList = nav->findChild<ListView*>("stampList");
    setUpStampListModel(stampList);

    // Create the application scene and we are done.
    Application::instance()->setScene(nav);
}


void StampCollectorApp::setUpStampListModel(ListView *stampList)
{
    // Here we create a new GroupDataModel, the GroupDataModel is in-memory data that we
    // populate with data we load with help from the JsonDataAcces function.
    JsonDataAccess jda;
    
    QVariantList mainList = jda.load("app/native/assets/stamps.json").value<QVariantList>();

    if (jda.hasError()) {
        bb::data::DataAccessError error = jda.error();
        qDebug() << "JSON loading error: " << error.errorType() << ": " << error.errorMessage();
        return;
    }

    // A GroupDataModel is a helper class that the list uses for data handling.
    // We sort on region in the model, this way will get different categories.
    GroupDataModel *stampModel = new GroupDataModel(QStringList() << "region");
    stampModel->setParent(this);
    stampModel->insertList(mainList);
    stampModel->setGrouping(ItemGrouping::ByFullValue);

    stampList->setDataModel(stampModel);
}
