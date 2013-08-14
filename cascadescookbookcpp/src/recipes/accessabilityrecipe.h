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

#ifndef _ACCESSABILITY_H_
#define _ACCESSABILITY_H_

#include <bb/cascades/CustomControl>
#include <bb/cascades/Label>

using namespace bb::cascades;

namespace bb
{
    namespace cascades
    {
        class Container;
    }
}

/**
 * Accessibility recipe Description:
 *
 * The application introduction recipe. Sets up a view with some text
 * explaining the purpose of the recipe and a small example UI
 * letting you know how to use the screen reader.
 *
 */
class AccessabilityRecipe: public bb::cascades::CustomControl
{
Q_OBJECT

public:

    /**
     * Constructor; sets up the Accessibility recipe.
     * @param parent The parent Container.
     */
    AccessabilityRecipe(Container *parent = 0);

public slots:
    /* Slot function for the segmented control
     * @param selectedIndex The index of the currently selected index in the list of options.
      */
    void onSegmentedIndexChanged(int selectedIndex);

private:
    // Labels handles that we change when SegmentedControl
    Label *mMuschroomLabel;
    Label *mCreamLabel;
};

#endif // ifndef _ACCESSABILITY_H_
