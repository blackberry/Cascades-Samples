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

#include "LoginDialog.hpp"
#include <bps/bps.h>
#include <QDebug>

LoginDialog::LoginDialog(QObject* parent)
    : Dialog(parent)
    , m_loginMessage("")
    , m_username("")
    , m_usernamePlaceholder("")
    , m_usernameLabel("")
    , m_password("")
    , m_passwordPlaceholder("")
    , m_passwordLabel("")
    , m_rememberme(false)
    , m_remembermeLabel("")
{
}

LoginDialog::~LoginDialog()
{
}

QString LoginDialog::getLoginMessage() const
{
    return m_loginMessage;
}

void LoginDialog::setLoginMessage(QString const& value)
{
    m_loginMessage = value;
}

QString LoginDialog::getUsername() const
{
    return m_username;
}

void LoginDialog::setUsername(QString const& value)
{
    m_username = value;
}

QString LoginDialog::getUsernamePlaceholder() const
{
    return m_usernamePlaceholder;
}

void LoginDialog::setUsernamePlaceholder(QString const& value)
{
    m_usernamePlaceholder = value;
}

QString LoginDialog::getUsernameLabel() const
{
    return m_usernameLabel;
}

void LoginDialog::setUsernameLabel(QString const& value)
{
    m_usernameLabel = value;
}

QString LoginDialog::getPassword() const
{
    return m_password;
}

void LoginDialog::setPassword(QString const& value)
{
    m_password = value;
}

QString LoginDialog::getPasswordPlaceholder() const
{
    return m_passwordPlaceholder;
}

void LoginDialog::setPasswordPlaceholder(QString const& value)
{
    m_passwordPlaceholder = value;
}

QString LoginDialog::getPasswordLabel() const
{
    return m_passwordLabel;
}

void LoginDialog::setPasswordLabel(QString const& value)
{
    m_passwordLabel = value;
}

bool LoginDialog::getRememberme() const
{
    return m_rememberme;
}

void LoginDialog::setRememberme(bool value)
{
    m_rememberme = value;
}

QString LoginDialog::getRemembermeLabel() const
{
    return m_remembermeLabel;
}

void LoginDialog::setRemembermeLabel(QString const& value)
{
    m_remembermeLabel = value;
}

void LoginDialog::run()
{
    bps_initialize();

    //request all dialog events
    dialog_request_events(0);
    if (dialog_create_login(&m_dialog) != BPS_SUCCESS)
    {
        qDebug() << "Failed to create login dialog.";
        emit cancel();
        return;
    }

    //set the login message
    if (!m_loginMessage.trimmed().isEmpty() && dialog_set_login_message_text(m_dialog, m_loginMessage.toAscii()) != BPS_SUCCESS)
    {
        qDebug() << "Failed to set login dialog message text.";
        destroyDialog();
        emit cancel();
        return;
    }

    //set the username field
    if (!m_username.trimmed().isEmpty() && dialog_set_login_username(m_dialog, m_username.toAscii()) != BPS_SUCCESS)
    {
        qDebug() << "Failed to set login dialog username.";
        destroyDialog();
        emit cancel();
        return;
    }

    //set the username placeholder text
    if (!m_usernamePlaceholder.trimmed().isEmpty() && dialog_set_login_username_placeholder(m_dialog, m_usernamePlaceholder.toAscii()) != BPS_SUCCESS)
    {
        qDebug() << "Failed to set login dialog username placeholder text.";
        destroyDialog();
        emit cancel();
        return;
    }

    //set the username label
    if (!m_usernameLabel.trimmed().isEmpty() && dialog_set_login_username_label(m_dialog, m_usernameLabel.toAscii()) != BPS_SUCCESS)
    {
        qDebug() << "Failed to set login dialog username label.";
        destroyDialog();
        emit cancel();
        return;
    }

    //set the password field
    if (!m_password.trimmed().isEmpty() && dialog_set_login_password(m_dialog, m_password.toAscii()) != BPS_SUCCESS)
    {
        qDebug() << "Failed to set login dialog password.";
        destroyDialog();
        emit cancel();
        return;
    }

    //set the password placeholder text
    if (!m_passwordPlaceholder.trimmed().isEmpty() && dialog_set_login_password_placeholder(m_dialog, m_passwordPlaceholder.toAscii()) != BPS_SUCCESS)
    {
        qDebug() << "Failed to set login dialog password placeholder text.";
        destroyDialog();
        emit cancel();
        return;
    }

    //set the password label
    if (!m_passwordLabel.trimmed().isEmpty() && dialog_set_login_password_label(m_dialog, m_passwordLabel.toAscii()) != BPS_SUCCESS)
    {
        qDebug() << "Failed to set login dialog password label.";
        destroyDialog();
        emit cancel();
        return;
    }

    //set the remember me value
    if (dialog_set_login_remember_me(m_dialog, m_rememberme) != BPS_SUCCESS)
    {
        qDebug() << "Failed to set login dialog remember me value.";
        destroyDialog();
        emit cancel();
        return;
    }

    //set the remember me label
    if (!m_remembermeLabel.trimmed().isEmpty() && dialog_set_login_remember_me_label(m_dialog, m_remembermeLabel.toAscii()) != BPS_SUCCESS)
    {
        qDebug() << "Failed to set login dialog remember me label.";
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
        qDebug() << "Failed to show login dialog.";
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
                const char* username = dialog_event_get_login_username(event);
                const char* password = dialog_event_get_login_password(event);
                m_rememberme = dialog_event_get_login_remember_me(event);

                if (username != NULL)
                {
                    m_username = username;
                }
                if (password != NULL)
                {
                    m_password = password;
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

                qDebug() << "Got login dialog click";
                shutdown = true;
            }
        }
    }
    destroyDialog();
}
