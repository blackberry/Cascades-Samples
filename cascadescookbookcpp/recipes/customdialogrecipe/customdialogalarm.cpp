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
#include "customdialogalarm.h"

#include <bb/cascades/Color>
#include <bb/cascades/Container>
#include <bb/cascades/CustomDialog>
#include <bb/cascades/DockLayout>
#include <bb/cascades/DockLayoutProperties>
#include <bb/cascades/ImageView>
#include <bb/cascades/Label>
#include <bb/cascades/StackLayout>
#include <bb/cascades/StackLayoutProperties>
#include <bb/cascades/SystemDefaults>
#include <bb/cascades/TextStyle>
#include <bb/cascades/ToggleButton>

using namespace bb::cascades;

CustomDialogAlarm::CustomDialogAlarm(QObject * parent) :
        QObject(parent)
{
    mCustomDialog = new CustomDialog();

    Container *contentContainer = new Container();
    contentContainer->setLayout(new DockLayout());

    // The dialog does not automatically fill the entire screen like a Page does, so in order
    // for it to be possible to center the dialog on screen, the width and height has to be set.
    contentContainer->setPreferredSize(768, 1280);

    // The background is set to semi transparent to indicate that it is not possible to interact
    // with the screen behind the dialog.
    contentContainer->setBackground(Color::fromARGB(0x88000000));

    Container *dialogContainer = new Container();
    dialogContainer->setLayout(new DockLayout());
    dialogContainer->setLayoutProperties(DockLayoutProperties::create().horizontal(HorizontalAlignment::Center).vertical(VerticalAlignment::Center));
    dialogContainer->setMaxHeight(397);

    ImageView *dialogBack = ImageView::create("asset:///images/customdialog/customdialog_alarm");

    Container *dialogContent = new Container();
    dialogContent->setLayout(StackLayout::create().top(5).bottom(23).left(23));

    Label *dialogTitle = new Label();
    dialogTitle->setText("FIRE ALARM!");
    dialogTitle->textStyle()->setBase(SystemDefaults::TextStyles::titleText());
    dialogTitle->textStyle()->setColor(Color::fromARGB(0xfffafafa));
    dialogTitle->setLayoutProperties(StackLayoutProperties::create().horizontal(HorizontalAlignment::Center).spaceQuota(1));

    Label *dialogText = new Label();
    dialogText->setText("BEEP! BEEP! BEEP!");
    dialogText->textStyle()->setBase(SystemDefaults::TextStyles::titleText());
    dialogText->setLayoutProperties(StackLayoutProperties::create().spaceQuota(2.5));

    ToggleButton *toggleAlarm = new ToggleButton();
    toggleAlarm->setLayoutProperties(StackLayoutProperties::create().horizontal(HorizontalAlignment::Center).spaceQuota(1));

    connect(toggleAlarm, SIGNAL(checkedChanged(bool)), this, SLOT(onAlarmSwitch(bool)));
    connect(this, SIGNAL(visibleChanged(bool)), toggleAlarm, SLOT(setChecked(bool)));

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
    qDebug() << "DELETE CustomDialogAlarm";
    delete mCustomDialog;
}

void CustomDialogAlarm::setVisible(bool visible)
{
    if (mVisible == visible) {
        return;
    }

    mCustomDialog->setVisible(visible);

    mVisible = visible;
    emit visibleChanged(visible);
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
