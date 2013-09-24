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
#include "BarcodeDecoder.hpp"
#include "BarcodeScannerApp.hpp"

#include <bb/cascades/AbstractPane>
#include <bb/cascades/Application>
#include <bb/cascades/QmlDocument>

#include <QLocale>
#include <QTranslator>

using namespace bb::cascades;

Q_DECL_EXPORT int main(int argc, char **argv)
{
    // We need to register the QML types in the multimedia-library,
    // otherwise we will get an error from the QML.
    qmlRegisterType<bb::community::barcode::BarcodeDecoderControl>(
            "bb.community.barcode", 1, 0, "BarcodeDecoder");

    //-- this is where the server is started etc
    Application app(argc, argv);

    //-- localization support
    QTranslator translator;
    const QString filename = QString("barcodescanner_%1").arg(QLocale().name());
    if (translator.load(filename, "app/native/qm")) {
        app.installTranslator(&translator);
    }

    BarcodeScannerApp* barcodeScanner = new BarcodeScannerApp(&app);

    QmlDocument *qml = QmlDocument::create("asset:///main.qml");

    // expose BarcodeScannerApp object in QML as an variable
    qml->setContextProperty("_barcodeScanner", barcodeScanner);
    AbstractPane *root = qml->createRootObject<AbstractPane>();

    Application::instance()->setScene(root);

    return Application::exec();
}
