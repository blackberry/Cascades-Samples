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
#include "BarcodeDecoder.hpp"
#include "BarcodeScannerApp.hpp"

#include <bb/cascades/AbstractPane>
#include <bb/cascades/Application>
#include <bb/cascades/QmlDocument>

#include <QLocale>
#include <QTranslator>

using namespace bb::cascades;

int main(int argc, char **argv)
{
    // We need to register the QML types in the multimedia-library,
    // otherwise we will get an error from the QML.
    qmlRegisterType<bb::community::barcode::BarcodeDecoderControl>(
            "bb.community.barcode", 1, 0, "BarcodeDecoder");

    //-- this is where the server is started etc
    Application app(argc, argv);

    //-- localization support
    QTranslator translator;
    QString locale_string = QLocale().name();
    QString filename = QString("BarcodeScanne_%1").arg(locale_string);
    if (translator.load(filename, "app/native/qm")) {
        app.installTranslator(&translator);
    }

    QmlDocument *qml = QmlDocument::create("asset:///main.qml");
    //QmlDocument
    //-- setContextProperty expose C++ object in QML as an variable
    BarcodeScannerApp* bscanner = new BarcodeScannerApp(&app);
    qml->setContextProperty("barcodeScanner", bscanner);

    AbstractPane *root = qml->createRootObject<AbstractPane>();

    Application::instance()->setScene(root);

    //connect start/stop scanning signals to when invoked as a card
    bb::community::barcode::BarcodeDecoderControl* bdcontrol = root->findChild<
            bb::community::barcode::BarcodeDecoderControl*>("barcodeDecoder");
    QObject::connect(bscanner, SIGNAL(startScan()), bdcontrol,
            SLOT(startScanning()));
    QObject::connect(bscanner, SIGNAL(stopScan()), bdcontrol,
            SLOT(stopScanning()));
    QObject::connect(bdcontrol, SIGNAL(newBarcodeDetected(QString)), bscanner,
            SLOT(newBarcodeDetected(QString)));

    //-- we complete the transaction started in the app constructor and start the client event loop here
    return Application::exec();
    //-- when loop is exited the Application deletes the scene which deletes all its children (per qt rules for children)
}
