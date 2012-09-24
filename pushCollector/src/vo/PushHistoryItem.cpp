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

#include "PushHistoryItem.hpp"

PushHistoryItem::PushHistoryItem(const QString &itemId, int seqNum)
{
    setItemId(itemId);
    setSeqNum(seqNum);
}

PushHistoryItem::~PushHistoryItem()
{

}

int PushHistoryItem::seqNum() const
{
    return m_seqNum;
}

QString PushHistoryItem::itemId() const
{
    return m_itemId;
}

void PushHistoryItem::setSeqNum(int seqNum)
{
    m_seqNum = seqNum;
}

void PushHistoryItem::setItemId(const QString &itemId)
{
    m_itemId = itemId;
}
