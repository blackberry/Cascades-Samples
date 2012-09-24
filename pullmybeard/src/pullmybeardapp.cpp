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
#include "pullmybeardapp.h"

#include <bb/cascades/Page>
#include <bb/cascades/QmlDocument>

using namespace bb::cascades;

PullMyBeardApp::PullMyBeardApp()
{

    // Create a QMLDocument and load it with pullmybread.qml, using build patterns.
    QmlDocument *qml = QmlDocument::create("asset:///pullmybeard.qml");

    if (!qml->hasErrors()) {

        // The application Page is created from QML.
        Page *appPage = qml->createRootObject<Page>();

        if (appPage) {

            // Set the context property that we want to use from inside the QML. Functions that
            // are exposed via Q_INVOKABLE will be found with this property and the name of the function.
            qml->setContextProperty("pullMyBeardApp", this);

            // Initialize the mSoundManager with a directory that resides in the
            // assets directory and that only contains playable files.
            mSoundManager = new SoundManager("sounds/");

            // Set the main scene for the application.
            Application::instance()->setScene(appPage);
        }
    }
}

PullMyBeardApp::~PullMyBeardApp()
{
    delete mSoundManager;
}

// This invokable function we call from the QML file to play a sound.
void PullMyBeardApp::playSound(const QString &msg)
{
    mSoundManager->play(msg);
}
