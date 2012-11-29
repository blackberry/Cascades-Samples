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

#ifndef PROFILEMESSAGE_H_
#define PROFILEMESSAGE_H_

#include <bb/platform/bbm/UserProfile>
#include <QtCore/QObject>

namespace bucketbbm
{
    /**
     * ProfileMessage Description:
     * 
     * The profile message connects to the device user in the 
     * BBM Social Platform and can update the status message.
     */
    class ProfileMessage: public QObject
    {
    Q_OBJECT

    public slots:
        /**
         * Sets the users personal message.
         *
         * @param personalMessageString the new personal message.
         */
        void setPersonalMessage(const QString &personalMessageString);

    public:
        ProfileMessage(QObject *parent = 0);
        ~ProfileMessage();

    private:
        //
        bb::platform::bbm::UserProfile * mUserProfile;

    };
}
#endif /* PROFILEMESSAGE_H_ */
