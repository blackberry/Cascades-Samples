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
#ifndef BARCODESCANNERAPP_HPP
#define BARCODESCANNERAPP_HPP

#include <QObject>

namespace bb {
namespace multimedia {
class MediaPlayer;
}

namespace system {
class InvokeManager;
class InvokeRequest;
class CardDoneMessage;
}
}

/**
 * This class deals with playing an audible sound upond barcode detection.
 * As well as with dealing with the different states of app launch (i.e. launched as a card)
 * such as being pooled and responding with data back to the parent that invoked it.
 */
class BarcodeScannerApp: public QObject
{
    Q_OBJECT

public:
    BarcodeScannerApp(QObject* parent = 0);

    /**
     * This method plays an audible sound upon barcode reading,
     * as well as sending the barcode data back to the parent if
     * invoked as a card.
     */
    Q_INVOKABLE void newBarcodeDetected(const QString &barcode);

public Q_SLOTS:
    /**
     * This method starts the barcode scanning if invoked
     * as a card.
     */
    void onInvoked(const bb::system::InvokeRequest&);

    /**
     * Terminates the barcode scanning when being pooled.
     */
    void onCardPooled(const bb::system::CardDoneMessage&);

Q_SIGNALS:
    /**
     * Emits signal to indicate it has stopped barcode scanning.
     */
    void stopScan();

    /**
     * Emits signal to indicate it has started barcode scanning.
     */
    void startScan();

private:
    //Card
    bb::system::InvokeManager *m_invokeManager;
    bool m_invoked;
    bool m_pooled;

    //Audio
    bb::multimedia::MediaPlayer *m_player;
};

#endif
