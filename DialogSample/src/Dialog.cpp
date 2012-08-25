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

#include "Dialog.hpp"
#include <bps/bps.h>
#include <QDebug>

namespace
{
const char* cancelButtonContext = "Dialog_Cancelled";
const char* okButtonContext = "Dialog_OK";
}

Dialog::Dialog(QObject* parent)
    : QThread(parent)
    , m_cancelButton(false)
    , m_okButton(false)
    , m_cancelRequired(false)
    , m_titleText("")
{
}

Dialog::~Dialog()
{
}

void Dialog::show()
{
    if (!isRunning())
    {
        start();
    }
}

void Dialog::addCustomButton(QString const& text, QString const& context)
{
    if (text.trimmed().isEmpty())
    {
        return;
    }

    m_customButtons[text] = context;
}

bool Dialog::getShowCancelButtonProperty() const
{
    return m_cancelButton;
}

void Dialog::setShowCancelButtonProperty(bool value)
{
    m_cancelButton = value;
}

bool Dialog::getShowOkButtonProperty() const
{
    return m_okButton;
}

void Dialog::setShowOkButtonProperty(bool value)
{
    m_okButton = value;
}

QString Dialog::getTitleTextProperty() const
{
    return m_titleText;
}

void Dialog::setTitleTextProperty(QString const& value)
{
    m_titleText = value;
}

QString Dialog::getOkButtonContext() const
{
    return okButtonContext;
}

QString Dialog::getCancelButtonContext() const
{
    return cancelButtonContext;
}

bool Dialog::showDialog()
{
    return (dialog_show(m_dialog) == BPS_SUCCESS);
}

void Dialog::destroyDialog()
{
    if (m_dialog)
    {
        dialog_destroy(m_dialog);
        m_dialog = 0;
    }
}

bool Dialog::setTitle()
{
    if (!m_titleText.trimmed().isEmpty() && dialog_set_title_text(m_dialog, m_titleText.toAscii()) != BPS_SUCCESS)
    {
        qDebug() << "Failed to set alert dialog title text.";
        return false;
    }
    return true;
}

bool Dialog::addButtons()
{
    //show localized ok button
    if (m_okButton && dialog_add_button(m_dialog, DIALOG_OK_LABEL, true, okButtonContext, true) != BPS_SUCCESS)
    {
        qDebug() << "Failed to add OK button to alert dialog.";
        return false;
    }

    //show the localized cancel button
    if (m_cancelButton && dialog_add_button(m_dialog, DIALOG_CANCEL_LABEL, true, cancelButtonContext, true) != BPS_SUCCESS)
    {
        qDebug() << "Failed to add CANCEL button to alert dialog.";
        return false;
    }

    //add custom buttons
    for (QMap<QString, QString>::iterator it = m_customButtons.begin(); it != m_customButtons.end(); ++it)
    {
        if (dialog_add_button(m_dialog, it.key().toAscii(), true, it.value().toAscii(), true) != BPS_SUCCESS)
        {
            qDebug() << "Failed to add " << it.key() << " button to alert dialog.";
            return false;
        }
    }
    return true;
}
