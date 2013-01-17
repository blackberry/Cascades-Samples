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
#ifndef BarcodeInvoker_HPP_
#define BarcodeInvoker_HPP_

#include <QObject>

namespace bb {
namespace system {
class CardDoneMessage;
}
}

/*!
 * @brief Application pane object
 *
 * Use this object to request barcodescanner card and retrieve data sent by the card
 * upon finishing request.
 */
class BarcodeInvoker: public QObject
{
    Q_OBJECT

    // The barcode data
    Q_PROPERTY(QString barcode READ barcode NOTIFY barcodeChanged)

public:
    BarcodeInvoker(QObject* obj);
    virtual ~BarcodeInvoker()
    {
    }

public Q_SLOTS:
    /**
     * Retrieves the message upon receiving car done signal from invoke manager.
     */
    void onChildCardDone(const bb::system::CardDoneMessage &message);

    /**
     * Send the request to the barcodescanner card via invoke manager.
     */
    void onInvokeButtonClicked() const;

Q_SIGNALS:
    // The change notification signal for the property
    void barcodeChanged();

private:
    QString m_barcode;
    QString barcode() const;
    void setBarcode(const QString &barcode);
};

#endif /* BarcodeInvoker_HPP_ */
