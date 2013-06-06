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

#ifndef _APPSHOT_H_
#define _APPSHOT_H_

#include <QObject>
#include <QSize>
#include <screen/screen.h>
#include "screensize.h"

/**
 * AppShot Description:
 *
 * This is a helper class for taking programmatic screenshots of a Cascades
 * application
 * What needs to be done:
 *
 * - Add libscreen and libbbdevice (for ScreenSize class) libraries by right
 *   clicking you project and selecting add Configure -> "Add Library"
 * - Give the app permission to write to the shared files folder, add
 *   <permission>access_shared</permission>, to the bar-descriptor.xml
 * - Register the AppShot type to use it in qml before creating your qml document
 *   ex: qmlRegisterType < AppShot > ("com.appshot", 1, 0, "ScreenShot");
 * - Register the AppShot type to use it in qml before creating your qml document
 *
 * C++
 *
 * #include "appshot/appshot.h"
 *
 *  ApplicationUI::ApplicationUI(bb::cascades::Application *app) :
 *         QObject(app)
 * {
 *     // Always register custom types before you create your qml document
 *     qmlRegisterType < AppShot > ("com.appshot", 1, 0, "AppShot");
 *
 *     // Create document, root control and set scene
 *     QmlDocument *qml = QmlDocument::create("asset:///main.qml").parent(this);
 *     AbstractPane *root = qml->createRootObject<AbstractPane>();
 *     app->setScene(root);
 * }
 *
 *
 * QML
 *
 * import bb.cascades 1.0
 * import com.appshot 1.0
 *
 * Page {
 *    Container {
 *        Button {
 *            text: "App Shot"
 *            onClicked: {
 *                appShot.captureScreen("test.jpg")
 *            }
 *
 *        }
 *    }
 *
 *    attachedObjects: AppShot {
 *        id: appShot
 *    }
 * }
 *
 *
 */
class AppShot : public QObject
{
    Q_OBJECT

    /**
     * The folder where screen shots are saved. Defaults to the applications shared
     * camera folder, in order for the screen shot to be saved the application needs
     * to have permission to write to this directory.
     */
    Q_PROPERTY(QString saveFolder READ saveFolder WRITE setSaveFolder NOTIFY saveFolderChanged)

public:
    AppShot(QObject *parent = 0);
    virtual ~AppShot();

    /**
     * Function that will perform a screen shot of the current cascades application
     * and store the image into the saveFolder.
     *
     * @param fileName - The name of the screen shot image file
     * @return the full path to the saved image
     */
    Q_INVOKABLE QString captureScreen(const QString fileName);

    /**
     * Sets the save folder where screen shots will be stored.
     *
     * @param saveFolder The name of the folder, the application needs to
     *        have permission to write to the folder
     */
    Q_SLOT void setSaveFolder(const QString saveFolder);

    /**
     * The folder in which screen shots are saved. The default is the applications
     * shared camera folder.
     *
     * @return The current save folder.
     */
    QString saveFolder();

    /**
     * Signal that is emitted when the folder in which screen shots are saved changes.
     */
    Q_SIGNAL void saveFolderChanged(QString saveFolder);

private:
    /**
     * Helper function for saving a pixel buffer to an image on the file system.
     *
     * @param filePath The full path including file name and prefix, currently only JPEG is supported
     * @param imageSize A QSize containing the application screen dimensions
     * @param ptr A pointer to the pixel buffer
     */
    bool saveImageBufferToFile(QString filePath,  QSize imageSize, unsigned char *ptr, int stride);

    // The default folder in which to save screen shots
    static const char* const mDefaultFolder;

    // Property variables
    QString mSaveFolder;
    ScreenSize *mScreenSize;
};

#endif /* _APPSHOT_H_ */
