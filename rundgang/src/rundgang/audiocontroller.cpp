/* Copyright (c) 2013 Research In Motion Limited.
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
#include "audiocontroller.h"
#include <QDir>
#include <bb/system/InvokeManager>
#include <bb/system/InvokeTargetReply>

using namespace bb::system;

AudioController::AudioController(QObject* parent): QObject(parent)
{
    // Retrieve the path to the app's working directory of the application.
    QString workingDir = QDir::currentPath();

    // Build the path, add it as a context property, and expose it to QML.
    mDirPaths.insert("voice", QVariant(QString("file://" + workingDir + "/shared/voice/")));
    emit dirPathsChanged(mDirPaths);

    mInvokeManager = new InvokeManager(this);
}

AudioController::~AudioController()
{
}

QVariantMap AudioController::dirPaths()
{
    return mDirPaths;
}

void AudioController::invokeAudio(QString path)
{
    InvokeRequest request;

    // Set up the Invocation request for an the email composer using
    // the incoming data.
    QString invokeTargetId = "sys.mediaplayer.previewer";
    QString invokeActionId = "bb.action.OPEN";

    request.setAction(invokeActionId);
    request.setTarget(invokeTargetId);
    request.setUri(path);

    // Finally make the invocation which will launch the media player and play the sound file.
    mInvokeManager->invoke(request);
}
