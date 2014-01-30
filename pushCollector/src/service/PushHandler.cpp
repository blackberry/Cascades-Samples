/*!
* Copyright (c) 2012, 2013  BlackBerry Limited.
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

#include "PushHandler.hpp"
#include <QDebug>

PushHandler::PushHandler()
{
}

PushHandler::~PushHandler()
{
}

bool PushHandler::checkForDuplicate(const PushHistoryItem &pushHistoryItem)
{
    if (pushHistoryItem.itemId().isEmpty()) {
        return false;
    }

    if (!m_pushHistoryDAO.createPushHistoryTable()) {
        return false;
    }

    const PushHistoryItem storedPushHistoryItem = m_pushHistoryDAO.pushHistoryItem(pushHistoryItem.itemId());

    if (storedPushHistoryItem.seqNum() > -1) {
        return true;
    }

    if (!m_pushHistoryDAO.add(pushHistoryItem)) {
        return false;
    }

    if (m_pushHistoryDAO.pushHistoryCount() > 10) {
        m_pushHistoryDAO.removeOldest();
    }

    return false;
}

int PushHandler::save(const Push &push)
{
    if (!m_pushDAO.createPushTable()) {
        return -1;
    }

    return m_pushDAO.add(push);
}

Push PushHandler::push(int pushSeqNum)
{
    return m_pushDAO.push(pushSeqNum);
}

QVariantList PushHandler::pushes()
{
    return m_pushDAO.pushes();
}

bool PushHandler::remove(int pushSeqNum)
{
    return m_pushDAO.remove(pushSeqNum);
}

bool PushHandler::removeAll()
{
    return m_pushDAO.removeAll();
}

bool PushHandler::removeAllPushHistory()
{
    return m_pushHistoryDAO.removeAll();
}

bool PushHandler::markAsRead(int pushSeqNum)
{
    return m_pushDAO.markAsRead(pushSeqNum);
}

bool PushHandler::markAllAsRead()
{
    return m_pushDAO.markAllAsRead();
}
