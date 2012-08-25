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

#include <bb/cascades/Application>
#include <bb/cascades/QmlDocument>
#include <bb/cascades/AbstractPane>

#include "app.hpp"
#include "AlertDialog.hpp"
#include "ContextMenuDialog.hpp"
#include "PromptDialog.hpp"
#include "LoginDialog.hpp"
#include "FileBrowseDialog.hpp"
#include "FileSaveDialog.hpp"

using namespace bb::cascades;

App::App()
{
    qmlRegisterType<AlertDialog>("Dialog.Alert", 1, 0, "AlertDialog");
    qmlRegisterType<ContextMenuDialog>("Dialog.ContextMenu", 1, 0, "ContextMenu");
    qmlRegisterType<PromptDialog>("Dialog.Prompt", 1, 0, "PromptDialog");
    qmlRegisterType<LoginDialog>("Dialog.Login", 1, 0, "LoginDialog");
    qmlRegisterType<FileBrowseDialog>("Dialog.FileBrowse", 1, 0, "FileBrowseDialog");
    qmlRegisterType<FileSaveDialog>("Dialog.FileSave", 1, 0, "FileSaveDialog");

    QmlDocument* qml = QmlDocument::create("main.qml");
    qml->setContextProperty("cs", this);

    AbstractPane* root = qml->createRootNode<AbstractPane>();
    Application::setScene(root);
}
