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

#include <bb/cascades/Application>
#include <bb/cascades/QmlDocument>
#include <bb/cascades/AbstractPane>
#include <bb/cascades/ListView>

#include "app.h"
#include "FilteredDataModel.h"

using namespace bb::cascades;

App::App()
{
    qmlRegisterType<FilteredDataModel>("custom.lib", 1, 0, "FilteredDataModel");

    QmlDocument *qml = QmlDocument::create("main.qml");
    qml->setContextProperty("cs", this);

    AbstractPane* root = qml->createRootNode<AbstractPane>();
    if (root != 0)
    {
        ListView* list = root->findChild<ListView*>("myList");

        connect(list, SIGNAL(selectionChanged(const QVariantList, bool)),
                this, SLOT(onSelectionChanged(const QVariantList, bool)));

        Application::setScene(root);
    }
}

void App::onSelectionChanged(const QVariantList indexPath, bool selected)
{
    if (indexPath.size() != 1 || !selected)
        return; // Not interested

    // Selected a header item!
    // Inform the database to change what it gives us
    ListView* list = dynamic_cast<ListView*>(sender());
    if (list)
    {
        FilteredDataModel* dm = dynamic_cast<FilteredDataModel*>(list->dataModel());
        if (dm)
        {
            int selection = indexPath[0].toInt();
            bool expand = true;
            if (dm->isHeaderExpanded(selection))
            {
                expand = false;
            }
            dm->expandHeader(selection, expand);
        }
    }
}
