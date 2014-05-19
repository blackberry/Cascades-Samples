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
#include "customdialogalarm.h"

#include <bb/cascades/Color>
#include <bb/cascades/Container>
#include <bb/cascades/Dialog>
#include <bb/cascades/DockLayout>
#include <bb/cascades/ImageView>
#include <bb/cascades/Label>
#include <bb/cascades/StackLayout>
#include <bb/cascades/StackLayoutProperties>
#include <bb/cascades/SystemDefaults>
#include <bb/cascades/TextStyle>
#include <bb/cascades/ToggleButton>

using namespace bb::cascades;

CustomDialogAlarm::CustomDialogAlarm(QObject * parent) :
        QObject(parent), mVisible(false)
{
    bool connectResult;
    Q_UNUSED(connectResult);

    mCustomDialog = new Dialog();

    // Get the UIConfig object in order to use resolution independent sizes.
    UIConfig *ui = mCustomDialog->ui();

    Container *contentContainer = new Container();
    contentContainer->setLayout(new DockLayout());
    contentContainer->setHorizontalAlignment(HorizontalAlignment::Fill);
    contentContainer->setVerticalAlignment(VerticalAlignment::Fill);

    // The background is set to semi-transparent to indicate that it is not possible to interact
    // with the screen behind the dialog.
    contentContainer->setBackground(Color::fromARGB(0x88000000));

    // Dialog Container
    Container *dialogContainer = new Container();
    dialogContainer->setLayout(new DockLayout());
    dialogContainer->setHorizontalAlignment(HorizontalAlignment::Center);
    dialogContainer->setVerticalAlignment(VerticalAlignment::Center);
    dialogContainer->setMaxHeight(ui->du(39.7));
    dialogContainer->setLeftPadding(ui->du(3));
    dialogContainer->setRightPadding(ui->du(3));

    // Dialog background image
    ImageView *dialogBack = ImageView::create("asset:///images/customdialog/customdialog_alarm.png");
    dialogBack->setVerticalAlignment(VerticalAlignment::Fill);

    // Dialog content Container
    Container *dialogContent = Container::create()
                                    .top(ui->du(0.5)).bottom(ui->du(2)).left(ui->du(2));
    dialogContent->setHorizontalAlignment(HorizontalAlignment::Fill);
    dialogContent->setVerticalAlignment(VerticalAlignment::Fill);

    // Title label for dialog
    Label *dialogTitle = new Label();
    dialogTitle->setText("FIRE ALARM!");
    dialogTitle->textStyle()->setBase(SystemDefaults::TextStyles::titleText());
    dialogTitle->textStyle()->setColor(Color::fromARGB(0xfffafafa));
    dialogTitle->setLayoutProperties(StackLayoutProperties::create().spaceQuota(1));
    dialogTitle->setHorizontalAlignment(HorizontalAlignment::Center);

    // Text label for dialog
    Label *dialogText = new Label();
    dialogText->setText("BEEP! BEEP! BEEP!");
    dialogText->textStyle()->setBase(SystemDefaults::TextStyles::titleText());
    dialogTitle->textStyle()->setColor(Color::fromARGB(0xff262626));
    dialogText->setLayoutProperties(StackLayoutProperties::create().spaceQuota(2.5));

    // Toggle button for alarm
    ToggleButton *toggleAlarm = new ToggleButton();
    toggleAlarm->setLayoutProperties(StackLayoutProperties::create().spaceQuota(1));
    toggleAlarm->setHorizontalAlignment(HorizontalAlignment::Center);

    // Connect to signals
    connectResult = connect(toggleAlarm, SIGNAL(checkedChanged(bool)), this, SLOT(onAlarmSwitch(bool)));
    Q_ASSERT(connectResult);

    connectResult = connect(this, SIGNAL(visibleChanged(bool)), toggleAlarm, SLOT(setChecked(bool)));
    Q_ASSERT(connectResult);

    // Add components to the appropriate Containers
    dialogContent->add(dialogTitle);
    dialogContent->add(dialogText);
    dialogContent->add(toggleAlarm);

    dialogContainer->add(dialogBack);
    dialogContainer->add(dialogContent);
    contentContainer->add(dialogContainer);

    mCustomDialog->setContent(contentContainer);
}

CustomDialogAlarm::~CustomDialogAlarm()
{
    // If the Dialog is showing make sure it is closed and reset its content
    if (mCustomDialog->isOpened()) {
        mCustomDialog->resetContent();
        mCustomDialog->close();
    }
    delete mCustomDialog;
}

void CustomDialogAlarm::setVisible(bool visible)
{
    if (mVisible != visible) {
        if (visible) {
            mCustomDialog->open();
        } else {
            mCustomDialog->close();
        }

        mVisible = visible;
        emit visibleChanged(visible);
    }
}

bool CustomDialogAlarm::visible()
{
    return mVisible;
}

void CustomDialogAlarm::onAlarmSwitch(bool checked)
{
    if (!checked) {
        // Close the dialog when the toggle button turns the fire alarm off.
        setVisible(false);
    }
}
