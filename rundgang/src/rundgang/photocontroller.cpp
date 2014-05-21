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
#include "photocontroller.h"
#include <bb/cascades/DisplayDirection>
#include <bb/cascades/OrientationSupport>
#include <bb/cascades/multimedia/CameraSettings>
#include <bb/device/DisplayInfo>
#include <libexif/exif-data.h>

using namespace bb::cascades;
using namespace bb::cascades::multimedia;

// Macro for getting the difference in x and y direction
#define DELTA(x, y) (x>y?(x-y):(y-x))

PhotoController::PhotoController(QObject* parent): QObject(parent)
{
}

PhotoController::~PhotoController()
{
}

void PhotoController::selectAspectRatio(bb::cascades::multimedia::Camera *camera)
{
    CameraSettings camsettings;
    camera->getSettings(&camsettings);

    bb::device::DisplayInfo display;
    float aspect = (float)(display.pixelSize().width()) / (float)(display.pixelSize().height());
    qDebug() << "Screen resolution: " << display.pixelSize().width() << "x" << display.pixelSize().height() << "aspect: " << aspect;

    if( display.aspectType() == bb::device::DisplayAspectType::Square )
    {
    	aspect = 1.0; //1:1
    }
    else // 16:9
    {
    	aspect = 0.5625;
    }

    // Get a list of supported resolutions.
    QVariantList reslist = camera->supportedCaptureResolutions(CameraMode::Photo);

    // Find the closest match to the aspect parameter
    for (int i = 0; i < reslist.count(); i++) {
        QSize res = reslist[i].toSize();

        if(  ((float)(res.width()) / (float)(res.height())) == aspect )
        {
        	//found the best aspect ratio, let's use that
            qDebug() << "found same aspect ratio: " <<  (float)(res.width()) / (float)(res.height());
            camsettings.setCaptureResolution(res);
        	break;
        }
    }

    // Update the camera setting
    camera->applySettings(&camsettings);
}

/*
void PhotoController::selectAspectRatio(bb::cascades::multimedia::Camera *camera, const float aspect)
{
    CameraSettings camsettings;
    camera->getSettings(&camsettings);
erer
    // Get a list of supported resolutions.
    QVariantList reslist = camera->supportedCaptureResolutions(CameraMode::Photo);

    // Find the closest match to the aspect parameter
    for (int i = 0; i < reslist.count(); i++) {
        QSize res = reslist[i].toSize();

        // Check for w:h or h:w within 5px margin of error...
        if ((DELTA(res.width() * aspect, res.height()) < 5)
                || (DELTA(res.width(), res.height() * aspect) < 5)) {
            camsettings.setCaptureResolution(res);
            break;
        }
    }

    // Update the camera setting
    camera->applySettings(&camsettings);
}
*/
void PhotoController::scaleImage(const QString imageFilePath, const float scaleFactor)
{
    // If the scaleFactor is one nothing is done, the image is at the correct scale already.
    if(scaleFactor != 1.0) {
        //Load the image using QImage.
        QImage originalImage = QImage(imageFilePath);

        if (!originalImage.isNull()) {
            ExifData *exifData = 0;
            ExifEntry *exifEntry = 0;
            int exifOrientation = 1;

            // Create a scaled version of the image.
            QImage scaledImage = originalImage.scaledToHeight(originalImage.height() * scaleFactor,
                    Qt::SmoothTransformation);

            // Since the image will loose its exif data when its opened in a QImage
            // it has to be manually rotated according to the exif in the image and device
            // orientation, this since the Camera control will not store the device orientation.
            // First get the device orientation, here the processing occur directly after the picture
            // is taken, so we can assume this is the actual orientation.
            DisplayDirection::Type displayDirection = OrientationSupport::instance()->displayDirection();

            // The exif data from the image at the imageFilePath.
            exifData = exif_data_new_from_file(imageFilePath.toLatin1().data());

            // Locate the orientation exif information.
            if (exifData != NULL) {
                for (int i = 0; i < EXIF_IFD_COUNT; i++) {
                    exifEntry = exif_content_get_entry(exifData->ifd[i], EXIF_TAG_ORIENTATION);

                    // If the entry corresponds to the orientation it will be a non zero pointer.
                    if (exifEntry) {
                        exifOrientation = exif_get_short(exifEntry->data, exif_data_get_byte_order(exifData));
                        break;
                    }
                }
            }

            // A transform will be used to rotate the image according to device and exif orientation.
            QTransform transform;

            // It's a bit tricky to get the correct orientation of the image. A combination of
            // the way the the device is oriented and what the actual exif data says has to be used
            // in order to rotate it in the correct way.
            if(exifOrientation == 6) {
                switch (displayDirection ) {
                    case DisplayDirection::North:
                        transform.rotate(displayDirection + 90);
                        break;
                    case DisplayDirection::West:
                        transform.rotate(displayDirection - 90);
                        break;
                    default:
                        // Do nothing for the other rotations
                        break;
                }
            } else {
                // First rotate the image according to the display direction
                transform.rotate(displayDirection);

                switch (displayDirection ) {
                    case DisplayDirection::East:
                        transform.rotate(displayDirection + 90);
                        break;
                    case DisplayDirection::West:
                        transform.rotate(displayDirection - 90);
                        break;
                    default:
                        // Do nothing for the other rotations
                        break;
                }
            }

            // Perform the rotation of the image before its saved.
            scaledImage = scaledImage.transformed(transform);

            if (!scaledImage.save(imageFilePath)) {
                qWarning() << "PhotoController::scaleImage: Failed to save image to path " << imageFilePath;
            }
        } else {
            qWarning() << "PhotoController::scaleImage: Failed to open image  " << imageFilePath;
        }
    }
}
