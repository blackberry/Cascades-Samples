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

#ifndef NFCRECEIVER_HPP
#define NFCRECEIVER_HPP

#include <QObject>

#include <bb/cascades/QListDataModel>
#include <bb/system/InvokeRequest.hpp>

/*!
 * @brief A helper class that encapsulates the code to receive a NDEF message.
 */
class NfcReceiver : public QObject
{
    Q_OBJECT

    // A property to make the received NDEF message records available to the UI
    Q_PROPERTY(bb::cascades::DataModel* messageRecords READ messageRecords CONSTANT)

public:
    /**
     * Creates a new NFC receiver object.
     *
     * @param parent The parent object.
     */
    NfcReceiver(QObject *parent = 0);

Q_SIGNALS:
    // This signal is emitted whenever a new NDEF message has been received
    void messageReceived();

private Q_SLOTS:
    // This slot is invoked whenever the InvokeManager detects an invocation request for this application
    void receivedInvokeTarget(const bb::system::InvokeRequest& request);

private:
    // The accessor method of the property
    bb::cascades::DataModel* messageRecords() const;

    // The model that contains the records of the received NDEF message
    bb::cascades::QVariantListDataModel* m_messageRecords;
};

#endif
