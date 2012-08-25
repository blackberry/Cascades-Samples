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

#include "PromptDialog.hpp"
#include <bps/bps.h>
#include <QDebug>

PromptDialog::PromptDialog(QObject* parent)
    : Dialog(parent)
    , m_promptMessage("")
    , m_placeholderText("")
    , m_inputField("")
{
}

PromptDialog::~PromptDialog()
{
}

QString PromptDialog::getPromptMessage() const
{
    return m_promptMessage;
}

void PromptDialog::setPromptMessage(QString const& value)
{
    m_promptMessage = value;
}

QString PromptDialog::getPlaceholderText() const
{
    return m_placeholderText;
}

void PromptDialog::setPlaceholderText(QString const& value)
{
    m_placeholderText = value;
}

QString PromptDialog::getInputField() const
{
    return m_inputField;
}

void PromptDialog::setInputField(QString const& value)
{
    m_inputField = value;
}

void PromptDialog::run()
{
    bps_initialize();

    //request all dialog events
    dialog_request_events(0);
    if (dialog_create_prompt(&m_dialog) != BPS_SUCCESS)
    {
        qDebug() << "Failed to create prompt dialog.";
        emit cancel();
        return;
    }

    //set the prompt message
    if (!m_promptMessage.trimmed().isEmpty() && dialog_set_prompt_message_text(m_dialog, m_promptMessage.toAscii()) != BPS_SUCCESS)
    {
        qDebug() << "Failed to set prompt dialog message text.";
        destroyDialog();
        emit cancel();
        return;
    }

    //set the prompt input field
    if (!m_inputField.trimmed().isEmpty() && dialog_set_prompt_input_field(m_dialog, m_inputField.toAscii()) != BPS_SUCCESS)
    {
        qDebug() << "Failed to set prompt dialog input field.";
        destroyDialog();
        emit cancel();
        return;
    }

    //set the input placeholder text
    if (!m_placeholderText.trimmed().isEmpty() && dialog_set_prompt_input_placeholder(m_dialog, m_placeholderText.toAscii()) != BPS_SUCCESS)
    {
        qDebug() << "Failed to set prompt dialog placeholder text.";
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
        qDebug() << "Failed to show prompt dialog.";
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
                const char* inputfield = dialog_event_get_prompt_input_field(event);
                if (inputfield != NULL)
                {
                    m_inputField = inputfield;
                }

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

                qDebug() << "Got prompt dialog click";
                shutdown = true;
            }
        }
    }
    destroyDialog();
}
