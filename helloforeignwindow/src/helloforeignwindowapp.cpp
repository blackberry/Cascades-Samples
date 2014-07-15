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
#include "helloforeignwindowapp.h"

#include <bb/cascades/AbsoluteLayoutProperties>
#include <bb/cascades/ForeignWindowControl>
#include <bb/cascades/Page>
#include <bb/cascades/QmlDocument>
#include <bb/cascades/Window>

#include <math.h>

using namespace bb::cascades;

HelloForeignWindowApp::HelloForeignWindowApp()
{
    mTvOn = false;
    mTvInitialized = false;

    // Create a QML document and load the main UI QML file, using build patterns.
    QmlDocument *qml = QmlDocument::create("asset:///helloforeignwindow.qml").parent(this);

    if (!qml->hasErrors()) {

        // Set the context property we want to use from inside the QML document. Functions exposed
        // via Q_INVOKABLE will be found with this property and the name of the function.
        qml->setContextProperty("foreignWindowApp", this);

        // The application Page is created from QML.
        mAppPage = qml->createRootObject<Page>();

        if (mAppPage) {

            Application::instance()->setScene(mAppPage);

            // Initialize the foreign window.
            initForeignWindow();

            // Start the thread in which we render to the custom window.
            start();
        }
    }
}

HelloForeignWindowApp::~HelloForeignWindowApp()
{
    // Cleanup screen context.
    screen_destroy_context(mScreenCtx);

    // Stop the thread.
    terminate();
    wait();
}

void HelloForeignWindowApp::run()
{
    while (true) {

        // A short sleep in between renders
        usleep(25000);

        if (mTvOn && mTvInitialized) {
            // If the TV is on, render noise.
            doNoise(true);
        }
    }
}

void HelloForeignWindowApp::doNoise(bool noise)
{
    unsigned char *ptr = NULL;
    int col = 0;

    // Setup the noise blit parameters are set up to render noise over the entire child window.
    int noiseBlitParameters[] = { SCREEN_BLIT_SOURCE_WIDTH, mSize[0], SCREEN_BLIT_SOURCE_HEIGHT,
            mSize[1], SCREEN_BLIT_END };

    // Get the pixmap buffer and fill it with noise (if noise is false; fill it with black).
    screen_get_buffer_property_pv(mScreenPixelBuffer, SCREEN_PROPERTY_POINTER, (void **) &ptr);

    for (int i = 0; i < mSize[1]; i++, ptr += mStride) {
        for (int j = 0; j < mSize[0]; j++) {

            // Random noise color
            if (noise) {
                col = rand() % 255;
            }
            ptr[j * 4] = col;
            ptr[j * 4 + 1] = col;
            ptr[j * 4 + 2] = col;
            ptr[j * 4 + 3] = 0xff;
        }

    }

    // Get the window buffer, blit the pixels to the buffer and post the window update.
    screen_get_window_property_pv(mScreenWindow, SCREEN_PROPERTY_RENDER_BUFFERS,
            (void **) mScreenBuf);
    screen_blit(mScreenCtx, mScreenBuf[0], mScreenPixelBuffer, noiseBlitParameters);
    screen_post_window(mScreenWindow, mScreenBuf[0], 1, mRect, 0);
}

bool HelloForeignWindowApp::createForeignWindow(const QString &group, const QString id, int x,
        int y, int width, int height)
{
    QByteArray groupArr = group.toAscii();
    QByteArray idArr = id.toAscii();

    // Window source rectangle
    mRect[0] = 0;
    mRect[1] = 0;
    mRect[2] = width;
    mRect[3] = height;

    // We must create a context before you create a window.
    if (screen_create_context(&mScreenCtx, SCREEN_APPLICATION_CONTEXT) != 0) {
        return false;
    }

    // Create a child window of the current window group, join the window group and set
    // a window ID.
    if (screen_create_window_type(&mScreenWindow, mScreenCtx, SCREEN_CHILD_WINDOW) != 0) {
        return false;
    }
    if (screen_join_window_group(mScreenWindow, groupArr.constData()) != 0) {
        return false;
    }
    if (screen_set_window_property_cv(mScreenWindow, SCREEN_PROPERTY_ID_STRING, idArr.length(),
            idArr.constData()) != 0) {
        return false;
    }

    // In this application, we will render to a pixmap buffer and then blit that to
    // the window, and then finally we set the usage to native.
    // (default is read and write but we do not need that here)
    int usage = SCREEN_USAGE_NATIVE;
    if (screen_set_window_property_iv(mScreenWindow, SCREEN_PROPERTY_USAGE, &usage) != 0) {
        return false;
    }

    // The window size is specified in QML so we need to set up the buffer size to
    // correspond to that, the default size would be the full screen.
    if (screen_set_window_property_iv(mScreenWindow, SCREEN_PROPERTY_BUFFER_SIZE, mRect + 2) != 0) {
        return false;
    }
    if (screen_set_window_property_iv(mScreenWindow, SCREEN_PROPERTY_SOURCE_SIZE, mRect + 2) != 0) {
        return false;
    }

    // Use negative Z order so that the window appears under the main window.
    // (required by the ForeignWindow functionality)
    int z = -5;
    if (screen_set_window_property_iv(mScreenWindow, SCREEN_PROPERTY_ZORDER, &z) != 0) {
        return false;
    }

    // Set the window position on screen.
    int pos[2] = { x, y };
    if (screen_set_window_property_iv(mScreenWindow, SCREEN_PROPERTY_POSITION, pos) != 0) {
        return false;
    }

    // Create the window buffers, in this application we will only use one buffer.
    if (screen_create_window_buffers(mScreenWindow, 1) != 0) {
        return false;
    }

    // Create a pixmap to be used as off-screen rendering target. We can use this pixmap
    // to render other pixmaps, allowing us to have full control of exactly which pixels
    // we choose to push to the screen.
    screen_pixmap_t screen_pix;
    if (screen_create_pixmap(&screen_pix, mScreenCtx) != 0) {
        return false;
    }

    // A combination of write and native usage is necessary to blit the pixmap to screen.
    usage = SCREEN_USAGE_WRITE | SCREEN_USAGE_NATIVE;
    if (screen_set_pixmap_property_iv(screen_pix, SCREEN_PROPERTY_USAGE, &usage) != 0) {
        return false;
    }

    // Set the width and height of the buffer to correspond to the one we specified in the QML document.
    mSize[0] = width;
    mSize[1] = height;
    if (screen_set_pixmap_property_iv(screen_pix, SCREEN_PROPERTY_BUFFER_SIZE, mSize) != 0) {
        return false;
    }

    // Create the pixmap buffer and get a reference to it for rendering in the doNoise function.
    if (screen_create_pixmap_buffer(screen_pix) != 0) {
        return false;
    }
    if (screen_get_pixmap_property_pv(screen_pix, SCREEN_PROPERTY_RENDER_BUFFERS,
            (void **) &mScreenPixelBuffer) != 0) {
        return false;
    }

    // Obtain the stride (the number of bytes between pixels on different rows), for use
    // later on when we perform the rendering to the pixmap buffer.
    if (screen_get_buffer_property_iv(mScreenPixelBuffer, SCREEN_PROPERTY_STRIDE, &mStride) != 0) {
        return false;
    }

    return true;
}

void HelloForeignWindowApp::initForeignWindow()
{

    // Get the foreign window Control specified in the QML document and attach to the window-attached signal.
    ForeignWindowControl *foreignWindow = mAppPage->findChild<ForeignWindowControl*>(
            "myForeignWindow");

    AbsoluteLayoutProperties *layoutProperties =
            dynamic_cast<AbsoluteLayoutProperties*>(foreignWindow->layoutProperties());

    // Set up the foreign window at the position specified by its LayoutProperties and the dimensions
    // given by its preferred width and height.
    if (createForeignWindow(Application::instance()->mainWindow()->groupId(),
            "HelloForeignWindowAppID", (int) layoutProperties->positionX(),
            (int) layoutProperties->positionY(), (int) foreignWindow->preferredWidth(),
            (int) foreignWindow->preferredHeight())) {
        // At this point, initialization of the window has been performed, so set the flag to true.
        mTvInitialized = true;
    } else {
        qDebug() << "The ForeginWindow was not properly initialized";
    }
}

void HelloForeignWindowApp::tvPower(bool on)
{
    mTvOn = on;

    // If the TV is off, render one frame of a black screen (noise false).
    if (mTvOn == false) {
        doNoise(false);
    }
}
