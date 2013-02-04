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

#include "bucketsettings.h"

BucketSettings::BucketSettings(QObject *parent) :
        QObject(parent)
{
    // If no value has been saved, return the default value.
    if (mSettings.value("postToBBM").isNull()) {
        mPostToBBM = false;
    } else {
        // Otherwise, set the value stored in the settings object.
        mPostToBBM = mSettings.value("postToBBM").toBool();
    }

    if (mSettings.value("connectToBBM").isNull()) {
        mConnectToBBM = true;
    } else {
        // Otherwise, set the value stored in the settings object.
        mConnectToBBM = mSettings.value("connectToBBM").toBool();
    }

}

BucketSettings::~BucketSettings()
{

}

void BucketSettings::setPostToBBM(bool postToBBM)
{
    if (postToBBM != mPostToBBM) {
        // Update the application settings then update and emit the change.
        mSettings.setValue("postToBBM", QVariant(postToBBM));
        mPostToBBM = postToBBM;
        emit postToBBMChanged(mPostToBBM);
    }
}

bool BucketSettings::postToBBM()
{
    return mPostToBBM;
}

void BucketSettings::setConnectToBBM(bool connectToBBM)
{
    if (connectToBBM != mConnectToBBM) {
        // Update the application settings then update and emit the change.
        mSettings.setValue("connectToBBM", QVariant(connectToBBM));
        mConnectToBBM = connectToBBM;
        emit connectToBBMChanged(mConnectToBBM);
    }
}

bool BucketSettings::connectToBBM()
{
    return mConnectToBBM;
}

