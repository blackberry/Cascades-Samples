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

#ifndef PUSHHISTORYITEM_HPP
#define PUSHHISTORYITEM_HPP

#include <QString>

/*!
 * Value object relating to an item in the push history.
 * The push history is used to check for potential duplicate pushes being sent.
 * Any duplicate pushes that are detected will be discarded and not displayed to the user.
 */
class PushHistoryItem
{
public:
    PushHistoryItem(const QString &itemId, int seqNum = -1);
    virtual ~PushHistoryItem();

    int seqNum() const;
    QString itemId() const;

    void setSeqNum(int seqNum);
    void setItemId(const QString &itemId);

private:
    // The unique id of the push history item (to identify it in the database)
    int m_seqNum;

    // For our implementation, this will match the "Push-Message-ID" header that
    // comes back with each push
    // If the header is missing, then we assume that there is no duplicate
    QString m_itemId;
};

#endif
