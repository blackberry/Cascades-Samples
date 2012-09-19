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
#include "cowbellapp.h"

#include <bb/cascades/Page>
#include <bb/cascades/QmlDocument>

using namespace bb::cascades;

CowBellApp::CowBellApp()
{
  // Obtain a QMLDocument and load it into the qml variable, using build patterns.
  QmlDocument *qml = QmlDocument::create("asset:///cowbell.qml");

  // If the QML document is valid, we process it.
  if (!qml->hasErrors()) {

    // Create the application Page from QMLDocument.
    Page *appPage = qml->createRootObject<Page>();

    if (appPage) {

      // Set the context property we want to use from inside the QML document. Functions exposed
      // via Q_INVOKABLE will be found with this property and the name of the function.
      qml->setContextProperty("cowbellApp", this);

      // Initialize the sound manager with a directory that resides in the
      // assets directory which only contains playable files.
      mSoundManager = new SoundManager("sounds/");

      // Set the main scene for the application to the Page.
      Application::instance()->setScene(appPage);

    }
  }
}

CowBellApp::~CowBellApp()
{
  // Destroy the sound manager.
  delete mSoundManager;
}

void CowBellApp::playSound(const QString &msg)
{
  // Play sound determined by QString parameter
  mSoundManager->play(msg);
}

