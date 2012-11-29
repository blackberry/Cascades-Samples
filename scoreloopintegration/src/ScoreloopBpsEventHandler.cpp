/*
* Copyright (c) 2012 Research In Motion Limited.
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

#include "ScoreloopBpsEventHandler.hpp"

//! [0]
ScoreloopBpsEventHandler::ScoreloopBpsEventHandler(SC_InitData_t initData)
    : m_initData(initData)
{
    /**
     * Subscribe to the Scoreloop event domain.
     *
     * Note: This only works after SC_Client_New was called
     */
    subscribe(SC_GetBPSEventDomain(&m_initData));
}
//! [0]

//! [1]
void ScoreloopBpsEventHandler::event(bps_event_t *event)
{
    // Pass the event to the Scoreloop event handler
    SC_HandleBPSEvent(&m_initData, event);
}
//! [1]
