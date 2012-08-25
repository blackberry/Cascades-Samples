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

#include "AlertDialog.hpp"
#include <bps/bps.h>
#include <QDebug>

AlertDialog::AlertDialog(QObject* parent)
    : Dialog(parent)
    , m_alertMessage("")
{
}

AlertDialog::~AlertDialog()
{
}

QString AlertDialog::getAlertMessage() const
{
    return m_alertMessage;
}

void AlertDialog::setAlertMessage(QString const& value)
{
    m_alertMessage = value;
}

void AlertDialog::run()
{
    bps_initialize();

    //request all dialog events
    dialog_request_events(0);
    if (dialog_create_alert(&m_dialog) != BPS_SUCCESS)
    {
        qDebug() << "Failed to create alert dialog.";
        emit cancel();
        return;
    }

    //set the alert message
    if (!m_alertMessage.trimmed().isEmpty() && dialog_set_alert_message_text(m_dialog, m_alertMessage.toAscii()) != BPS_SUCCESS)
    {
        qDebug() << "Failed to set alert dialog message text.";
        destroyDialog();
        emit cancel();
        return;
    }

    if (!setTitle())
    {
        destroyDialog();
        emit cancel();
        return;
    }

    if (!addButtons())
    {
        destroyDialog();
        emit cancel();
        return;
    }

    if (!showDialog())
    {
        qDebug() << "Failed to show alert dialog.";
        destroyDialog();
        emit cancel();
        return;
    }

    bool shutdown = false;
    while (!shutdown)
    {
        bps_event_t* event = NULL;

        // -1 means that the function waits for an event before returning
        bps_get_event(&event, -1);

        if (event)
        {
            if (bps_event_get_domain(event) == dialog_get_domain())
            {
                int selectedIndex = dialog_event_get_selected_index(event);
                const char* label = dialog_event_get_selected_label(event);
                const char* context = dialog_event_get_selected_context(event);

                if (label != NULL && context != NULL)
                {
                    if (QString::compare(context, getCancelButtonContext()) == 0)
                    {
                        emit cancel();
                    }
                    else if (QString::compare(context, getOkButtonContext()) == 0)
                    {
                        emit ok();
                    }
                    else
                    {
                        emit customButton(selectedIndex, label, context);
                    }
                }

                qDebug() << "Got alert dialog click";
                shutdown = true;
            }
        }
    }
    destroyDialog();
}
