/* Copyright (c) 2013, 2014 BlackBerry Limited.
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
#include "appshotapp.h"

#include <bb/cascades/Application>
#include <bb/cascades/QmlDocument>
#include <bb/cascades/AbstractPane>
#include <bb/system/InvokeRequest>
#include <bb/system/InvokeManager>
#include <bb/system/InvokeTargetReply>

using namespace bb::cascades;
using namespace bb::system;

const QString AppShotApp::SHOW_INSTRUCTIONS_KEY("showInstructions");

AppShotApp::AppShotApp(QObject *parent ) :
        QObject(parent), mShowInstruction(false)
{
    // Set up the application organization and name (used by QSettings
    // when saving values to the persistent store).
    QCoreApplication::setOrganizationName("Example");
    QCoreApplication::setApplicationName("App Shot");
    QSettings settings;

    // Check if the showInstruction property is set if it is not it means
    // this is the first time the app launches and the instruction should be shown.
    if (settings.value(SHOW_INSTRUCTIONS_KEY).isNull()) {
    	    setShowInstruction(true);
	}

    // Register a timer used when hiding the action bar for screen shot.
    qmlRegisterType < QTimer > ("com.appshot", 1, 0, "QTimer");

    // Create scene document from main.qml asset, the parent is set
    // to ensure the document gets destroyed properly at shut down.
    QmlDocument *qml = QmlDocument::create("asset:///main.qml").parent(this);

    qml->setContextProperty("_app", this);

    // Create root object for the UI
    AbstractPane *root = qml->createRootObject<AbstractPane>();

    // Set created root object as the application scene
    Application::instance()->setScene(root);
}

void AppShotApp::showPhotoInCard(const QString fileName)
{
    // Create InvokeManager and InvokeRequest objects to able to invoke a card with a viewer that will show the
    // snapped screen shot. Requires libbbsystem.so to be added (System APIs for the clipboard, toasts, dialogs,
	// and inter-application invocation).
    bb::system::InvokeManager manager;
    bb::system::InvokeRequest request;

    // Setup what to show and in what target.
    request.setUri(QUrl::fromLocalFile(fileName));
    request.setTarget("sys.pictures.card.previewer");
    request.setAction("bb.action.VIEW");
    InvokeTargetReply *targetReply = manager.invoke(request);

    // Setting the parent to "this" will make the manager live on after this function is destroyed
    manager.setParent(this);

    if (targetReply == NULL) {
        qDebug() << "InvokeTargetReply is NULL: targetReply = " << targetReply;
    } else {
        targetReply->setParent(this);
    }
}

void AppShotApp::setShowInstruction(bool showInstruction) {
	if(showInstruction != mShowInstruction) {
	    mShowInstruction = showInstruction;
		emit showInstructionChanged(mShowInstruction);

		// Store the value in QSettings so that it persist between app sessions.
        QSettings().setValue(SHOW_INSTRUCTIONS_KEY, QVariant(showInstruction));
	}
}

bool AppShotApp::showInstruction() {
	return mShowInstruction;
}
