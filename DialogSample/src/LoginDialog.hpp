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

#ifndef LOGINDIALOG_HPP_
#define LOGINDIALOG_HPP_

#include "Dialog.hpp"

/*
 * The LoginDialog is a dialog for entering login info.
 * The username, place holder texts, password and remember me options
 * can be set or have their labels customized through QML properties
 */
class LoginDialog : public Dialog
{
    Q_OBJECT

    /*
     * QML property for the login message to display
     */
    Q_PROPERTY(QString loginMessage READ getLoginMessage WRITE setLoginMessage)

    /*
     * QML property to set or retrieve the username
     */
    Q_PROPERTY(QString username READ getUsername WRITE setUsername)

    /*
     * QML property for the username input placeholder
     */
    Q_PROPERTY(QString usernamePlaceholder READ getUsernamePlaceholder WRITE setUsernamePlaceholder)

    /*
     * QML property for the username label
     */
    Q_PROPERTY(QString usernameLabel READ getUsernameLabel WRITE setUsernameLabel)

    /*
     * QML property to set or retrieve the password
     */
    Q_PROPERTY(QString password READ getPassword WRITE setPassword)

    /*
     * QML property for the password input placeholder text
     */
    Q_PROPERTY(QString passwordPlaceholder READ getPasswordPlaceholder WRITE setPasswordPlaceholder)

    /*
     * QML property for the password label
     */
    Q_PROPERTY(QString passwordLabel READ getPasswordLabel WRITE setPasswordLabel)

    /*
     * QML property for the remember me flag. true is checked, false is unchecked
     */
    Q_PROPERTY(bool rememberme READ getRememberme WRITE setRememberme)

    /*
     * QML property for the remember me label text
     */
    Q_PROPERTY(QString remembermeLabel READ getRemembermeLabel WRITE setRemembermeLabel)
public:
    /*
     * Ctor & Dtor
     */
    LoginDialog(QObject* parent = 0);
    virtual ~LoginDialog();

public:
    /*
     * Getter and Setter for the login message QML property
     */
    QString getLoginMessage() const;
    void setLoginMessage(QString const& value);

    /*
     * Getter and Setter for the username QML property
     */
    QString getUsername() const;
    void setUsername(QString const& value);

    /*
     * Getter and Setter for the username placeholder text QML property
     */
    QString getUsernamePlaceholder() const;
    void setUsernamePlaceholder(QString const& value);

    /*
     * Getter and Setter for the username label QML property
     */
    QString getUsernameLabel() const;
    void setUsernameLabel(QString const& value);

    /*
     * Getter and Setter for the password QML property
     */
    QString getPassword() const;
    void setPassword(QString const& value);

    /*
     * Getter and Setter for the password placeholder QML property
     */
    QString getPasswordPlaceholder() const;
    void setPasswordPlaceholder(QString const& value);

    /*
     * Getter and Setter for the password label QML property
     */
    QString getPasswordLabel() const;
    void setPasswordLabel(QString const& value);

    /*
     * Getter and Setter for the remember me QML property
     */
    bool getRememberme() const;
    void setRememberme(bool value);

    /*
     * Getter and Setter for the remember me label QML property
     */
    QString getRemembermeLabel() const;
    void setRemembermeLabel(QString const& value);

protected:
    /*
     * Implements the run loop. Dialog stays open until a button is clicked.
     */
    virtual void run();

protected:
    QString m_loginMessage;
    QString m_username;
    QString m_usernamePlaceholder;
    QString m_usernameLabel;
    QString m_password;
    QString m_passwordPlaceholder;
    QString m_passwordLabel;
    bool m_rememberme;
    QString m_remembermeLabel;
};

#endif /* LOGINDIALOG_HPP_ */
