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

#ifndef _PULLMYBEARDAPP_H_
#define _PULLMYBEARDAPP_H_

#include <bb/cascades/Application>
#include "soundmanager.h"

/*
 * Pull my Bread Description:
 *
 * This sample application illustrates how to set up basic touch interaction.
 * We will demonstrate how to move an image when responding to touch events. And we
 * will also show how to animate the image back to position when touches end.
 *
 * By pressing your finger on the beard and pulling downwards you will move the beard,
 * releasing it will let the beard snap into place.
 *
 * Feature summary
 * - Moving an image by touch, simple drag and drop
 * - Animation triggering
 *
 */
class PullMyBeardApp: public QObject
{
    Q_OBJECT

public:
    // This is our constructor that sets up the recipe.
    PullMyBeardApp(QObject *parent = 0);
    ~PullMyBeardApp();

    /**
     * Invokable function used for triggering sounds from QML.
     *
     * @param msg A string describing which sound should be played
     */
    Q_INVOKABLE
    void playSound(const QString &msg);

private:
    // The sound manager
    SoundManager *mSoundManager;
};

#endif // ifndef _PULLMYBEARDAPP_H_
