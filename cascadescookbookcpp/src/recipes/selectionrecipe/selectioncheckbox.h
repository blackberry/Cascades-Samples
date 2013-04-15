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
        class CheckBox;
        class Container;
        class Color;
    }
}

/** 
 * SelectionCheckBox Description:
 * 
 * A CustomControl that extends the CheckBox to be an entire item
 * with a title label and a colored box.
 */
class SelectionCheckBox: public bb::cascades::CustomControl
{
    Q_OBJECT
    Q_PROPERTY( QString title READ title WRITE setTitle NOTIFY titleChanged )
    Q_PROPERTY( QVariant oliveColor READ oliveColor WRITE setOliveColor NOTIFY oliveColorChanged )

public:
    /**
     * Constructor that sets up the CustomControl for the selection checkbox
     * @param parent The parent Container, if not specified, 0 is used.
     **/
    SelectionCheckBox(Container * parent = 0);

    /**
     * Accessor and Mutator methods for mTitle
     **/
    void setTitle(const QString &title);
    QString title() const;

    /**
     * Accessor and Mutator methods for mOliveColor
     */
    void setOliveColor(QVariant oliveColor);
    QVariant oliveColor() const;

private slots:

    /*
     * This Slot function is connected to the CheckBox Control in the SelectionCheckBox;
     *
     * @param checked boolean parameter true if the CheckBox was checked, false otherwise
     */
    void onCheckedChanged(bool checked);

    signals:
    void titleChanged();
    void oliveColorChanged(QVariant oliveColor);

private:
    QString mTitle;
    QVariant mOliveColor;

    Container *mColorContainer;
    CheckBox *mOliveCheckBox;
};

#endif // ifndef _SELECTIONCHECKBOX_H_
