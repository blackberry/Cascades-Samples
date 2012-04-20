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
#ifndef _STAMPCOLLECTORAPP_
#define _STAMPCOLLECTORAPP_

#include <bb/cascades/Application>
#include <bb/data/JsonDataAccess>

using namespace bb::cascades;
using namespace bb::data;

namespace bb
{
    namespace cascades
    {
        class GroupDataModel;
        class ListView;
        class NavigationPane;
        class Page;
    }
}

class QDeclarativeContext;

/**
 * StampCollector
 * 
 * A philatelist collection of stamps is used to illustrate how to create a simple list, 
 * and trigger changes in the UI on selection. List control signals are used to listen 
 * for item clicked events.
 * 
 * You will learn how to:
 * - Set up a ListView with customized item controls
 * - Populate a List Control to a JSON and XML data model 
 * - Perform list selection event handling
 * - Handle list item focus/unfocus events
 */
class StampCollectorApp: public QObject
{
Q_OBJECT

public:
    StampCollectorApp();

private slots:

    /**
     * Slot function for selection in list.
     *
     * @param indexPath Index path to the item.
     * @param active True if the new state is selected, otherwise false.
     */
    void onSelectionChanged(const QVariantList indexPath, bool selected);

private:

    /**
     * Function that initializes the stamp list.
     */
    void setUpStampListModel(ListView *stampList);

    // Model for the stamp list.
    GroupDataModel *mStampModel;

    // The declarative context, used to bind properties so they are available in QML.
    QDeclarativeContext *mQmlContext;

    // The Content Page used for presenting a detailed view for the selected stamp.
    Page *mContentPage;

    // A NavigationPane is used to create the drill down UI.
    NavigationPane *mNav;
};

#endif /// ifndef _STAMPCOLLECTORAPP_
