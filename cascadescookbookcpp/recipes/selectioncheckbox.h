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
#ifndef _SELECTIONCHECKBOX_H_
#define _SELECTIONCHECKBOX_H_

#include <bb/cascades/Color>
#include <bb/cascades/CustomControl>

using namespace bb::cascades;

namespace bb
{
    namespace cascades
    {
        class TouchEvent;
        class Label;
        class Container;
        class Color;
    }
}

/** 
 * SelectionCheckBox
 * 
 * A CustomControl that extends the CheckBox to be an entire item
 * with a title label and a colored box. The entire Control is
 * controlling the checked state of the CheckBox Control.
 */
class SelectionCheckBox: public bb::cascades::CustomControl
{
Q_OBJECT
Q_PROPERTY( QString title READ title WRITE setTitle NOTIFY titleChanged )
Q_PROPERTY( QVariant oliveColor READ oliveColor WRITE setOliveColor NOTIFY oliveColorChanged )

public:
    SelectionCheckBox(Container * parent = 0);

    void setTitle(const QString &title);
    QString title() const;

    void setOliveColor(QVariant oliveColor);
    QVariant oliveColor() const;
    void setTextColor(Color col);
    
private slots:

    /*
     * Function which is connected to the CheckBox Control in the
     * SelectionCheckBox;
     *
     * @param checked boolean parameter true if the CheckBox was checked, false otherwise
     */
    void onCheckedChanged(bool checked);

    /* Slot function used for letting the entire CheckBox Item control the
     * checked state of the CheckBox Control, but the state should only change if
     * user interaction started on the item
     * (if the user actually pressed down and up on the item).
     *
     * @ event the touch event
     */
    void onCheckBoxComponentTouch(bb::cascades::TouchEvent *event);

signals:
    void titleChanged();
    void oliveColorChanged(QVariant oliveColor);

private:
    bool mPressed;
    QString mTitle;
    QVariant mOliveColor;

    Container *mColorContainer;
    Label *mTitleLabel;
};

#endif // ifndef _SELECTIONCHECKBOX_H_
