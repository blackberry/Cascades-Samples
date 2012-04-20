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

#ifndef _PROGRESSINDICATORRECIPE_H_
#define _PROGRESSINDICATORRECIPE_H_

#include <bb/cascades/CustomControl>


using namespace bb::cascades;

namespace bb
{
    namespace cascades
    {
        class Label;
        class ProgressIndicator;
        class Button;
        
    }
}

/* ProgressIndicatorRecipe
 * 
 * Simple ProgressIndicator recipe shows how a ProgressIndicator
 * control can be added and the different states the ProgressIndicator can have.
 * 
 */
class ProgressIndicatorRecipe: public bb::cascades::CustomControl
{
Q_OBJECT

public:
    ProgressIndicatorRecipe(Container *parent = 0)
;
public slots:
/**
 * Slot function connected to the ProgressIndicator, 
 *
 * @param value the current ProgressIndicator value.
 */
void onValueChanged(float value);
    void onClicked();

private:
ProgressIndicator *mProgressIndicator;
    Button *mButton;
};

#endif // ifndef _PROGRESSINDICATORRECIPE_H_
