/* Copyright (c) 2012, 2013  BlackBerry Limited.
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

#include "app.hpp"

#include <bb/cascades/Application>

using ::bb::cascades::Application;
// this allows us to write "Application"
// instead of "bb::cascades::Application"

// main() is the entry point of the application. It will be called by the
// operating system when you start the application. You should never call this
// yourself.
Q_DECL_EXPORT int main(int argc, char **argv)
{
    // "Application" is the BB cascades class that handles interaction the
    // with BB10 operating system.
    Application app(argc, argv);

    // Register this type so qml can refer to enums and other symbols
    // declared in the App class.
    qmlRegisterType<App>("PersistentObjectsLib", 1, 0, "App");

    // Create an instance of App on the stack. App's
    // constructor registers itself with Application object using setScene().
    // See app.cpp
    App mainApp;

    // Start the application event loop (run-loop).
    return Application::exec();

    // When the loop is exited the Application deletes the scene which deletes
    // all its children (per Qt rules for children)
}
