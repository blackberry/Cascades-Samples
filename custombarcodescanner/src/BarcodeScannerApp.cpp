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
#include "BarcodeScannerApp.hpp"

#include <bb/cascades/Application>
#include <bb/multimedia/MediaPlayer.hpp>
#include <bb/system/ApplicationStartupMode>
#include <bb/system/CardDoneMessage.hpp>
#include <bb/system/InvokeManager>
#include <bb/system/InvokeRequest>

#include <zxing/MultiFormatReader.h>
#include <zxing/common/GreyscaleLuminanceSource.h>
#include <zxing/common/HybridBinarizer.h>

#include <exception>

using namespace bb::multimedia;
using namespace bb::system;

//! [0]
BarcodeScannerApp::BarcodeScannerApp(QObject* parent)
    : QObject(parent)
    , m_invokeManager(new InvokeManager(this))
    , m_invoked(false)
    , m_pooled(false)
    , m_player(new MediaPlayer(this))
{
    m_player->setSourceUrl(QDir::currentPath() + "/app/native/assets/sounds/boopdoop.mp3");

    bool ok = connect(m_invokeManager, SIGNAL(invoked(const bb::system::InvokeRequest&)),
                      this, SLOT(onInvoked(const bb::system::InvokeRequest&)));
    Q_ASSERT(ok);

    ok = connect(m_invokeManager, SIGNAL(cardPooled(const bb::system::CardDoneMessage&)),
                 this, SLOT(onCardPooled(const bb::system::CardDoneMessage&)));
    Q_ASSERT(ok);

    switch (m_invokeManager->startupMode()) {
        case ApplicationStartupMode::LaunchApplication:
            m_invoked = false;
            break;
        case ApplicationStartupMode::InvokeApplication:
        case ApplicationStartupMode::InvokeCard:
            m_invoked = true;
            break;
        default:
            break;
    }

    qDebug() << "+++++++++ Application invoked: " << m_invoked << endl;
}
//! [0]

//! [1]
void BarcodeScannerApp::newBarcodeDetected(const QString &barcode)
{
    m_player->play();

    if (m_invoked) {
        CardDoneMessage message;
        message.setData(barcode);
        message.setDataType("text/plain");
        message.setReason("Barcode scanned!");
        m_invokeManager->sendCardDone(message);
    }
}

void BarcodeScannerApp::onCardPooled(const bb::system::CardDoneMessage&)
{
    m_pooled = true;
    Q_EMIT stopScan();
}

void BarcodeScannerApp::onInvoked(const bb::system::InvokeRequest&)
{
    if (m_pooled)
        Q_EMIT startScan();
}
//! [1]
