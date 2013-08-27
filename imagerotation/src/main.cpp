/*
 * Copyright (c) 2011, 2012, 2013  BlackBerry Limited.
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

#include "rotationsensor.hpp"

#include <bb/cascades/AbstractPane>
#include <bb/cascades/Application>
#include <bb/cascades/QmlDocument>

using namespace bb::cascades;

/**
 * This sample application shows the picture of Mona Lisa on the screen.
 * When the user rotates the device to the left or right-hand side, the picture
 * will always stay vertical to the ground.
 *
 * In this example we show how to use the QRotationSensor from the Qt sensors module
 * to calculate the current rotation angle of the device.
 */
Q_DECL_EXPORT int main(int argc, char **argv)
{
    Application app(argc, argv);

    // Create the rotation sensor object
    RotationSensor sensor;

    // Load the UI description from main.qml
    QmlDocument *qml = QmlDocument::create("asset:///main.qml").parent(&app);

    // Make the RotationSensor object available to the UI as context property
    qml->setContextProperty("_sensor", &sensor);

    // Create the application scene
    AbstractPane *appPage = qml->createRootObject<AbstractPane>();
    Application::instance()->setScene(appPage);

    return Application::exec();
}
