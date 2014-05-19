/* Copyright (c) 2012, 2013, 2014 BlackBerry Limited.
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
#include "selectioncheckbox.h"

#include <bb/cascades/CheckBox>
#include <bb/cascades/Color>
#include <bb/cascades/Container>
#include <bb/cascades/Label>
#include <bb/cascades/StackLayout>
#include <bb/cascades/StackLayoutProperties>

using namespace bb::cascades;

SelectionCheckBox::SelectionCheckBox(Container * parent) :
        CustomControl(parent)
{
    bool connectResult;
    Q_UNUSED(connectResult);

    // Get the UIConfig object in order to use resolution independent sizes.
    UIConfig *ui = this->ui();

    // The title is stored in the custom control so that we can present which olive
    // has been added or removed from the mix. So we initialize the title to an empty QString.
    mTitle = QString("");

    // A colored rectangle representing the olive color and the
    // CheckBox is aligned to the Right side of the component.
    Container *checkBoxContainer = Container::create().top(ui->du(3)).bottom(ui->du(3));

    StackLayout *checkBoxLayout = new StackLayout();
    checkBoxLayout->setOrientation(LayoutOrientation::LeftToRight);
    checkBoxContainer->setLayout(checkBoxLayout);

    mColorContainer = new Container();
    mColorContainer->setBackground(Color::Black);
    mColorContainer->setPreferredSize(ui->du(5), ui->du(5));
    mColorContainer->setVerticalAlignment(VerticalAlignment::Center);

    // The CheckBox which will tell if the olive should be part of the
    // olive mix or not.
    mOliveCheckBox = new CheckBox();
    mOliveCheckBox->setObjectName("checkBox");
    mOliveCheckBox->setLayoutProperties(StackLayoutProperties::create().spaceQuota(1));

    // Connect to the checkedChange signal, in order to respond to selection of the checkbox.
    connectResult = connect(mOliveCheckBox, SIGNAL(checkedChanged(bool)), this,
                                            SLOT(onCheckedChanged(bool)));
    Q_ASSERT(connectResult);

    checkBoxContainer->add(mOliveCheckBox);
    checkBoxContainer->add(mColorContainer);

    setRoot(checkBoxContainer);
}

void SelectionCheckBox::setTitle(const QString &title)
{
    if (mTitle != title) {
        mOliveCheckBox->setText(title);
        mTitle = title;
        emit titleChanged(mTitle);
    }

}

QString SelectionCheckBox::title() const
{
    return mTitle;
}

void SelectionCheckBox::setOliveColor(QVariant oliveColor)
{
    if (oliveColor != mOliveColor) {
        if (oliveColor.canConvert<Color>()) {
            Color newColor;
            newColor = oliveColor.value<Color>();
            mColorContainer->setBackground(newColor);
            mOliveColor = oliveColor;
            emit oliveColorChanged(oliveColor);
        }
    }
}

QVariant SelectionCheckBox::oliveColor() const
{
    return mOliveColor;
}

void SelectionCheckBox::onCheckedChanged(bool checked)
{
    // When the checked state changes for the CheckBox, a signal is
    // caught in this slot function and we print a status to the console.
    if (checked) {
        qDebug() << mTitle << " added to the mix";
    } else {
        qDebug() << mTitle << " removed from the mix";
    }
}
