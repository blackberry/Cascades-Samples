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

#ifndef ALERTDIALOG_HPP_
#define ALERTDIALOG_HPP_

#include "Dialog.hpp"

/*
 * An alert dialog is a simple dialog that displays title, message
 * and buttons.
 */
class AlertDialog : public Dialog
{
    Q_OBJECT

    /*
     * QML exposed property for the alert message
     */
    Q_PROPERTY(QString alertMessage READ getAlertMessage WRITE setAlertMessage)
public:
    /*
     * Ctor & Dtor
     */
    AlertDialog(QObject* parent = 0);
    virtual ~AlertDialog();

public:
    /*
     * Getter and Setter for alert message
     */
    QString getAlertMessage() const;
    void setAlertMessage(QString const& value);

protected:
    /*
     * Implements the run loop. The dialog stays open until a button is clicked.
     */
    virtual void run();

protected:
    QString m_alertMessage;
};

#endif /* ALERTDIALOG_HPP_ */
