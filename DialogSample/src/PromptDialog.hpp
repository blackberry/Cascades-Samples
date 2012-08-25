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

#ifndef PROMPTDIALOG_HPP_
#define PROMPTDIALOG_HPP_

#include "Dialog.hpp"

/*
 * The prompt dialog displays a prompt with an input field
 */
class PromptDialog : public Dialog
{
    Q_OBJECT

    /*
     * QML exposed property to set the prompt message
     */
    Q_PROPERTY(QString promptMessage READ getPromptMessage WRITE setPromptMessage)

    /*
     * QML property to set or retrieve the string in the input field
     */
    Q_PROPERTY(QString inputField READ getInputField WRITE setInputField)

    /*
     * QML exposed property to set the placeholder text for the input field
     */
    Q_PROPERTY(QString placeholder READ getPlaceholderText WRITE setPlaceholderText)
public:
    /*
     * Ctor & Dtor
     */
    PromptDialog(QObject* parent = 0);
    virtual ~PromptDialog();

public:
    /*
     * Setter and Getter for the prompt message QML property
     */
    QString getPromptMessage() const;
    void setPromptMessage(QString const& value);

    /*
     * Setter and Getter for the placeholder text QML property
     */
    QString getPlaceholderText() const;
    void setPlaceholderText(QString const& value);

    /*
     * Setter and Getter for the input field QML property
     */
    QString getInputField() const;
    void setInputField(QString const& value);

protected:
    /*
     * Implements the run loop. Dialog stays open until dialog is closed
     */
    virtual void run();

protected:
    QString m_promptMessage;
    QString m_placeholderText;
    QString m_inputField;
};

#endif /* PROMPTDIALOG_HPP_ */
