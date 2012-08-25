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

#include "FileSaveDialog.hpp"
#include <bps/bps.h>
#include <QDebug>

FileSaveDialog::FileSaveDialog(QObject* parent)
    : Dialog(parent)
    , m_filename("")
{
}

FileSaveDialog::~FileSaveDialog()
{
}

QString FileSaveDialog::getFilename() const
{
    return m_filename;
}

void FileSaveDialog::setFilename(QString const& value)
{
    m_filename = value;
}

void FileSaveDialog::run()
{
    bps_initialize();

    //request all dialog events
    dialog_request_events(0);
    if (dialog_create_filesave(&m_dialog) != BPS_SUCCESS)
    {
        qDebug() << "Failed to create file save dialog.";
        emit cancel();
        return;
    }

    if (!m_filename.trimmed().isEmpty() && dialog_set_filesave_filename(m_dialog, m_filename.toAscii()) != BPS_SUCCESS)
    {
        qDebug() << "Failed to set the file save dialog filename";
        dialog_destroy(m_dialog);
        m_dialog = 0;
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
        qDebug() << "Failed to show file save dialog.";
        destroyDialog();
        emit cancel();
        return;
    }

    bool shutdown = false;
    while (!shutdown)
    {
        bps_event_t* event = NULL;
        bps_get_event(&event, -1);    // -1 means that the function waits
        // for an event before returning

        if (event)
        {
            if (bps_event_get_domain(event) == dialog_get_domain())
            {
                int selectedIndex = dialog_event_get_selected_index(event);
                const char* label = dialog_event_get_selected_label(event);
                const char* context = dialog_event_get_selected_context(event);

                const char* filename = dialog_event_get_filesave_filepath(event);
                if (filename != NULL)
                {
                    m_filename = filename;
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

                qDebug() << "Got file save dialog click";
                shutdown = true;
            }
        }
    }
    destroyDialog();
}
