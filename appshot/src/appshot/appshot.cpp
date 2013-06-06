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
#include "appshot.h"

#include <QtGui/QImage>
#include <bb/cascades/Application>
#include <bb/cascades/Window>

const char* const AppShot::mDefaultFolder = "/shared/camera/";

using namespace bb::cascades;

AppShot::AppShot(QObject *parent) : QObject(parent)
{
    mSaveFolder = mDefaultFolder;
    mScreenSize = new ScreenSize(this);
}

AppShot::~AppShot()
{
}

QString AppShot::captureScreen(const QString fileName)
{
    Window *cascadesWindow = Application::instance()->mainWindow();

    // The pixel buffer
    screen_buffer_t screen_shot_buffer;

    // The pixel buffer size
    int size[2];

    // Variables for setting up taking a screenshot.
    screen_pixmap_t screen_pix;

    //screen_buffer_t screenshot_buf;
    screen_context_t screenshot_ctx;

    // Create a context for set up of pixmap
    screen_create_context(&screenshot_ctx, SCREEN_APPLICATION_CONTEXT);

    char *screenshot_ptr = NULL;
    int screenshot_stride = 0;
    int usage, format;

    // Create pixmap.
    screen_create_pixmap(&screen_pix, screenshot_ctx);

    // Set Usage Flags.
    usage = SCREEN_USAGE_READ | SCREEN_USAGE_NATIVE;
    screen_set_pixmap_property_iv(screen_pix, SCREEN_PROPERTY_USAGE, &usage);

    // Set format.
    format = SCREEN_FORMAT_RGBA8888;
    screen_set_pixmap_property_iv(screen_pix, SCREEN_PROPERTY_FORMAT, &format);

    // Set pixmap buffer size, get the current width and height of the screen
    size[0] = mScreenSize->width();
    size[1] = mScreenSize->height();
    screen_set_pixmap_property_iv(screen_pix, SCREEN_PROPERTY_BUFFER_SIZE, size);

    // Create pixmap buffer and get handle to the buffer.
    screen_create_pixmap_buffer(screen_pix);
    screen_get_pixmap_property_pv(screen_pix, SCREEN_PROPERTY_RENDER_BUFFERS,
            (void**) &screen_shot_buffer);

    // Get a pointer to the buffer.
    screen_get_buffer_property_pv(screen_shot_buffer, SCREEN_PROPERTY_POINTER,
            (void**) &screenshot_ptr);

    // Get the stride.
    screen_get_buffer_property_iv(screen_shot_buffer, SCREEN_PROPERTY_STRIDE, &screenshot_stride);

    // Get the screen shot of the cascades application window.
    screen_read_window(cascadesWindow->handle(), screen_shot_buffer, 0, NULL, 0);

    unsigned char *ptr = NULL;
    screen_get_buffer_property_pv(screen_shot_buffer, SCREEN_PROPERTY_POINTER, (void **) &ptr);

    // Retrieve the path to the app's working directory
    QString workingDir = QDir::currentPath();

    // The full file path for saving the image
    QString filePath = workingDir + mSaveFolder + fileName;

    // Save the buffer to file
    if(!saveImageBufferToFile(filePath, mScreenSize->res(), ptr, screenshot_stride)){
        qDebug() << "AppShot: Failed to save file.";
    }

    // Destroy the pixmap and context
    screen_destroy_pixmap(screen_pix);
    screen_destroy_context(screenshot_ctx);

    return filePath;
}

void AppShot::setSaveFolder(const QString saveFolder)
{
    if(mSaveFolder != saveFolder){
        mSaveFolder = saveFolder;
        emit saveFolderChanged(mSaveFolder);

        QString workingDir = QDir::currentPath();
        QDir dir(workingDir + mSaveFolder);

        // If the folder doesn't exist create it.
        if (!dir.exists()) {
            if(!dir.mkpath(".")){
                qWarning() << "AppShot: Failed to create save directory, screen shots might not be saved.";
            }
        }
    }
}

QString AppShot::saveFolder()
{
    return mSaveFolder;
}

bool AppShot::saveImageBufferToFile(QString filePath,  QSize imageSize, unsigned char* ptr, int stride)
{
	QImage image = QImage( imageSize, QImage::Format_RGB32 );

	// Copy the pixels into the QImage for easy saving.
	for (int i = 0; i < imageSize.height(); ++i){
		memcpy(image.scanLine(i), ptr + i * stride, stride);
	}

    // Save the image to the specified save folder, here we use jpeg encoding
    // but QImage can save to a lot of other formats if needed.
    return image.save(filePath, "JPG");
}

