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
#include "BarcodeInvoker.hpp"

#include <bb/system/CardDoneMessage>
#include <bb/system/InvokeManager>
#include <bb/system/InvokeRequest>
#include <bb/system/InvokeTargetReply>

#include <QtCore/QtDebug>

using namespace bb::system;
//! [0]
BarcodeInvoker::BarcodeInvoker(QObject* obj)
    : QObject(obj)
{
    QObject::connect(new InvokeManager(this),
            SIGNAL(childCardDone(const bb::system::CardDoneMessage&)), this,
            SLOT(onChildCardDone(const bb::system::CardDoneMessage&)));
}
//! [0]
//! [1]
void BarcodeInvoker::onInvokeButtonClicked() const
{
    InvokeManager* imanager = qobject_cast<InvokeManager*>(sender());
    InvokeRequest invokeRequest;
    invokeRequest.setTarget("com.example.BarcodeScanner");
    invokeRequest.setAction("community.action.SCANBARCODE");
    InvokeTargetReply *invokeReply = imanager->invoke(invokeRequest);

    if (0 == invokeReply) {
        qDebug() << "failed to sent invoke request message";
    }
}

void BarcodeInvoker::onChildCardDone(const bb::system::CardDoneMessage &message)
{
    setBarcode(message.data());
}

QString BarcodeInvoker::barcode() const
{
    return m_barcode;
}

void BarcodeInvoker::setBarcode(const QString &barcode)
{
    m_barcode = barcode;
    Q_EMIT barcodeChanged();
}
//! [1]
