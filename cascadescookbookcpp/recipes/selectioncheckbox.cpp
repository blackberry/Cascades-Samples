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
#include <bb/cascades/DockLayoutProperties>
#include <bb/cascades/TextStyle>
#include <bb/cascades/SystemDefaults>
#include <bb/cascades/Label>
#include <bb/cascades/StackLayout>
#include <bb/cascades/StackLayoutProperties>

using namespace bb::cascades;

SelectionCheckBox::SelectionCheckBox(Container * parent) :
        CustomControl(parent)
{
    Container *checkBoxComponent = new Container();
    DockLayout *checkBoxComponentLayout = new DockLayout();
    checkBoxComponentLayout->setTopPadding(41.0f);
    checkBoxComponentLayout->setBottomPadding(41.0f);
    checkBoxComponent->setPreferredWidth(600.0f);

    checkBoxComponent->setLayout(checkBoxComponentLayout);
    this->setLayoutProperties(
            StackLayoutProperties::create().horizontal(HorizontalAlignment::Fill));

    // The title is stored in the custom control so that we can present which olive
    // has been added or removed from the mix.
    mTitle = QString("");

    mTitleLabel = new Label();
    mTitleLabel->setText(mTitle);
    mTitleLabel->textStyle()->setBase(SystemDefaults::TextStyles::titleText());
    mTitleLabel->setLayoutProperties(
            DockLayoutProperties::create().vertical(VerticalAlignment::Center));

    // A colored rectangle representing the olive color and the
    // CheckBox is aligned to the Right side of the component.
    Container *checkBoxContainer = new Container();
    StackLayout *checkBoxLayout = new StackLayout();
    checkBoxLayout->setLayoutDirection(LayoutDirection::LeftToRight);

    // A colored rectangle representing the olive color and the
    // CheckBox is aligned to the Right side of the component.
    checkBoxContainer->setLayout(checkBoxLayout);
    checkBoxContainer->setLayoutProperties(
            DockLayoutProperties::create().horizontal(HorizontalAlignment::Right));

    mColorContainer = new Container();
    mColorContainer->setBackground(Color::Black);
    mColorContainer->setPreferredSize(42, 42);
    mColorContainer->setRightMargin(33.0f);
    mColorContainer->setLayoutProperties(
            StackLayoutProperties::create().vertical(VerticalAlignment::Center));

    // The CheckBox which will tell if the olive should be part of the
    // olive mix or not.
    CheckBox *oliveCheckBox = new CheckBox();
    oliveCheckBox->setObjectName("checkBox");
    connect(oliveCheckBox, SIGNAL(checkedChanged(bool)), this, SLOT(onCheckedChanged(bool)));

    checkBoxContainer->add(mColorContainer);
    checkBoxContainer->add(oliveCheckBox);

    checkBoxComponent->add(mTitleLabel);
    checkBoxComponent->add(checkBoxContainer);

    // To get a behavior that is consistent with how the RadioGroupOptions work
    // we need to change the checked state when interacting with the entire
    // check box option (not only the check box itself). So we listen for
    // touch events on the entire Container and keep track of if the press
    // down occurred on the Container
    connect(checkBoxComponent, SIGNAL(touch(bb::cascades::TouchEvent *)), this,
            SLOT(onCheckBoxComponentTouch(bb::cascades::TouchEvent *)));

    setRoot(checkBoxComponent);
}

void SelectionCheckBox::setTextColor(Color col)
{
    mTitleLabel->textStyle()->setColor(col);
}

void SelectionCheckBox::setTitle(const QString &title)
{
    if (mTitle == title) {
        return;
    }

    mTitleLabel->setText(title);
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
    }

    if (newColor.isValid()) {
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
    // When the checked state changes for the CheckBox a signal is
    // caught in this slot function and we print a status to the the console.
    if (checked) {
        qDebug() << mTitle << " added to the mix";
    } else {
        qDebug() << mTitle << " removed from the mix";
    }
}

void SelectionCheckBox::onCheckBoxComponentTouch(bb::cascades::TouchEvent *event)
{
    // The entire CheckBox Item is controlling the checked state of the CheckBox,
    // but the state should only change if user interaction started on the item
    // (if the user actually pressed down and up on the item).
    if (event->isDown()) {
        mPressed = true;
    } else if (event->isUp()) {
        VisualNode* touchControl = event->target();
        if (touchControl) {
            CheckBox *oliveCheckBox = touchControl->findChild<CheckBox*>("checkBox");

            if (mPressed && oliveCheckBox != 0) {
                oliveCheckBox->setChecked(!oliveCheckBox->isChecked());
            }
        }
        mPressed = false;
    }
}
