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
#include "selectioncheckbox.h"

#include <bb/cascades/CheckBox>
#include <bb/cascades/Color>
#include <bb/cascades/Container>
#include <bb/cascades/DockLayout>
#include <bb/cascades/Label>
#include <bb/cascades/StackLayout>
#include <bb/cascades/StackLayoutProperties>
#include <bb/cascades/SystemDefaults>
#include <bb/cascades/TextStyle>

using namespace bb::cascades;

SelectionCheckBox::SelectionCheckBox(Container * parent) :
        CustomControl(parent)
{
    // The title is stored in the custom control so that we can present which olive
    // has been added or removed from the mix. So we initialize the title to an empty QString.
    mTitle = QString("");

    // A colored rectangle representing the olive color and the
    // CheckBox is aligned to the Right side of the component.
    Container *checkBoxContainer = Container::create().top(43.0f).bottom(43.0f);

    StackLayout *checkBoxLayout = new StackLayout();
    checkBoxLayout->setOrientation(LayoutOrientation::LeftToRight);
    checkBoxContainer->setLayout(checkBoxLayout);

    mColorContainer = new Container();
    mColorContainer->setBackground(Color::Black);
    mColorContainer->setPreferredSize(42, 42);
    mColorContainer->setRightMargin(33.0f);
    mColorContainer->setVerticalAlignment(VerticalAlignment::Center);

    // The CheckBox which will tell if the olive should be part of the
    // olive mix or not.
    mOliveCheckBox = new CheckBox();
    mOliveCheckBox->setObjectName("checkBox");
    mOliveCheckBox->setLayoutProperties(StackLayoutProperties::create().spaceQuota(1));

    connect(mOliveCheckBox, SIGNAL(checkedChanged(bool)), this,
            SLOT(onCheckedChanged(bool)));

    checkBoxContainer->add(mOliveCheckBox);
    checkBoxContainer->add(mColorContainer);

    setRoot(checkBoxContainer);
}

void SelectionCheckBox::setTitle(const QString &title)
{
    if (mTitle == title) {
        return;
    }

    mOliveCheckBox->setText(title);
    mTitle = title;
}

QString SelectionCheckBox::title() const
{
    return mTitle;
}

void SelectionCheckBox::setOliveColor(QVariant oliveColor)
{
    Color newColor;
    if (oliveColor == mOliveColor) {
        return;
    }

    if (oliveColor.canConvert<Color>()) {
        newColor = oliveColor.value<Color>();
        mColorContainer->setBackground(newColor);
        mOliveColor = oliveColor;
        emit oliveColorChanged(oliveColor);
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
