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

#ifndef CONTEXTMENUDIALOG_HPP_
#define CONTEXTMENUDIALOG_HPP_

#include <QThread>
#include <bps/dialog.h>
#include <QMap>

/*
 * A context menu dialog shows a menu of buttons, each of which has
 * text and/or icons.
 */
class ContextMenuDialog : public QThread
{
    Q_OBJECT
    /*
     * QML exposed property to display the copy button, text and icon
     */
    Q_PROPERTY(bool copyButton READ getCopyButton WRITE setCopyButton)

    /*
     * QML exposed property to display the cut button, text and icon
     */
    Q_PROPERTY(bool cutButton READ getCutButton WRITE setCutButton)

    /*
     * QML exposed property to display the paste button, text and icon
     */
    Q_PROPERTY(bool pasteButton READ getPasteButton WRITE setPasteButton)

    /*
     * QML exposed property to set the width of the menu
     */
    Q_PROPERTY(int width READ getWidth WRITE setWidth)

    /*
     * QML exposed property to set the height of the menu
     */
    Q_PROPERTY(int height READ getHeight WRITE setHeight)

    /*
     * QML exposed property to set the screen x location of the menu
     */
    Q_PROPERTY(int screenx READ getLocationX WRITE setLocationX)

    /*
     * QML exposed property to set the screen y location of the men
     */
    Q_PROPERTY(int screeny READ getLocationY WRITE setLocationY)

public:
    /*
     * Ctor and Dtor
     */
    ContextMenuDialog(QObject* parent = 0);
    virtual ~ContextMenuDialog();

    /*
     * Exposed to QML to start the run loop which creates and displays the dialog.
     * The dialog is shown until a button is clicked.
     */
    Q_INVOKABLE void show();

    /*
     * Exposed to QML to allow custom buttons to be added to the dialog
     * @param text - the text displayed on the button
     * @param context - a string data associated with the custom button
     */
    Q_INVOKABLE void addCustomButton(QString const& text, QString const& context);

public:
    /*
     * Setter and Getter for copy button QML property
     */
    bool getCopyButton() const;
    void setCopyButton(bool value);

    /*
     * Setter and Getter for cut button QML property
     */
    bool getCutButton() const;
    void setCutButton(bool value);

    /*
     * Setter and Getter for paste button QML property
     */
    bool getPasteButton() const;
    void setPasteButton(bool value);

    /*
     * Setter and Getter for width QML property
     */
    int getWidth() const;
    void setWidth(int value);

    /*
     * Setter and Getter for Height QML property
     */
    int getHeight() const;
    void setHeight(int value);

    /*
     * Setter and Getter for screen x QML property
     */
    int getLocationX() const;
    void setLocationX(int value);

    /*
     * Setter and Getter for screen y QML property
     */
    int getLocationY() const;
    void setLocationY(int value);

signals:
    /*
     * Signal emitted when copy menu button is clicked
     */
    void copy();

    /*
     * Signal emitted when cut menu button is clicked
     */
    void cut();

    /*
     * Signal emitted when paste menu button is clicked
     */
    void paste();

    /*
     * Signal emitted when a custom menu button is clicked
     */
    void customButton(int index, QString label, QString context);

protected:
    /*
     * Implements the run loop. Displays the menu until a button is clicked
     */
    virtual void run();

private:
    dialog_instance_t m_dialog;
    bool m_copyButton;
    bool m_cutButton;
    bool m_pasteButton;
    double m_backgroundAlpha;
    int m_width;
    int m_height;
    int m_locationX;
    int m_locationY;
    QMap<QString, QString> m_customButtons;
};

#endif /* CONTEXTMENUDIALOG_HPP_ */
