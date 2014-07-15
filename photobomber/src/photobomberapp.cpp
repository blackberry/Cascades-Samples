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

#include "photobomberapp.h"

#include <QtGui/QImage>
#include <QtGui/QImageReader>

#include <bb/cascades/Page>
#include <bb/cascades/QmlDocument>

#include <bb/cascades/multimedia/CameraSettings>

#include <bb/system/InvokeRequest>
#include <bb/system/InvokeManager>
#include <bb/system/InvokeTargetReply>

#include <bb/device/DisplayInfo>

#include <libexif/exif-data.h>

using namespace bb::cascades;
using namespace bb::cascades::multimedia;
using namespace bb::system;

// Macro for getting the difference in x and y direction
#define DELTA(x, y) (x>y?(x-y):(y-x))

PhotoBomberApp::PhotoBomberApp(QObject *parent) :
        QObject(parent)
{
    // Create a QMLDocument and load it, using build patterns
    QmlDocument *qml = QmlDocument::create("asset:///main.qml").parent(this);

    qml->setContextProperty("_photoBomber", this);

    if (!qml->hasErrors()) {
        // The application Page is created from QML.
        Page *appPage = qml->createRootObject<Page>();

        if (appPage) {

            // Set the application scene.
            Application::instance()->setScene(appPage);
        }
    }
}

PhotoBomberApp::~PhotoBomberApp()
{
}

void PhotoBomberApp::selectAspectRatio(bb::cascades::multimedia::Camera *camera)
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

void PhotoBomberApp::showPhotoInCard(const QString fileName)
{
    // Create InvokeManager and InvokeRequest objects to able to invoke a card with a viewer that will show the
    // latest bomber photo.
    bb::system::InvokeManager manager;
    bb::system::InvokeRequest request;

    // Setup what to show and in what target.
    request.setUri(QUrl::fromLocalFile(fileName));
    request.setTarget("sys.pictures.card.previewer");
    request.setAction("bb.action.VIEW");
    InvokeTargetReply *targetReply = manager.invoke(request);
    //setting the parent to "this" will make the manager live on after this function is destroyed
    manager.setParent(this);

    if (targetReply == NULL) {
        qDebug() << "InvokeTargetReply is NULL: targetReply = " << targetReply;
    } else {
        targetReply->setParent(this);
    }
}

void PhotoBomberApp::manipulatePhoto(const QString &fileName)
{
    QImageReader reader;

    // Set image name from the given file name.
    reader.setFileName(fileName);
    QImage image = getRotateImage(fileName); //reader.read();
    QSize imageSize = image.size();
    QColor color;

    // Gray it out! (this is not the gray-scale algorithm that should be used)
    for (int i = 0; i < imageSize.width(); i++)
        for (int ii = 0; ii < imageSize.height(); ii++) {
            color = QColor(image.pixel(i, ii));
            color.setRed((color.red() + color.green() + color.blue()) / 3);
            color.setGreen(color.red());
            color.setBlue(color.red());
            image.setPixel(i, ii, color.rgb());
        }

    // Paint an image on top of another image, so we add the gray-scaled image first.
    QPainter merger(&image);

    QString appFolder(QDir::homePath());
    appFolder.chop(4);

    QString bomberFileName;
    QString bombfolder;

    // The aspect ratio value is used for selecting correct folder for bomber photos.
    float imageWidth = imageSize.width();
    float imageHeight = imageSize.height();
    float aspectRatio = imageWidth / imageHeight;

    if (aspectRatio == 1) {
        bombfolder = appFolder + "app/native/assets/720x720/images/bombers/";
    } else {
        bombfolder = appFolder + "app/native/assets/images/bombers/";
    }

    // Positions for the bombers; we need these so we can overlay the bomber image at it's correct position.
    // The reason for not making an image as large as the picture is so we can change resolution and or switch
    // between portrait/landscape if we would want to.
    enum positions
    {
        TOP, CENTER, BOTTOM, LEFT, RIGHT
    };

    int vertical = RIGHT;
    int horizontal = BOTTOM;

    switch (random() % 9) {
        case 0:
            bomberFileName = bombfolder + "cow.png";
            vertical = RIGHT;
            horizontal = CENTER;
            break;
        case 1:
            bomberFileName = bombfolder + "crow.png";
            vertical = RIGHT;
            horizontal = BOTTOM;
            break;
        case 2:
            bomberFileName = bombfolder + "dog_2.png";
            vertical = RIGHT;
            horizontal = BOTTOM;
            break;
        case 3:
            bomberFileName = bombfolder + "larsenby.png";
            vertical = RIGHT;
            horizontal = BOTTOM;
            break;
        case 4:
            bomberFileName = bombfolder + "dog_1.png";
            vertical = LEFT;
            horizontal = BOTTOM;
            break;
        case 5:
            bomberFileName = bombfolder + "snail.png";
            vertical = LEFT;
            horizontal = BOTTOM;
            break;
        case 6:
            bomberFileName = bombfolder + "cat.png";
            vertical = RIGHT;
            horizontal = BOTTOM;
            break;
        case 7:
            bomberFileName = bombfolder + "pair.png";
            vertical = CENTER;
            horizontal = BOTTOM;
            break;
        case 8:
            bomberFileName = bombfolder + "seagull.png";
            vertical = LEFT;
            horizontal = TOP;
            break;

        default:
            bomberFileName = bombfolder + "crow.png";
            vertical = RIGHT;
            horizontal = BOTTOM;
            break;
    }

    reader.setFileName(bomberFileName);
    QImage bombimage = reader.read();

    // Read image current size.
    QSize bomberImageSize = bombimage.size();

    int vertical_pos;
    int horizontal_pos;

    // Vertical position
    if (vertical == LEFT) {
        vertical_pos = 0;
    } else if (vertical == CENTER) {
        vertical_pos = imageSize.width() / 2 - bomberImageSize.width() / 2;
    } else if (vertical == RIGHT) {
        vertical_pos = imageSize.width() - bomberImageSize.width();
    }

    // Horizontal position
    if (horizontal == TOP) {
        horizontal_pos = 0;
    } else if (horizontal == CENTER) {
        horizontal_pos = imageSize.height() / 2 - bomberImageSize.height() / 2;
    } else if (horizontal == BOTTOM) {
        horizontal_pos = imageSize.height() - bomberImageSize.height();
    }

    // Add the image and save the composition.
    merger.drawImage(vertical_pos, horizontal_pos, bombimage);
    image.save(fileName, "JPG");

    // Show the photo by using this function that take use of the InvokeManager.
    showPhotoInCard(fileName);
}

QImage PhotoBomberApp::getRotateImage(const QString imageFilePath)
{
    //Load the image using QImage.
    QImage image = QImage(imageFilePath);

    ExifData *exifData = 0;
    ExifEntry *exifEntry = 0;
    int exifOrientation = 1;

    // Since the image will loose its exif data when its opened in a QImage
    // it has to be manually rotated according to the exif orientation.
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

    qDebug() << "Exif data:" << exifOrientation;


    // It's a bit tricky to get the correct orientation of the image. A combination of
    // the way the the device is oriented and what the actual exif data says has to be used
    // in order to rotate it in the correct way.
    switch(exifOrientation) {
        case 1:
            // 0 degree rotation
            break;
        case 3:
            // 180 degree rotation
            transform.rotate(180);
            break;
        case 6:
            // 90 degree rotation
            transform.rotate(90);
            break;
        case 8:
            // 270 degree rotation
            transform.rotate(270);
            break;
        default:
            // Other orientations are mirrored orientations, do nothing.
            break;
    }

    // Perform the rotation of the image before its saved.
    image = image.transformed(transform);

    return image;
}
