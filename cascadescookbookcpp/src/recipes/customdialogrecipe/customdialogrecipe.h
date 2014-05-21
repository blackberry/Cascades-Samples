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
#ifndef _CUSTOMDIALOGRECIPE_H_
#define _CUSTOMDIALOGRECIPE_H_

#include <bb/cascades/CustomControl>
#include "customdialogalarm.h"

using namespace bb::cascades;

namespace bb
{
    namespace cascades
    {
        class SequentialAnimation;
        class ImageView;
    }
}

/**
 * CustomDialogRecipe Description:
 *
 * A CustomDialog is a full-screen view that is displayed as a transparent layer on top
 * of the current screen. Basically, it's a separate view within the current application.
 * The use-case of the dialog is similar to Sheet, in that it is used when something happens
 * out-side the normal UI flow, like e.g. an alarm or message is triggered.
 * In this recipe shows how a CustomDialog is created and shown as a cozy candle triggers
 * the fire alarm.
 */
class CustomDialogRecipe: public bb::cascades::CustomControl
{
    Q_OBJECT

public:
    CustomDialogRecipe(Container * parent = 0);

private slots:

    /**
     * This Slot function is called when the flame animation is complete. So we set the dialog to visible.
     */
    void onHideAnimEnded();

    /**
     * This Slot function is called when the visibilityChanged signal is emitted. When the dialog is not
     * visible, the flame animation is shown.
     *
     * @param visible True if the dialog is to be visible, false otherwise
     */
    void onDialogVisible(bool visible);

    /**
     * Function called when the layouting of the candle flame image is ready,
     * used to be able to set the pivot point correctly depending on the size of the image.
     *
     * @param layoutRect QRect that holds the layout size of the candle light image
     */
    void flameLayoutFrameUpdated(QRectF layoutRect);

private:
    CustomDialogAlarm *mAlarmDialog;
    SequentialAnimation *mRisingFlame;
    ImageView *mFlame;
};

#endif // ifndef _CUSTOMDIALOGRECIPE_H_
