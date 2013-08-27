/* Copyright (c) 2013 BlackBerry Limited.
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
#include "SocialInvocation.hpp"

#include <bb/cascades/AbstractPane>
#include <bb/cascades/Application>
#include <bb/cascades/QmlDocument>

#include <QLocale>
#include <QTranslator>

using namespace bb::cascades;

Q_DECL_EXPORT int main(int argc, char **argv)
{
    Application app(argc, argv);

    QTranslator translator;
    const QString filename = QString::fromLatin1("socialinvocation_%1" ).arg(QLocale().name());
    if (translator.load(filename, "app/native/qm"))
        app.installTranslator( &translator );

    //! [0]
    QmlDocument *qml = QmlDocument::create("asset:///main.qml").parent(&app);
    qml->setContextProperty("_socialInvocation", new SocialInvocation(&app));

    // Retrieve the path to the app's working directory
    const QString workDir = QDir::currentPath();

    // Build the path, add it as a context property,
    // and expose it to QML
    QVariantMap dirPaths;
    dirPaths["camera"] = QString::fromLatin1("file://%1/shared/camera/").arg(workDir);
    dirPaths["asset"] = QString::fromLatin1("file://%1/app/native/assets/").arg(workDir);
    qml->documentContext()->setContextProperty("_dirPaths", dirPaths);

    AbstractPane *root = qml->createRootObject<AbstractPane>();
    app.setScene(root);
    //! [0]

    return Application::exec();
}
