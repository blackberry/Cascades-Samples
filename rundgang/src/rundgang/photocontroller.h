/* Copyright (c) 2013 BlackBerry Limited.
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
#ifndef PHOTOCONTROLLER_H_
#define PHOTOCONTROLLER_H_

#include <QObject>
#include <bb/cascades/multimedia/Camera>

using namespace bb::cascades::multimedia;

/**
 * PhotoController Description:
 *
 * This class contain functionality that extends the functionality of
 * the Camera Control.
 */
class PhotoController: public QObject
{
    Q_OBJECT
public:
    PhotoController(QObject* parent = 0);
    virtual ~PhotoController();

    /**
     *  Function that set up the aspect ratio of a camera, there
     *  are limitations to the allowed values, the function will look for the
     *  closest match.
     *
     *  @param camera the file path to the bombed image.
     */
    Q_INVOKABLE void selectAspectRatio(bb::cascades::multimedia::Camera *camera);

    /**
     * Scales the image at a certain location to a given scale factor and replaces
     * the image with the scaled image.
     *
     * @param imageFilePath The path to the image that will be scaled.
     * @param scaleFactor Decides in what way the image will be scaled.
     */
    Q_INVOKABLE void scaleImage(const QString imageFilePath, const float scaleFactor);

};

#endif /* PHOTOCONTROLLER_H_ */
