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

#include "HelloBattery.hpp"

#include <bb/cascades/Application>
#include <bb/cascades/QmlDocument>
#include <bb/cascades/AbstractPane>

#include <bb/device/BatteryInfo>
#include <bb/Exception>

#include <iostream>

using namespace bb::cascades;
HelloBattery::HelloBattery()
{

    try {
//! [0]
        QmlDocument *qml = QmlDocument::create("HelloBattery.qml");
        // bind battery info to the qml context
        qml->setContextProperty("battery", new bb::device::BatteryInfo(this));
//! [0]
        AbstractPane *root = qml->createRootNode<AbstractPane>();
        Application::instance()->setScene(root);

    } catch(bb::Exception & ex) {
    	qDebug() << "exception.what() = " << ex.what();
    } catch(...) {
		qDebug() << "ouch!?";
	}
}
