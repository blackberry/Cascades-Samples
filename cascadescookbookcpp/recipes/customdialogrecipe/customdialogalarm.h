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
#ifndef _CUSTOMDIALOGALARM_H_
#define _CUSTOMDIALOGALARM_H_

#include <bb/cascades/CustomControl>

using namespace bb::cascades;

namespace bb
{
    namespace cascades
    {
        class Dialog;
    }
}

/** 
 * CustomDialogAlarm Description:
 * 
 * This is the CustomDialog class for the CustomDialog recipe. It is custom built to
 * overlay, simulating an alarm with at toggle button for dismissal.
 *
 */
class CustomDialogAlarm: public QObject
{
    Q_OBJECT
    Q_PROPERTY( bool visible READ visible WRITE setVisible NOTIFY visibleChanged )

public:

    /**
     * Constructor that sets up the custom dialog with a main content Container and a dialog Container
     * containing a background image, dialog content Container, a title label, a text label, and toggle
     * button for the alarm. We connect the checkedChanged signal for the toggleAlarm to call onAlarmSet
     * slot function and connect changes the visibility of the custom dialog to the visibilityChanged
     * function.
     *
     * @param parent The parent object, if not specified, @c is 0
     *
     */
    CustomDialogAlarm(QObject * parent = 0);
    ~CustomDialogAlarm();

    /**
     * This mutator function sets the visibility of the custom dialog
     *
     * @param visibility True is visible, false if not visible
     */
    void setVisible(bool visibility);

    /**
     * This accessor function returns the visibility state of the custom dialog
     *
     * @return True if visible, false if not visible
     */
    bool visible();

private slots:

    /**
     * This slot function is called when the toggle alarm state has been changed (checkedChanged
     * signal). On = true, Off = false
     *
     * @param checked True if alarm is on, false if alarm is off
     */
    void onAlarmSwitch(bool checked);

    signals:

    /**
     * After setting our Q_PROPERTY for visible when a change in the visibility state occurs,
     * this function is called.
     *
     * @param visibility The boolean value to be changed to; true if visible, false if not visible
     */
    void visibleChanged(bool visibility);

private:
    // Whether the custom dialog is visible or not
    bool mVisible;

    // The custom dialog
    Dialog *mCustomDialog;
};

#endif // ifndef _CUSTOMDIALOGALARM_H_
