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

#include <bb/platform/bbm/ProfileBoxItem>
#include "profilebox.h"
#include "global.h"

namespace bucketbbm
{
    ProfileBox::ProfileBox(QObject *parent) :
            QObject(parent), mProfileBox(0)
    {
    }

    ProfileBox::~ProfileBox()
    {
        delete mProfileBox;
    }

    void ProfileBox::registerIcons()
    {
        // The ProfileBox items use the same icons as the application menu.
        // Images are located in the assets/images directory.
        // finished.png, chickened.png and todo.png are used.
        QString imageDir(QDir::currentPath() + "/app/native/assets/images/");

        // Register the application icons and assign a unique id to it.
        registerIcon(imageDir + "finished.png", 1);
        registerIcon(imageDir + "chickened.png", 2);
        registerIcon(imageDir + "todo.png", 3);
    }

    void ProfileBox::registerIcon(const QString& path, int iconId)
    {
        // Register an icon with the BBBM SP.
        // Read the icon into a buffer from the file 
        // to register it with the BBM SP.
        QImage image;
        QByteArray iconArray;
        QBuffer buffer(&iconArray);
        buffer.open(QIODevice::WriteOnly);

        if (not image.load(path)) {
            qDebug() << ": Failed to load icon";
            return;
        }

        // Save the image to a buffer, so that it can be passed 
        // to the requestRegisterIcon function.
        image.save(&buffer, "PNG");

        // Create the icon object and register the icon
        if (mProfileBox->requestRegisterIcon(iconId, bb::platform::bbm::ImageType::Png, iconArray)
                == false) {
            qDebug() << "Failed to register icons ";
        }
    }

    void ProfileBox::createItem(const QString& text, const QString& iconPath)
    {
        // Creates a new item in the application's ProfileBox.
        if (mProfileBox == 0) {
            // Get an instance of bb::platform::bbm::ProfileBox
            mProfileBox = new bb::platform::bbm::ProfileBox(Global::instance()->getContext(), this);

            // Register the icons we plan to use in the ProfileBox items.
            registerIcons();
        }

        // Icon was selected. Determine its ID
        int iconId;
        if (iconPath == "images/finished.png") {
            iconId = 1;
        } else if (iconPath == "images/chickened.png") {
            iconId = 2;
        } else if (iconPath == "images/todo.png") {
            iconId = 3;
        } else {
            qDebug() << ": Item could not be added because icon ID could not"
                    " be determined; iconPath=" << iconPath;
            return;
        }

        // Add the ProfileBox item.
        if (mProfileBox->requestAddItem(text, iconId, QString("cookie")) == false) {
            qDebug() << "Failed to add a profile box.";
        }
    }
}
