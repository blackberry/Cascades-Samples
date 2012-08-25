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

#ifndef DIALOG_HPP_
#define DIALOG_HPP_

#include <QThread>
#include <bps/dialog.h>
#include <QMap>

/*
 * This is a base class for certain types of dialogs like the
 * Alert, Prompt, Login and FileSave Dialogs which require
 * addition of custom buttons.
 *
 * Derived classes implement a run loop specific to the type of
 * dialog being displayed.
 */
class Dialog : public QThread
{
    Q_OBJECT

    /*
     * Property exposed to QML to show or hide the localized cancel button
     */
    Q_PROPERTY(bool cancelButton READ getShowCancelButtonProperty WRITE setShowCancelButtonProperty)

    /*
     * Property exposed to QML to show or hide the localized OK button
     */
    Q_PROPERTY(bool okButton READ getShowOkButtonProperty WRITE setShowOkButtonProperty)

    /*
     * Property exposed to QML to set the title of the dialog. Note: not all dialogs
     * will show a title, for example the filesave dialog
     */
    Q_PROPERTY(QString titleText READ getTitleTextProperty WRITE setTitleTextProperty)
public:
    /*
     * Ctor & Dtor
     */
    Dialog(QObject* parent = 0);
    virtual ~Dialog();

    /*
     * Exposed to QML to start the run loop which creates and displays the dialog.
     * The dialog is shown until a button is clicked.
     */
    virtual Q_INVOKABLE void show();

    /*
     * Exposed to QML to allow custom buttons to be added to the dialog
     * @param text - the text displayed on the button
     * @param context - a string data associated with the custom button
     */
    virtual Q_INVOKABLE void addCustomButton(QString const& text, QString const& context);

public:
    /*
     * Getter and Setter for the show OK button property exposed to QML
     */
    bool getShowOkButtonProperty() const;
    void setShowOkButtonProperty(bool value);

    /*
     * Getter and Setter for the show Cancel button property exposed to QML
     */
    bool getShowCancelButtonProperty() const;
    void setShowCancelButtonProperty(bool value);

    /*
     * Getter and Setter for the Title text property exposed to QML
     */
    QString getTitleTextProperty() const;
    void setTitleTextProperty(QString const& value);

protected:
    /*
     * Adds the OK, Cancel and Custom buttons into the dialog
     */
    bool addButtons();

    /*
     * Sets the title of the dialog
     */
    bool setTitle();

    /*
     * Displays the dialog on the screen
     */
    bool showDialog();

    /*
     * Destroys the dialog
     */
    void destroyDialog();

    /*
     * Returns the context string for the localized OK button
     */
    QString getOkButtonContext() const;

    /*
     * Returns the context string for the localized Cancel button
     */
    QString getCancelButtonContext() const;

signals:
    /*
     * Signal emitted when the localized OK button is clicked
     */
    void ok();

    /*
     * Signal emitted when the localized CANCEL button is clicked
     */
    void cancel();

    /*
     * Signal emitted when a custom button is clicked
     * @param index - the index of the custom button
     * @param label - the label of the custom button
     * @param context - the context data of the custom button
     */
    void customButton(int index, QString label, QString context);

protected:
    dialog_instance_t m_dialog;
    bool m_cancelButton;
    bool m_okButton;
    bool m_cancelRequired;
    QString m_titleText;
    QMap<QString, QString> m_customButtons;
};

#endif /* DIALOG_HPP_ */
