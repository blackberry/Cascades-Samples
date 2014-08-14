/* Copyright (c) 2013 BlackBerry Limited.
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

#include "activeFrameQML.h"
#include <bb/cascades/Container>
#include <bb/cascades/Application>

using namespace bb::cascades;

ActiveFrameQML::ActiveFrameQML(QObject *parent) :
		SceneCover(parent), isActiveFrame(false) {

	// Create active frame document from AppCover.qml asset
	// set parent to created document to ensure it exists for the whole application lifetime
	m_qml = QmlDocument::create("asset:///AppCover.qml").parent(parent);
	m_qml->setContextProperty("activeFrame", this);
	Container *mainContainer = m_qml->createRootObject<Container>();
	setContent(mainContainer);


	// Setup connections to react to application changes

	// If any Q_ASSERT statement(s) indicate that the slot failed to connect to
	// the signal, make sure you know exactly why this has happened. This is not
	// normal, and will cause your app to stop working!!
	bool connectResult;

	// Since the variable is not used in the app, this is added to avoid a
	// compiler warning.
	Q_UNUSED(connectResult);

	connectResult = QObject::connect(Application::instance(), SIGNAL(thumbnail()), this, SLOT(backgrounded()));

	// This is only available in Debug builds.
	Q_ASSERT(connectResult);

	connectResult = QObject::connect(Application::instance(), SIGNAL(fullscreen()), this, SLOT(foregrounded()));
	Q_ASSERT(connectResult);

	connectResult = QObject::connect(Application::instance(), SIGNAL(invisible()), this, SLOT(foregrounded()));
	Q_ASSERT(connectResult);

}

void ActiveFrameQML::foregrounded() {
	isActiveFrame = false;
}

void ActiveFrameQML::backgrounded() {
	isActiveFrame = true;
	update();
}

void ActiveFrameQML::update() {
	// TODO: Replace with data-driven update instead.
	if (isActiveFrame) {
		QTimer::singleShot(10000, this, SLOT(update()));
		emit updateChanged();
	}
}

void ActiveFrameQML::setContextProperty(QString propertyName, QObject *object) {
	m_qml->setContextProperty(propertyName, object);
}
