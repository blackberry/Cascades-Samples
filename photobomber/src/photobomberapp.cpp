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

#include "photobomberapp.h"

#include <QImage>
#include <QImageReader>

#include <bb/system/InvokeManager.hpp>
#include <bb/system/InvokeRequest.hpp>

#include <bb/cascades/Page>
#include <bb/cascades/QmlDocument>

#include <bb/cascades/multimedia/Camera>

#include <bps/soundplayer.h>

using namespace bb::cascades;
using namespace bb::cascades::multimedia;
using namespace bb::system;

PhotoBomberApp::PhotoBomberApp()
{
    // We need to register the QML types in the multimedia-library,
    // otherwise we will get an error from the QML.
    qmlRegisterType<Camera>("bb.cascades.multimedia",1,0,"Camera");


    // Create a QMLDocument and load it, using build patterns
    QmlDocument *qml = QmlDocument::create("asset:///main.qml");

    qml->setContextProperty("photoBomber", this);

    if (!qml->hasErrors()) {
        // The application Page is created from QML.
        Page *appPage = qml->createRootObject<Page>();

        if (appPage) {

			      // Set the application scene and connect the camera's shutterFired signal to our slot function
            Application::instance()->setScene(appPage);

            Camera *camera = appPage->findChild<Camera*>("myCamera");
            QObject::connect(camera, SIGNAL(shutterFired()), this, SLOT(onShutterFired()));

            camera->open(CameraUnit::Front);

        }
    }
}

void PhotoBomberApp::onShutterFired()
{
    // A cool trick here to play a sound. There are legal requirements in many countries to have a shutter-sound when
    // taking pictures. So we need this shutter sound if you are planning to submit you're app to app world.
	// So we play the shutter-fire sound when the onShutterFired event occurs.
    soundplayer_play_sound("event_camera_shutter");
}

void PhotoBomberApp::showInPicturesApp(QString fileName)
{
    // Here we create a invoke request to the pictures app.
    // We could also ask the system what other applications can
    // receive something of our mimeType. But the pictures app will come pre-installed so it's a safe bet.

    InvokeRequest invokeRequest;
    invokeRequest.setAction("bb.action.OPEN");
    invokeRequest.setTarget("sys.pictures.app");
    invokeRequest.setMimeType("images/jpeg");
    invokeRequest.setUri(QString("%1%2").arg("photos:").arg(fileName.remove(0,7)));

    InvokeManager invokeManager;
    invokeManager.invoke(invokeRequest);
}

void PhotoBomberApp::manipulatePhoto(const QString &fileName)
{
    QImageReader reader;

    // Set image name from the given file name.
    reader.setFileName(fileName);
    QImage image = reader.read();
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
    QString bombfolder = appFolder + "app/native/assets/images/bombers/";

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
}

