/*
 * Copyright (c) 2011, 2012, 2013 BlackBerry Limited.
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

#include "GroupArtistDataModelDecorator.hpp"
#include "GroupStyleDataQueryDecorator.hpp"
#include "DataQualityDataQueryDecorator.hpp"
#include "DataQualityModelDecorator.hpp"
#include "ExpandableDataModelDecorator.hpp"
#include "SelectionDataModelDecorator.hpp"
#include "DataModelLoggerDecorator.hpp"
#include "applicationui.hpp"

#include <bb/cascades/AbstractPane>
#include <bb/cascades/Application>
#include <bb/cascades/QmlDocument>

using namespace std;
using namespace bb::cascades;
//! [0]
ApplicationUI::ApplicationUI(bb::cascades::Application *app)
    : QObject(app)
{
    qmlRegisterType<GroupArtistDataModelDecorator>("bb.cascades.decorators", 1,
            2, "GroupArtistDataModelDecorator");
    qmlRegisterType<GroupStyleDataQueryDecorator>("bb.cascades.decorators", 1,
            2, "GroupStyleDataQueryDecorator");
    qmlRegisterType<DataQualityDataQueryDecorator>("bb.cascades.decorators", 1,
            2, "DataQualityDataQueryDecorator");
    qmlRegisterType<DataQualityModelDecorator>("bb.cascades.decorators", 1, 2,
            "DataQualityModelDecorator");
    qmlRegisterType<ExpandableDataModelDecorator>("bb.cascades.decorators", 1,
            2, "ExpandableDataModelDecorator");
    qmlRegisterType<SelectionDataModelDecorator>("bb.cascades.decorators", 1, 2,
            "SelectionDataModelDecorator");
    qmlRegisterType<DataModelLoggerDecorator>("bb.cascades.decorators", 1, 2,
            "DataModelLoggerDecorator");

    // create scene document from main.qml asset
    // set parent to created document to ensure it exists for the whole application lifetime
    QmlDocument *qml = QmlDocument::create("asset:///main.qml").parent(this);

    const QString workingDir = QDir::currentPath();
    const QString dirPaths = QString::fromLatin1(
            "file://%1/app/public/data/images/").arg(workingDir);
    m_sqlImages = QDir(workingDir + "/app/public/data/images");

    qml->documentContext()->setContextProperty("_sqlImagesDir", dirPaths);
    qml->documentContext()->setContextProperty("_app", this);

    // create root object for the UI
    AbstractPane *root = qml->createRootObject<AbstractPane>();
    // set created root object as a scene
    app->setScene(root);
}
//! [0]

bool ApplicationUI::fileExists(const QString file) {
    return m_sqlImages.exists(file);
}
