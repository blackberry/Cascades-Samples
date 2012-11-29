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
#ifndef _DATETIMEPICKERRECIPE_H_
#define _DATETIMEPICKERRECIPE_H_

#include <bb/cascades/CustomControl>

using namespace bb::cascades;

namespace bb
{
    namespace cascades
    {
        class ImageView;
    }
}

/* DateTimePickerRecipe Description:
 * 
 * In this recipe the DateTimePicker Control is introduced. 
 * By setting different dates in the past, present and future
 * one can time warp the ripening and decay of a banana.
 */
class DateTimePickerRecipe: public bb::cascades::CustomControl
{
    Q_OBJECT

public:

    /**
     * Constructor; sets up the DateTimePicker control recipe.
     * @param parent A pointer to the parent Container.
     */
    DateTimePickerRecipe(Container *parent = 0);

private slots:
    /**
     * This Slot function is for QDateTime update signals of the DateTimePicker.
     *
     * @param value The new value of the date and time.
     */
    void onValueChanged(QDateTime value);

private:
    // An Image is used to illustrate how the DateTimePicker values
    ImageView *mTimeWarpFruit;
};

#endif // ifndef _DATETIMEPICKERRECIPE_H_
