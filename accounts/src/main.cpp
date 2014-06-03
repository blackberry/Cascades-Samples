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

#include "AccountEditor.hpp"
#include "AccountViewer.hpp"
#include "applicationui.hpp"

#include <bb/cascades/Application>

using namespace bb::cascades;

Q_DECL_EXPORT int main(int argc, char **argv)
{
//! [0]
    // Register our custom types with QML, so that they can be used as property types
    qmlRegisterUncreatableType<AccountEditor>("com.example.bb10samples.pim.accounts", 1, 0, "AccountEditor", "Usage as property type and access to enums");
    qmlRegisterType<AccountViewer>();
//! [0]

    Application app(argc, argv);

//! [1]
    // Create the Application UI object, this is where the main.qml file
    // is loaded and the application scene is set.
    ApplicationUI appui;
//! [1]

    return Application::exec();
}
