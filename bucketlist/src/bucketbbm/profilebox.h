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

#ifndef PROFILEBOX_HPP_
#define PROFILEBOX_HPP_

#include <bb/platform/bbm/ProfileBox>

#include <QtCore/QObject>
#include <QtCore/QMetaType>

namespace bucketbbm
{
    /**
     * ProfileBox Description:
     *
     * This class sets up and handles the applications profile box. A profile box for the application appears
     * in the current user's BBM profile. Each box contains a list of items, each composed of text and an optional icon.
     * There is a maximum of 3 items; as more are added, older items will be removed automatically.
     * (see bb/platform/bbm/ProfileBox for further details)
     */
    class ProfileBox: public QObject
    {
    Q_OBJECT

    public slots:
        /**
         * Creates a new item in the application's ProfileBox.
         * 
         * @param text Text used for the profile box
         * @param iconPath The path to where the box icons are
         */
        void createItem(const QString& text, const QString& iconPath);

    public:
        ProfileBox(QObject *parent = 0);
        ~ProfileBox();

    private:
        /**
         * Registers all of the icons.
         * The ProfileBox items use the same icons as the application menu.
         */
        void registerIcons();

        /**
         * Registers the icon at \a path as \a iconId.
         *
         * @param path Path to the icon image
         * @param iconId A unique icon id
         */
        void registerIcon(const QString& path, int iconId);

        // The platform profile box for the application.
        bb::platform::bbm::ProfileBox* mProfileBox;
    };
}

#endif /* PROFILEBOX_HPP_ */
