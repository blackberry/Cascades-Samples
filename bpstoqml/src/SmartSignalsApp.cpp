/* Copyright (c) 2012-2013 BlackBerry Limited.
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
#include <QList>
#include <QDebug>

#include "SmartSignalsApp.hpp"

using namespace bb::cascades;

SmartSignalsApp::SmartSignalsApp() {
	// Load the UI description from main.qml
    QmlDocument *qml = QmlDocument::create("asset:///main.qml");
    if (qml->hasErrors()) {
    	QList<QDeclarativeError> errors = qml->errors();
    	qDebug() << "Errors occurred parsing qml:";
    	qDebug() << &errors;
    } else {
		qml->setContextProperty("cs", this);

		// Create the application scene
	    AbstractPane *root = qml->createRootObject<AbstractPane>();
	    Application::instance()->setScene(root);
    }
}
