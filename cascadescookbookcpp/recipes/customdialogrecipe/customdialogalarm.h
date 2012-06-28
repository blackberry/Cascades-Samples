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
        class CustomDialog;
    }
}

/** 
 * CustomDialogAlarm
 * 
 * This is the CustomDialog for the CustomDialog recipe. It is a custom built
 * overlay illustrating alarm with at toggle button for dismissal.
 *
 */
class CustomDialogAlarm: public QObject
{
Q_OBJECT
Q_PROPERTY( bool visible READ visible WRITE setVisible NOTIFY visibleChanged )

public:
    CustomDialogAlarm(QObject * parent = 0);
    ~CustomDialogAlarm();

    void setVisible(bool visibility);
    bool visible();
    
private slots:

    /*
     * 
     * 
     *
     * @param onAlarmSwitch
     */
    void onAlarmSwitch(bool checked);

signals:

    void visibleChanged(bool visibility);

private:
    bool  mVisible;
    CustomDialog *mCustomDialog;
};

#endif // ifndef _CUSTOMDIALOGALARM_H_
