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

#include "NfcReceiver.hpp"

#include <bb/system/InvokeManager.hpp>

#include <QtNfcSubset/qndefmessage.h>

//! [0]
NfcReceiver::NfcReceiver(QObject *parent)
    : QObject(parent)
    , m_messageRecords(new bb::cascades::QVariantListDataModel())
{
    m_messageRecords->setParent(this);

    // Create an InvokeManager
    bb::system::InvokeManager *invokeManager = new bb::system::InvokeManager(this);

    /**
     * The signal invoked(const bb::system::InvokeRequest&) is used for applications.
     */
    connect(invokeManager, SIGNAL(invoked(const bb::system::InvokeRequest&)),
            this, SLOT(receivedInvokeTarget(const bb::system::InvokeRequest&)));
}
//! [0]

//! [1]
void NfcReceiver::receivedInvokeTarget(const bb::system::InvokeRequest& request)
{
    // The data contains our NDEF message
    const QByteArray data = request.data();

    // Create out NDEF message
    const QtMobilitySubset::QNdefMessage ndefMessage = QtMobilitySubset::QNdefMessage::fromByteArray(data);

    // Fill the model with the single records
    m_messageRecords->clear();

    for (int i = 0; i < ndefMessage.size(); ++i) {
        const QtMobilitySubset::QNdefRecord record = ndefMessage.at(i);

        QVariantMap entry;
        entry["tnfType"] = record.typeNameFormat();
        entry["recordType"] = QString::fromLatin1(record.type());
        entry["payload"] = QString::fromLatin1(record.payload());
        entry["hexPayload"] = QString::fromLatin1(record.payload().toHex());

        m_messageRecords->append(entry);
    }

    emit messageReceived();
}
//! [1]

bb::cascades::DataModel* NfcReceiver::messageRecords() const
{
    return m_messageRecords;
}
