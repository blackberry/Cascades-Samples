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

#include "bucketbbmmanager.h"
#include <QDebug>

namespace bucketbbm
{
    BucketBBMManager::BucketBBMManager(QObject *parent) :
            QObject(parent), mRegistrationHandler(0), mProfileBox(0), mProfileMessage(0)
    {

    }

    BucketBBMManager::~BucketBBMManager()
    {
    }

    void BucketBBMManager::registerApp()
    {
        if (mRegistrationHandler == 0) {
            // If no registration handler exist when trying register, create one
            // and connect to its signals, so that updates can be relayed via this
            // objects properties.
            mRegistrationHandler = new RegistrationHandler(this);

            connect(mRegistrationHandler, SIGNAL(statusTextChanged(QString)), SLOT(onStatusTextChanged(QString)));
            connect(mRegistrationHandler, SIGNAL(registeredChanged(bool)), SLOT(onRegisteredChanged(bool)));
        }

        mRegistrationHandler->registerApp();
    }

    void BucketBBMManager::onRegisteredChanged(bool registered)
    {
        emit appRegisteredChanged(registered);
    }

    void BucketBBMManager::onStatusTextChanged(QString statusText)
    {
        emit registerStatusTextChanged(statusText);
    }

    void BucketBBMManager::updateStatus(const QString message, const QString icon)
    {
        if (mRegistrationHandler && mRegistrationHandler->registered()) {
            if (mProfileBox == 0) {
                // Create a profile box for the bucket list. This adds an
                // specific item box for this application where messages can be seen.
                mProfileBox = new ProfileBox(this);
            }
            if (mProfileMessage == 0) {
                // Create a message for the user in order to update its status.
                mProfileMessage = new ProfileMessage(this);
            }

            mProfileBox->createItem(message, icon);
            mProfileMessage->setPersonalMessage(message);
        }

    }

    QString BucketBBMManager::registerStatusText()
    {
        if (mRegistrationHandler == 0) {
            // If a registration handler exists return its status.
            return mRegistrationHandler->statusText();
        }

        // If registration has not been attempted return an empty string.
        return "";
    }

    bool BucketBBMManager::appRegistered()
    {
        if (mRegistrationHandler == 0) {
            // If a registration handler exists return its status.
            mRegistrationHandler->registered();
        }

        // If registration has not been attempted return false.
        return false;
    }
}
