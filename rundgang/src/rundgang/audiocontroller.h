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
#ifndef AUDIOCONTROLLER_H_
#define AUDIOCONTROLLER_H_

#include <QObject>
#include <QVariantMap>

namespace bb {
    namespace system {
        class InvokeManager;
    }
}

using namespace bb::system;

/**
 * AudioController Description:
 *
 * This class contain functionality for handling sound. Getting the location
 * for storing a recording and invoking the mediaplayer card to play a sound.
 */
class AudioController: public QObject
{
    Q_OBJECT

    /**
     * Property holding directory paths in the current example only one
     * path is in fact used but this might be extended.
     */
    Q_PROPERTY(QVariantMap dirPaths READ dirPaths NOTIFY dirPathsChanged)

public:
    AudioController(QObject *parent = 0);
    virtual ~AudioController();

    /**
     * The directory paths to where sound can be stored.
     *
     * @return A map containing different paths for storing audio files.
     */
    QVariantMap dirPaths();

    /**
     * Inovkes the media player card and plays a sound file.
     *
     * @param path The file path to the audio to play in the media player card
     */
    Q_INVOKABLE void invokeAudio(QString path);

signals:
    /**
     * Signal emitted when the directory path property is changed.
     */
    void dirPathsChanged(QVariantMap  dirPaths);

private:
    // Variable for storing the directory paths.
    QVariantMap mDirPaths;

    // Invocation manager used to launch an application card.
    InvokeManager *mInvokeManager;
};

#endif /* AUDIOCONTROLLER_H_ */
