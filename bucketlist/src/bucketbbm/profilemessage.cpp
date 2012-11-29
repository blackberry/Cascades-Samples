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

#include "profilemessage.h"
#include "global.h"

namespace bucketbbm
{
    ProfileMessage::ProfileMessage(QObject *parent) :
            QObject(parent), mUserProfile(0)
    {
    }

    ProfileMessage::~ProfileMessage()
    {
        delete mUserProfile;
    }

    void ProfileMessage::setPersonalMessage(const QString &personalMessageString)
    {

        if (mUserProfile == 0) {
            // Grab the user's Profile.
            mUserProfile = new bb::platform::bbm::UserProfile(Global::instance()->getContext(),
                    this);
        }

        //Update the user's personal message.
        mUserProfile->requestUpdatePersonalMessage(personalMessageString);
    }
}
