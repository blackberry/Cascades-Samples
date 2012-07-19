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
 * CascadesCookbookApp
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
     * Slot function for selection in list.
     *
     * @param indexPath Index path to the item.
     */
    void onTriggered(const QVariantList indexPath);

    /**
     * Slot function that receives signals, when the top Control has changed in the
     * NavigationPane.
     */
    void onTopChanged(bb::cascades::Page* page);

private:
    /**
     * Function that initializes the content page.
     */
    Page *createContentPage();

    /**
     * Function that initializes the recipe list page.
     */
    Page *createRecipePage();

    /**
     * Function that initializes the recipe list.
     */
    ListView *createRecipeListView();

    // Model for the recipe list.
    bb::cascades::QVariantListDataModel mRecipeModel;

    // A navigation pane is used to navigate between the list and the actual recipes.
    NavigationPane *mNavPane;

    // We create a page with the control as content.
    Page *mContentPage;

    // The recipe list.
    ListView *mRecipeListView;
};

#endif // ifndef _CASCADESCOOKBOOKAPP_H_
