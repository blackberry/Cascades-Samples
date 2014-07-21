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

#ifndef _POEMMAKER_H
#define _POEMMAKER_H

#include <bb/cascades/Application>

using namespace bb::cascades;

/**
 * PoemMaker Description:
 *
 * The PoemMaker is a simple application with three lines of text (Labels)
 * and a custom button. Clicking the button will generate a random "refrigerator"
 * poem.
 *
 * UI components included in the code are containers, images, and labels that
 * are created and positioned in QML.
 *
 * It is a simple UI with three lines of text and a button. When the button is clicked
 * a random "refrigerator" poem is generated.
 *
 * Feature summary:
 * - Adding and positioning basic UI components (Label/Image/Container)
 * - Attaching event listener in order to react on user interaction
 * - Running animations
 */
class PoemMaker: public QObject
{
    Q_OBJECT

public:
    // This is our constructor that sets up the recipe.
    PoemMaker(QObject *parent = 0);
};

#endif // ifndef _POEMMAKER_H
