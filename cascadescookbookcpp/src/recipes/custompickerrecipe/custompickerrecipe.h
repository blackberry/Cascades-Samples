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
#ifndef _CUSTOMPICKERRECIPE_H_
#define _CUSTOMPICKERRECIPE_H_

#include <bb/cascades/CustomControl>

using namespace bb::cascades;

namespace bb
{
    namespace cascades
    {
        class Label;
        class Picker;
    }
}


/**
 * CustomPickerRecipe Description:
 * 
 * In this recipe you can see how to set up your own Picker Control.
 * It is shown how one can use an XML model to populate a Picker with
 * customized item controls.
 */
class CustomPickerRecipe: public bb::cascades::CustomControl
{
    Q_OBJECT

public:

    /**
     * Constructor; sets up the CustomPicker control recipe.
     * @param parent A pointer to the parent Container.
     */
    CustomPickerRecipe(Container *parent = 0);

private slots:
    /**
     * The pickers value changed signal is connected to this function
     * in which the selected value is used to set up a label text based
     * on the current selection in the Picker.
     *
     * @param selectedValue the current selection in the picker
     */
    void onValueChanged(const QVariant &selectedValue);

private:
    Picker *mPicker;
    Label *mDescription;
    Label *mBill;

};

#endif // ifndef _CUSTOMPICKERRECIPE_H_
