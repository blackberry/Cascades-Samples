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

#ifndef _CASCADESCOOKBOOKAPP_H_
#define _CASCADESCOOKBOOKAPP_H_

#include <bb/cascades/Application>
#include <bb/cascades/QListDataModel>

using namespace bb::cascades;

namespace bb
{
    namespace cascades
    {
        class Container;
        class ListView;
        class NavigationPane;
        class Page;
    }
}

/**
 * CascadesCookbookApp Description:
 *
 * A kitchen sink style demo with all available UI controls and settings for those controls.
 *
 */
class CascadesCookbookApp: public QObject
{
    Q_OBJECT

public:
    CascadesCookbookApp();
    ~CascadesCookbookApp();

private slots:

    /**
     * This Slot function for selection in list.
     *
     * @param indexPath Index path to the item
     */
    void onTriggered(const QVariantList indexPath);

    /**
     * This Slot function receives signals when the top Control has changed in the
     * NavigationPane.
     *
     * @param page Top Container to be changed to
     */
    void onTopChanged(bb::cascades::Page* page);

    /**
     * This Signal handler function for when navigation ends in the NavigationPane.
     *
     * @param page The main content Container we are transitioning away from
     */
    void onPopTransitionEnded(bb::cascades::Page *page);

    /**
     * This Slot function receives signals when an action from the menu have been triggered
     *
     */
    void onActionTriggerd();

private:
    /**
     * This function initializes the content page
     *
     * @return The created content Page
     */
    Page *createContentPage();

    /**
     * This function initializes the recipe list page
     *
     * @return The created recipe Page
     */
    Page *createRecipePage();

    /**
     * This function initializes the ListView recipe list
     *
     * @return The ListView of the created recipe list
     */
    ListView *createRecipeListView();

    /**
     * Function used to create and set the SceneCover of the application,
     * the cover is shown when the app is running in minimized mode.
     */
    void addApplicationCover();

    // Model for the recipe list
    bb::cascades::QVariantListDataModel mRecipeModel;

    // A navigation pane is used to navigate between the list and the actual recipes
    NavigationPane *mNavPane;

    // Create a page with the control as content
    Page *mContentPage;

    // The recipe list
    ListView *mRecipeListView;
};

#endif // ifndef _CASCADESCOOKBOOKAPP_H_
