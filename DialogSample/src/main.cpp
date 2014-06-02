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
#include "applicationui.hpp"

#include <bb/cascades/Application>

#include "AlertDialog.hpp"
#include "ContextMenuDialog.hpp"
#include "PromptDialog.hpp"
#include "LoginDialog.hpp"

using namespace bb::cascades;

int main(int argc, char** argv) {
	qmlRegisterType<AlertDialog>("Dialog.Alert", 1, 0, "AlertDialog");
	qmlRegisterType<ContextMenuDialog>("Dialog.ContextMenu", 1, 0,
			"ContextMenu");
	qmlRegisterType<PromptDialog>("Dialog.Prompt", 1, 0, "PromptDialog");
	qmlRegisterType<LoginDialog>("Dialog.Login", 1, 0, "LoginDialog");

	Application app(argc, argv);


	// Create the Application UI object, this is where the main.qml file
	// is loaded and the application scene is set.
	ApplicationUI appui;

	return Application::exec();
}

