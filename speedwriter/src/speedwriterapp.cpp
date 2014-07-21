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
#include "speedwriterapp.h"
#include "speedgauge.h"
#include "wordchecker.h"

#include <bb/cascades/Page>
#include <bb/cascades/QmlDocument>

using namespace bb::cascades;

SpeedWriterApp::SpeedWriterApp(QObject *parent) : QObject(parent)
{
    // Registration of the CustomControl which will show the current speed and
    // the WordChecker object that is used for checking if the entered text is
    // correct or not. qmlRegisterType will make the objects recognizable by the
    // QML parsing engine.
    qmlRegisterType<SpeedGauge>("com.speedwriter", 1, 0, "SpeedGauge");
    qmlRegisterType<WordChecker>("com.speedwriter", 1, 0, "WordChecker");

    // Create a QMLDocument and load it, using build patterns
    QmlDocument *qml = QmlDocument::create("asset:///speedwriter.qml").parent(this);

    if (!qml->hasErrors()) {
        // Create the application Page from QML.
        Page *appPage = qml->createRootObject<Page>();

        if (appPage) {
            // Set the application scene
            Application::instance()->setScene(appPage);
        }
    }
}

SpeedWriterApp::~SpeedWriterApp()
{
}
