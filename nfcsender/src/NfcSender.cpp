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

#include "NfcSender.hpp"

#include <QtCore/QDebug>

#include <bps/bps.h>

//! [0]
NfcSender::NfcSender(QObject *parent)
    : QObject(parent)
    , m_payload(QLatin1String("Hello NFC"))
{
    bps_initialize();
    subscribe(nfc_get_domain());

    const int rc = nfc_request_events();
    if (rc == NFC_RESULT_SUCCESS) {
        qDebug() << "[INFO] Request NFC Events: NFC_RESULT_SUCCESS" << endl;
    } else {
        nfc_stop_events();
        unsubscribe(nfc_get_domain());
        bps_shutdown();
        qDebug() << "[ERRO] Request NFC Events: NFC_RESULT_FAILURE" << endl;
    }
    nfc_register_snep_client();
}
//! [0]

NfcSender::~NfcSender()
{
    nfc_unregister_snep_client();
    nfc_stop_events();
    unsubscribe(nfc_get_domain());
    bps_shutdown();
}

//! [1]
void NfcSender::event(bps_event_t *event)
{
    uint16_t code = bps_event_get_code(event);
    nfc_event_t *nfc_event = 0;

    if (nfc_get_nfc_event(event, &nfc_event) != BPS_SUCCESS) {
        qDebug() << "[ERRO] Get NFC event: BPS_FAILURE" << endl;
    }

    nfc_target_t *target = 0;
    nfc_get_target(nfc_event, &target);

    switch (code) {
        case NFC_SNEP_CONNECTION_EVENT: {
            qDebug() << "[INFO] Received NFC_NDEF_PUSH_EVENT" << endl;
            handleSnepPush(target);
        }
        break;
        case NFC_NDEF_PUSH_SUCCEED_EVENT: {
            qDebug() << "[INFO] Received NFC_NDEF_PUSH_SUCCEED_EVENT" << endl;
        }
        break;
        case NFC_NDEF_PUSH_FAILURE_MSG_OVER_SIZE_EVENT: {
            qDebug() << "[ERRO] Received NFC_NDEF_PUSH_FAILURE_MSG_OVER_SIZE_EVENT" << endl;
        }
        break;
        case NFC_NDEF_PUSH_FAILURE_REJECTED_EVENT: {
            qDebug() << "[ERRO] Received NFC_NDEF_PUSH_FAILURE_REJECTED_EVENT" << endl;
        }
        break;
        case NFC_NDEF_PUSH_FAILURE_IO_ERROR_EVENT: {
            qDebug() << "[ERRO] Received NFC_NDEF_PUSH_FAILURE_IO_ERROR_EVENT" << endl;
        }
        break;
        default: {
            qDebug() << "[WARN] Event not handled: " << code << endl;
        }
        break;
    }

    nfc_destroy_target(target);
}
//! [1]

QString NfcSender::payload() const
{
    return m_payload;
}

void NfcSender::setPayload(const QString &payload)
{
    if (m_payload == payload)
        return;

    m_payload = payload;
    emit payloadChanged();
}

//! [2]
void NfcSender::handleSnepPush(nfc_target_t *target)
{
    nfc_ndef_message_t *ndef_message;
    nfc_create_ndef_message(&ndef_message);

    nfc_ndef_record_t *record;
    const char* record_type = "text/plain";
    const uchar_t *payload = (const uchar_t *)m_payload.toUtf8().constData();

    //TODO: Kind of sketchy.
    const int payload_length = m_payload.toUtf8().length();
    nfc_create_ndef_record(NDEF_TNF_MEDIA, record_type, payload, payload_length, 0, &record);

    nfc_add_ndef_record(ndef_message, record);

    nfc_push_ndef_message(target, ndef_message);
    nfc_delete_ndef_message(ndef_message, true);
}
//! [2]
