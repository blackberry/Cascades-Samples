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

#include "FileBrowseDialog.hpp"
#include <bps/bps.h>
#include <QDebug>

FileBrowseDialog::FileBrowseDialog(QObject* parent)
    : QThread(parent)
    , m_multiSelect(false)
{
    m_filters.push_back(QString("*.*"));
}

FileBrowseDialog::~FileBrowseDialog()
{
}

void FileBrowseDialog::show()
{
    if (!isRunning())
    {
    	m_filePaths.clear();
        start();
    }
}

QVariant FileBrowseDialog::getFilePaths() const
{
    return m_filePaths;
}

bool FileBrowseDialog::getMultiSelect() const
{
    return m_multiSelect;
}

void FileBrowseDialog::setMultiSelect(bool value)
{
    m_multiSelect = value;
}

QVariant FileBrowseDialog::getFilters() const
{
    return m_filters;
}

void FileBrowseDialog::setFilters(QVariant const& value)
{
    m_filters = value.toList();
    qDebug() << "filter count: " << m_filters.count();
}

void FileBrowseDialog::run()
{
    bps_initialize();

    //request all dialog events
    dialog_request_events(0);
    if (dialog_create_filebrowse(&m_dialog) != BPS_SUCCESS)
    {
        qDebug() << "Failed to create file browse dialog.";
        emit selectionCancelled();
        return;
    }

    //set the selection filters
    if (m_filters.count() > 0)
    {
        char** ext = (char**)new char[m_filters.count()*sizeof(char*)];
        int i = 0;
        for (QVariantList::iterator it = m_filters.begin(); it != m_filters.end(); ++it, ++i)
        {
            QString filter = it->toString();
            if (!filter.trimmed().isEmpty())
            {
                int length = (filter.length() + 1) * sizeof(char);
                ext[i] = new char[length];
                strncpy(ext[i], filter.toAscii(), length);
            }
        }
        if (dialog_set_filebrowse_filter(m_dialog, (const char**)ext, m_filters.count()) != BPS_SUCCESS)
        {
            qDebug() << "unable to set file browse dialog extensions";
        }
        for (i = 0; i < m_filters.count(); i++)
        {
            delete ext[i];
        }
        delete ext;
    }

    if (dialog_show(m_dialog) != BPS_SUCCESS)
    {
        qDebug() << "Failed to show file browse dialog.";
        dialog_destroy(m_dialog);
        m_dialog = 0;
        emit selectionCancelled();
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
                //0=ok, 1=cancel
                int selectedIndex = dialog_event_get_selected_index(event);

                if (selectedIndex == 1)
                {
                    int count;
                    char** filepaths;
                    if (BPS_SUCCESS == dialog_event_get_filebrowse_filepaths(event, &filepaths, &count))
                    {
                        for (int i = 0; i < count; i++)
                        {
                            qDebug() << "selected file: " << filepaths[i];
                            m_filePaths.push_back(QString(filepaths[i]));
                        }
                        bps_free(filepaths);
                    }
                    emit selectionCompleted();
                }
                else
                {
                    emit selectionCancelled();
                }

                qDebug() << "Got file browse dialog click";
                shutdown = true;
            }
        }
    }

    if (m_dialog)
    {
        dialog_destroy(m_dialog);
    }
}
