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
#include <bb/cascades/Page>
#include <bb/cascades/QmlDocument>
#include <bb/data/JsonDataAccess>

StampCollectorApp::StampCollectorApp()
{
    // The entire UI is a drill down list, which means that when an item is selected
    // a navigation takes place to a Content View with a large stamp image and a descriptive text.
    // The main.qml contain the navigation pane and the first page (the list).
    QmlDocument *qml = QmlDocument::create().load("main.qml");
    qml->setParent(this);
    mNav = qml->createRootNode<NavigationPane>();

    // We get the ListView from QML, so that we can connect to the selctionChange signal and set
    // up a DataModel for JSON data.
    ListView *stampList = mNav->findChild<ListView*>("stampList");
    setUpStampListModel(stampList);
    QObject::connect(stampList, SIGNAL(selectionChanged(const QVariantList, bool)), this,
            SLOT(onSelectionChanged(const QVariantList, bool)));

    // The second page, with a detailed description and large stamp image is set up in QML
    // Navigation to this page is handled in the onSelectionChanged Slot function.
    QmlDocument *contentQml = QmlDocument::create().load("ContentPage.qml");
    contentQml->setParent(this);
    mQmlContext = contentQml->documentContext();

    // Set up a context property to the navigation pane so that it is possible to navigate back to the list
    // and create the content page.
    mQmlContext->setContextProperty("_nav", mNav);
    mContentPage = contentQml->createRootNode<Page>();

    // Create the application scene and we are done.
    Application::setScene(mNav);
}

void StampCollectorApp::setUpStampListModel(ListView *stampList)
{
    // Here we create a new GroupDataModel, the GroupDataModel is in-memory data that we
    // populate with data we load with help from the JsonDataAcces function.
    JsonDataAccess jda("app/native/assets/stamps.json");
    
    QVariantList mainList = jda.load().value<QVariantList>();

    if (jda.hasError()) {
        bb::data::DataAccessError* error = jda.error();
        qDebug() << "Json loading error: " << error->errorType() << ": " << error->errorMessage();
    }

    // A GroupDataModel is a helper class that the list uses for data handling.
    // We sort on region in the model, this way will get different categories.
    mStampModel = new GroupDataModel(QStringList() << "region");
    mStampModel->setParent(this);
    mStampModel->clear();
    mStampModel->insert(mainList);
    mStampModel->setGrouping(ItemGrouping::ByFullValue);

   stampList->setDataModel(mStampModel);
}

void StampCollectorApp::onSelectionChanged(const QVariantList indexPath, bool selected)
{
    if (selected) {
        // We use the sender to get the list view for accessing the data model and then the actual data.
        if(sender()) {
            ListView* stampList = dynamic_cast<ListView*>(sender());
            DataModel* stampModel = stampList->dataModel();

            // Update the content view context property so that it corresponds to
            // the selected item and navigate to the page.
            QVariantMap map = stampModel->data(indexPath).toMap();
            mQmlContext->setContextProperty("_contentView", map);
            mNav->push(mContentPage);
        }
    }
}

