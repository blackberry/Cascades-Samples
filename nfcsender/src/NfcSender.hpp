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

#ifndef NFCSENDER_HPP
#define NFCSENDER_HPP

#include <QObject>

#include <bb/AbstractBpsEventHandler>
#include <nfc/nfc_bps.h>

/*!
 * @brief A helper class that encapsulates the code to send a NDEF message with a given payload.
 */
class NfcSender : public QObject, public bb::AbstractBpsEventHandler
{
    Q_OBJECT

    // The property that makes the payload accessible to the UI
    Q_PROPERTY(QString payload READ payload WRITE setPayload NOTIFY payloadChanged)

public:
    /**
     * Creates a new NFC sender object.
     *
     * @param parent The parent object.
     */
    NfcSender(QObject *parent = 0);

    /**
     * Destroys the NFC sender object.
     */
    virtual ~NfcSender();

    // Reimplemented from bb::AbstractBpsEventHandler, is called whenever a BPS event is received.
    virtual void event(bps_event_t *event);

Q_SIGNALS:
    // The change notification signal of the property
    void payloadChanged();

private:
    // The accessor methods of the property
    void setPayload(const QString &payload);
    QString payload() const;

    // This method is called whenever a NDEF message can be send
    void handleSnepPush(nfc_target_t *target);

    // The payload of the NDEF message
    QString m_payload;
};

#endif
