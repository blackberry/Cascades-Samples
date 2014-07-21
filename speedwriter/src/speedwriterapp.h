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

#ifndef SPEEDWRITERAPP_H
#define SPEEDWRITERAPP_H

#include <bb/cascades/Application>

/**
 * SpeedWriterApp Description:
 *
 * This is a challenging game for testing your typing skills on the virtual keyboard.
 * Consequently, one of the things you learn with this sample is how to handle text input.
 * The speed is measured using a gauge, which is implemented as a Custom Control.
 *
 * You will learn how to:
 * - Create a Custom Control in C++ and add it to a QML document.
 * - Create a Custom Object for word checking and add it to QML.
 * - Monitor changes in a TextField and update a TextArea.
 */
class SpeedWriterApp: public QObject
{
    Q_OBJECT

public:
    // This is our constructor that sets up the recipe.
    SpeedWriterApp(QObject *parent = 0);
    ~SpeedWriterApp();
};

#endif // ifndef SPEEDWRITERAPP_H
