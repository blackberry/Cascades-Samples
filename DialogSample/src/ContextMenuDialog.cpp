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

#include "ContextMenuDialog.hpp"
#include <bps/bps.h>
#include <QDebug>

namespace
{
const char* copyButtonContext = "ContextMenuDialog_Copy";
const char* cutButtonContext = "ContextMenuDialog_Cut";
const char* pasteButtonContext = "ContextMenuDialog_Paste";
}

ContextMenuDialog::ContextMenuDialog(QObject* parent)
    : QThread(parent)
    , m_copyButton(false)
    , m_cutButton(false)
    , m_pasteButton(false)
    , m_backgroundAlpha(0.0)
    , m_width(-1)
    , m_height(-1)
    , m_locationX(-1)
    , m_locationY(-1)
{
}

ContextMenuDialog::~ContextMenuDialog()
{
}

void ContextMenuDialog::show()
{
    if (!isRunning())
    {
        start();
    }
}

void ContextMenuDialog::addCustomButton(QString const& text, QString const& context)
{
    if (text.trimmed().isEmpty())
    {
        return;
    }

    m_customButtons[text] = context;
}

bool ContextMenuDialog::getCopyButton() const
{
    return m_copyButton;
}

void ContextMenuDialog::setCopyButton(bool value)
{
    m_copyButton = value;
}

bool ContextMenuDialog::getPasteButton() const
{
    return m_pasteButton;
}

void ContextMenuDialog::setPasteButton(bool value)
{
    m_pasteButton = value;
}

bool ContextMenuDialog::getCutButton() const
{
    return m_cutButton;
}

void ContextMenuDialog::setCutButton(bool value)
{
    m_cutButton = value;
}

int ContextMenuDialog::getWidth() const
{
    return m_width;
}

void ContextMenuDialog::setWidth(int value)
{
    m_width = value;
}

int ContextMenuDialog::getHeight() const
{
    return m_height;
}

void ContextMenuDialog::setHeight(int value)
{
    m_height = value;
}

int ContextMenuDialog::getLocationX() const
{
    return m_locationX;
}

void ContextMenuDialog::setLocationX(int value)
{
    m_locationX = value;
}

int ContextMenuDialog::getLocationY() const
{
    return m_locationY;
}

void ContextMenuDialog::setLocationY(int value)
{
    m_locationY = value;
}

void ContextMenuDialog::run()
{
    bps_initialize();

    //request all dialog events
    dialog_request_events(0);
    if (dialog_create_context_menu(&m_dialog) != BPS_SUCCESS)
    {
        qDebug() << "Failed to create context menu dialog.";
        return;
    }

    //show localized copy button
    if (m_copyButton && dialog_context_menu_add_button(m_dialog, DIALOG_COPY_LABEL, true, copyButtonContext, true, DIALOG_COPY_ICON) != BPS_SUCCESS)
    {
        qDebug() << "Failed to add Copy button to context menu dialog.";
        dialog_destroy(m_dialog);
        m_dialog = 0;
        return;
    }

    //show the localized cut button
    if (m_cutButton && dialog_context_menu_add_button(m_dialog, DIALOG_CUT_LABEL, true, cutButtonContext, true, DIALOG_CUT_ICON) != BPS_SUCCESS)
    {
        qDebug() << "Failed to add Cut button to context menu dialog.";
        dialog_destroy(m_dialog);
        m_dialog = 0;
        return;
    }

    //show the localized paste button
    if (m_pasteButton && dialog_context_menu_add_button(m_dialog, DIALOG_PASTE_LABEL, true, pasteButtonContext, true, DIALOG_PASTE_ICON) != BPS_SUCCESS)
    {
        qDebug() << "Failed to add Paste button to context menu dialog.";
        dialog_destroy(m_dialog);
        m_dialog = 0;
        return;
    }

    //add custom buttons
    for (QMap<QString, QString>::iterator it = m_customButtons.begin(); it != m_customButtons.end(); ++it)
    {
        if (dialog_context_menu_add_button(m_dialog, it.key().toAscii(), true, it.value().toAscii(), true, DIALOG_NO_ICON) != BPS_SUCCESS)
        {
            qDebug() << "Failed to add " << it.key() << " button to context menu dialog.";    //log only.
        }
    }

    //set height, width, coordinate x,y if value valid
    if (m_height >= 0 && dialog_set_context_menu_height(m_dialog, m_height) != BPS_SUCCESS)
    {
        qDebug() << "Failed to set the height of the context menu dialog."; //log only
    }

    if (m_width >= 0 && dialog_set_context_menu_width(m_dialog, m_width) != BPS_SUCCESS)
    {
        qDebug() << "Failed to set the height of the context menu dialog."; //log only
    }

    if (m_locationX >= 0 && m_locationY >= 0 && dialog_set_context_menu_coordinates(m_dialog, m_locationX, m_locationY) != BPS_SUCCESS)
    {
        qDebug() << "Failed to set the screen coordinates of the context menu dialog."; //log only
    }

    if (dialog_show(m_dialog) != BPS_SUCCESS)
    {
        qDebug() << "Failed to show context menu dialog.";
        dialog_destroy(m_dialog);
        m_dialog = 0;
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

                if (label != NULL && context != NULL)
                {
                    if (QString::compare(context, copyButtonContext) == 0)
                    {
                        emit copy();
                    }
                    else if (QString::compare(context, cutButtonContext) == 0)
                    {
                        emit cut();
                    }
                    else if (QString::compare(context, pasteButtonContext) == 0)
                    {
                        emit paste();
                    }
                    else
                    {
                        emit customButton(selectedIndex, label, context);
                    }
                }

                qDebug() << "Got context menu dialog click";
                shutdown = true;
            }
        }
    }

    if (m_dialog)
    {
        dialog_destroy(m_dialog);
    }
}
