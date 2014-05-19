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
#ifndef _DOCKLAYOUTRECIPE_H_
#define _DOCKLAYOUTRECIPE_H_

#include <bb/cascades/CustomControl>

using namespace bb::cascades;

namespace bb
{
    namespace cascades
    {
        class Label;
        class ImageView;
        class Container;
    }
}

/**
 * DockLayoutRecipe Description:
 *
 * This recipe shows how an object can be aligned in a DockLayout.
 *
 */
class DockLayoutRecipe: public bb::cascades::CustomControl
{
    Q_OBJECT

public:
    /**
     * Constructor; sets up the dock layout recipe.
     * @param parent A pointer to the parent Container, @c if not specified, 0 is used
     */
    DockLayoutRecipe(Container *parent = 0);

private slots:

    /**
     * This Slot function is called when the touch event occurs on the main Container.
     * When called, the function alters the layout properties of the images in the UI to different layouts.
     * @param event The touch event
     */
    void onTouch(bb::cascades::TouchEvent *event);

private:
    // Action Label
    Label *mActionLabel;

    // Controls that will be layouted using the DockLayout property
    ImageView *mBottomBean;
    ImageView *mRightBean;
    ImageView *mLeftBean;
    ImageView *mTopBean;
};

#endif // ifndef _DOCKLAYOUTRECIPE_H_
