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

using namespace bb::cascades;

namespace bb
{
    namespace cascades
    {
        class ListView;
    }
}

/**
 * StampCollector Description:
 * 
 * A philatelist collection of stamps is used to illustrate how to create a simple list, 
 * and trigger changes in the UI on selection. List control signals are used to listen 
 * for item clicked events.
 * 
 * You will learn how to:
 * - Set up a ListView with customized item controls
 * - Populate a List Control to a JSON and XML data model 
 * - Perform list selection event handling
 */
class StampCollectorApp: public QObject
{
    Q_OBJECT

public:
    // This is our constructor that sets up the recipe.
    StampCollectorApp();

private:

    /**
     * This function initializes the stamp list.
     *
     * @param stampList The list of stamps
     */
    void setUpStampListModel(ListView *stampList);
};

#endif /// ifndef _STAMPCOLLECTORAPP_
