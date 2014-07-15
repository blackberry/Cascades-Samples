/* Copyright (c) 2012 BlackBerry Limited.
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
#ifndef __PHOTOBOMBERAPP_H__
#define __PHOTOBOMBERAPP_H__

#include <QtCore/QObject>
#include <QtCore/QMetaType>
#include <bb/cascades/multimedia/Camera>

#include <bb/cascades/Application>

using namespace bb::cascades;
using namespace bb::cascades::multimedia;

namespace bb
{
    namespace cascades
    {
        class Invocation;
    }
}

/**
 * Photobomber Description:
 *
 * The PhotoBomber is a camera application that lets you take photos and then have them
 * "bombed" by someone that jumps in, automatically.
 *
 * We use the Camera control from cascades multi-media library. This control needs to be
 * initiated from C++ code before we can use it though.
 *
 * Feature Summary
 * - Use the Invoke framework to start an application
 * - Register the Camera control from the multi-media library
 * - Connect to the camera's shutter fired signal
 */

class PhotoBomberApp: public QObject
{
    Q_OBJECT

public:
    /**
     * This is our constructor that sets up the application. We register the multi-media library
     * Camera in QML so it can accessed from the QML code.
     */
    PhotoBomberApp(QObject *parent = 0);
    ~PhotoBomberApp();

    // This is where the magic is done: gray-scaling the image and adding a bomber image.
    Q_INVOKABLE void manipulatePhoto(const QString &fileName);

    /**
     *  Function that sets up an Invocation of the pictures app card
     *
     *  @param fileName the file path to the bombed image
     */
    Q_INVOKABLE void showPhotoInCard(const QString fileName);

    /**
     *  Function that lets you set up the aspect ratio of the camera, there
     *  are limitations to the allowed values, the function will look for the
     *  closest match.
     *
     *  @param camera the file path to the bombed image
     */
    Q_INVOKABLE void selectAspectRatio(bb::cascades::multimedia::Camera *camera);
private:
    /**
     * Helper function that opens an image in a QImage and rotates it according to the
     * device orientation and EXIF data in the image on file.
     *
     * @param imageFilePath the path to the picture file.
     * @return A QImage containing the rotated picture.
     */
    QImage getRotateImage(const QString imageFilePath);

};

#endif // ifndef __PHOTOBOMBERAPP_H__
