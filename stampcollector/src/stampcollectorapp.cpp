/* Copyright (c) 2012, 2013, 2014 BlackBerry Limited.
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
#include <bb/cascades/Page>
#include <bb/cascades/QmlDocument>
#include <bb/data/JsonDataAccess>

using namespace bb::data;

StampCollectorApp::StampCollectorApp(QObject *parent) : QObject(parent)
{
    // The entire UI is a drill down list, which means that when an item is selected
    // a navigation takes place to a Content View with a large stamp image and a descriptive text.
    // The main.qml contain the navigation pane and the first page (the list).
    QmlDocument *qml = QmlDocument::create("asset:///main.qml").parent(this);
    NavigationPane *nav = qml->createRootObject<NavigationPane>();

    // Get the ListView from QML and setup a DataModel from the JSON data.
    // Connect to the stampList triggered signal to the onListTriggered Slot function.
    ListView *stampList = nav->findChild<ListView*>("stampList");
    setUpStampListModel(stampList);

    // Create the application scene.
    Application::instance()->setScene(nav);
}

void StampCollectorApp::setUpStampListModel(ListView *stampList)
{
    // Create a new GroupDataModel; the GroupDataModel is a helper class that ListView uses for data handling.
    // It uses in-memory storage so we can populate data.
    // We load the GroupDataModel with help from the JsonDataAcces function, load().
    JsonDataAccess jda;

    QVariantList mainList = jda.load("app/native/assets/stamps.json").value<QVariantList>();

    if (jda.hasError()) {
        bb::data::DataAccessError error = jda.error();
        qDebug() << "JSON loading error: " << error.errorType() << ": " << error.errorMessage();
        return;
    }

    // Sort on region in the model so we will get different categories.
    GroupDataModel *stampModel = new GroupDataModel(QStringList() << "region");
    stampModel->setParent(this);
    stampModel->insertList(mainList);
    stampModel->setGrouping(ItemGrouping::ByFullValue);

    stampList->setDataModel(stampModel);
}
