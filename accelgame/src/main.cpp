/*
 * Copyright (c) 2011, 2012, 2013  BlackBerry Limited.
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

#include <bb/cascades/Application>

#include "gamecontroller.hpp"

using namespace bb::cascades;

/**
 * This sample application shows how to use the QAccelerometer class from the
 * QtSensors module to navigate a player inside a maze. Depending on whether the
 * device is tilted to the left/right/up/down, the player is moved that direction.
 */
Q_DECL_EXPORT int main(int argc, char **argv)
{
    Application app(argc, argv);

    // Create the game controller object
    GameController gameController;

    return Application::exec();
}
