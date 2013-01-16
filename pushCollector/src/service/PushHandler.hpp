/*!
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

#ifndef PUSHHANDLER_HPP
#define PUSHHANDLER_HPP

#include "../dao/PushDAO.hpp"
#include "../dao/PushHistoryDAO.hpp"

class PushHandler
{
public:
    PushHandler();
    virtual ~PushHandler();

    /*!
     * Checks if the specified push has already been received
     * @param pushHistoryItem
     */
    bool checkForDuplicate(const PushHistoryItem &pushHistoryItem);

    /*!
     * Saves the specified push to the persistent store
     * @param push the push that will be saved
     */
    int save(const Push &push);

    /*!
     * Retrieves the specified push from the persistent store
     * @param pushSeqNum the sequence number of the push that will be retrieved
     */
    Push push(int pushSeqNum);

    /*!
     * Retrieves all pushes from the persistent store
     */
    QVariantList pushes();

    /*!
     * Removes the specified push from the persistent store
     * @param pushSeqNum the sequence number of the push that will be deleted
     */
    bool remove(int pushSeqNum);

    /*!
     * Removes all pushes from the persistent store
     */
    bool removeAll();

    /*
     * Removes all push history from the persistent store
     */
    bool removeAllPushHistory();

    /*!
     * Marks the specified push as read
     * @param pushSeqNum the sequence number of the push that will be marked as read
     */
    bool markAsRead(int pushSeqNum);

    /*!
     * Marks all pushes as read
     */
    bool markAllAsRead();

private:
    PushDAO m_pushDAO;
    PushHistoryDAO m_pushHistoryDAO;
};

#endif
