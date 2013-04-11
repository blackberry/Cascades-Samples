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

#include "app.hpp"

#include <bb/cascades/AbstractPane>
#include <bb/cascades/Application>
#include <bb/cascades/QmlDocument>

using namespace bb::cascades;
using namespace bb::system;

//! [0]
App::App(QObject *parent)
	: QObject(parent)
	, m_invokeManager(new InvokeManager(this))
{
	// Listen to incoming invocation requests
	connect(m_invokeManager, SIGNAL(invoked(const bb::system::InvokeRequest&)),
			this, SLOT(handleInvoke(const bb::system::InvokeRequest&)));
	connect(m_invokeManager,
			SIGNAL(cardResizeRequested(const bb::system::CardResizeMessage&)),
			this, SLOT(resized(const bb::system::CardResizeMessage&)));
	connect(m_invokeManager,
			SIGNAL(cardPooled(const bb::system::CardDoneMessage&)), this,
			SLOT(pooled(const bb::system::CardDoneMessage&)));

	m_source = m_target = m_action = m_mimeType = m_uri = m_data = m_status =
			tr("--");

	// Initialize properties with default values
	switch (m_invokeManager->startupMode()) {
	case ApplicationStartupMode::LaunchApplication:
		m_startupMode = tr("Launch");
		initFullUI();
		break;
	case ApplicationStartupMode::InvokeApplication:
		// Wait for invoked signal to determine and initialize the appropriate UI
		m_startupMode = tr("Invoke");
		break;
	case ApplicationStartupMode::InvokeCard:
		// Wait for invoked signal to determine and initialize the appropriate UI
		m_startupMode = tr("Card");
		break;
	}

	// Create the UI

}
//! [0]

//! [1]
void App::handleInvoke(const InvokeRequest& request) {
	// Copy data from incoming invocation request to properties
	m_source =
			QString::fromLatin1("%1 (%2)").arg(request.source().installId()).arg(
					request.source().groupId());
	m_target = request.target();
	m_action = request.action();
	m_mimeType = request.mimeType();
	m_uri = request.uri().toString();
	m_data = QString::fromUtf8(request.data());

	if (m_target == "com.example.bb10samples.invocation.sharecomposer") {
		initComposerUI();
	} else if (m_target
			== "com.example.bb10samples.invocation.imagepreviewer") {
		initPreviewerUI();
	} else if (m_target == "com.example.bb10samples.invocation.eggpicker") {
		initPickerUI();
	}

	// Signal that the properties have changed
	emit requestChanged();
}
//! [1]

void App::initFullUI() {
	QmlDocument *qml = QmlDocument::create("asset:///main.qml");
	qml->setContextProperty("_app", this);
	AbstractPane *root = qml->createRootObject<AbstractPane>();
	Application::instance()->setScene(root);
}

void App::initComposerUI() {
	QmlDocument *qml = QmlDocument::create("asset:///Composer.qml");
	qml->setContextProperty("_app", this);
	AbstractPane *root = qml->createRootObject<AbstractPane>();
	Application::instance()->setScene(root);
}

void App::initPreviewerUI() {
	QmlDocument *qml = QmlDocument::create("asset:///Previewer.qml");
	qml->setContextProperty("_app", this);
	AbstractPane *root = qml->createRootObject<AbstractPane>();
	Application::instance()->setScene(root);
}

void App::initPickerUI() {
	QmlDocument *qml = QmlDocument::create("asset:///Picker.qml");
	qml->setContextProperty("_app", this);
	AbstractPane *root = qml->createRootObject<AbstractPane>();
	Application::instance()->setScene(root);
}

void App::cardDone(const QString& msg) {
	// Assemble message
	CardDoneMessage message;
	message.setData(msg);
	message.setDataType("text/plain");
	message.setReason(tr("Success!"));

	// Send message
	m_invokeManager->sendCardDone(message);
}

void App::resized(const bb::system::CardResizeMessage&) {
	m_status = tr("Resized");
	emit statusChanged();
}

void App::pooled(const bb::system::CardDoneMessage& doneMessage) {
	m_status = tr("Pooled");
	m_source = m_target = m_action = m_mimeType = m_uri = m_data = tr("--");
	emit statusChanged();
	emit requestChanged();
}

QString App::startupMode() const {
	return m_startupMode;
}

QString App::source() const {
	return m_source;
}

QString App::target() const {
	return m_target;
}

QString App::action() const {
	return m_action;
}

QString App::mimeType() const {
	return m_mimeType;
}

QString App::uri() const {
	return m_uri;
}

QString App::data() const {
	return m_data;
}

QString App::status() const {
	return m_status;
}
