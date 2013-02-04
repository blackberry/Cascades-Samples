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
#include "netimagetracker.h"
#include <bb/cascades/ImageTracker>

using namespace bb;
using namespace bb::cascades;

NetImageTracker::NetImageTracker(QObject *parent) :
        ImageTracker(parent)
{
    mIsCreated = false;

    connect(this, SIGNAL(creationCompleted()), this, SLOT(onCreationCompleted()));
}

void NetImageTracker::onCreationCompleted()
{
    mIsCreated = true;

    if (!mSource.isEmpty() && mManager) {
        // Once creation of the tracker has completed, the lookup of the image is ready to
        // be executed. If the tracker is used within a list item this lookup is not needed
        // since the update of list item data will enforce a  refresh of the imageSource that
        // will perform the lookup. But for the tracker to work in ImageViews that are not
        // part of a list item we need this here.
        mManager->lookUpImage(mSource);
    }
}

void NetImageTracker::onImageReady(const QString filePath, const QString imageName)
{
    if (imageName.compare(mSource) == 0) {

        // Set the path to the image that is now downloaded and cahced in the data folder on the device.
        QUrl url = QUrl(filePath);
        setImageSource(url);
    } else if (imageName.compare("loading") == 0) {

        // If we dont have an image to display, let's display a loading image
        QUrl url = QUrl("asset:///images/ca_rss_unread.png");
        setImageSource(url);
    }
}

void NetImageTracker::setSource(const QString source)
{
    if (!source.isEmpty() && mSource.compare(source) != 0) {
        mSource = source;

        if (mManager) {
            // If a manger has been set make a request to look up the image. Otherwise
            // the request is delayed to onCreationCompleted or at the next time a call
            // to set the source is made.
            mManager->lookUpImage(mSource);
        } else {
            qWarning()
                    << "This NetImageTracker does not have any NetImageManager, set up one as an attached object and add it to the property.";
        }

        emit sourceChanged(mSource);
    }
}

QString NetImageTracker::source()
{
    return mSource;
}

void NetImageTracker::setManager(NetImageManager *manager)
{
    if (mManager != manager) {

        // Change the manager that is used for the tracker.
        if (mManager) {
            disconnect(mManager, SIGNAL(imageReady(const QString , const QString )), this,
                    SLOT(onImageReady( const QString , const QString )));
            delete (mManager);
        }

        mManager = manager;
        emit managerChanged(mManager);

        connect(mManager, SIGNAL(imageReady(const QString , const QString )), this,
                SLOT(onImageReady( const QString , const QString )));
    }
}

NetImageManager *NetImageTracker::manager()
{
    return mManager;
}

