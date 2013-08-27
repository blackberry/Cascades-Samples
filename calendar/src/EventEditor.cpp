/* Copyright (c) 2012, 2013  BlackBerry Limited.
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

#include "EventEditor.hpp"

#include <bb/cascades/Option>
#include <bb/pim/calendar/CalendarEvent>
#include <bb/pim/calendar/CalendarFolder>
#include <bb/pim/calendar/EventSearchParameters>

using namespace bb::cascades;
using namespace bb::pim::calendar;

//! [1]
EventEditor::EventEditor(CalendarService *service, QObject *parent)
    : QObject(parent)
    , m_calendarService(service)
    , m_mode(CreateMode)
{
}
//! [1]

//! [0]
void EventEditor::initializeFolderDropDown(DropDown *dropDown)
{
    if (!dropDown)
        return;

    dropDown->removeAll();

    bool firstEntry = true;
    foreach (const CalendarFolder &folder, m_calendarService->folders()) {
        if (folder.isReadOnly())
            continue;

        Option *option = new Option();
        option->setText(folder.name());

        QVariantMap value;
        value["folderId"] = folder.id();
        value["accountId"] = folder.accountId();
        option->setValue(value);

        // Pre-select the first entry
        if (firstEntry) {
            firstEntry = false;
            option->setSelected(true);
        }

        dropDown->add(option);
    }
}
//! [0]

//! [2]
void EventEditor::loadEvent(const EventKey &eventKey)
{
    m_eventKey = eventKey;

    // Load the event from the persistent storage
    const CalendarEvent event = m_calendarService->event(m_eventKey.accountId(), m_eventKey.eventId());

    // Update the properties with the data from the event
    m_subject = event.subject();
    m_location = event.location();
    m_startTime = event.startTime();
    m_endTime = event.endTime();
    m_folderId = event.folderId();
    m_accountId = event.accountId();

    // Emit the change notifications
    emit subjectChanged();
    emit locationChanged();
    emit startTimeChanged();
    emit endTimeChanged();
    emit folderIdChanged();
    emit accountIdChanged();
}
//! [2]

//! [3]
void EventEditor::saveEvent()
{
    if (m_mode == CreateMode) {
        // Create a new event object
        CalendarEvent event;
        event.setAccountId(m_accountId);
        event.setFolderId(m_folderId);
        event.setSubject(m_subject);
        event.setLocation(m_location);
        event.setStartTime(m_startTime);
        event.setEndTime(m_endTime);

        // Save the event to persistent storage
        m_calendarService->createEvent(event);
    } else if (m_mode == EditMode) {
        // Load the event from persistent storage
        CalendarEvent event = m_calendarService->event(m_eventKey.accountId(), m_eventKey.eventId());

        event.setSubject(m_subject);
        event.setLocation(m_location);
        event.setStartTime(m_startTime);
        event.setEndTime(m_endTime);

        // Save the updated event back to persistent storage
        m_calendarService->updateEvent(event);
    }
}
//! [3]

//! [4]
void EventEditor::reset()
{
    // Reset all properties
    m_subject.clear();
    m_location.clear();
    m_startTime = QDateTime::currentDateTime();
    m_endTime = QDateTime::currentDateTime();
    m_folderId = -1;
    m_accountId = -1;

    // Emit the change notifications
    emit subjectChanged();
    emit locationChanged();
    emit startTimeChanged();
    emit endTimeChanged();
    emit folderIdChanged();
    emit accountIdChanged();
}
//! [4]

void EventEditor::setSubject(const QString &subject)
{
    if (m_subject == subject)
        return;

    m_subject = subject;
    emit subjectChanged();
}

QString EventEditor::subject() const
{
    return m_subject;
}

void EventEditor::setLocation(const QString &location)
{
    if (m_location == location)
        return;

    m_location = location;
    emit locationChanged();
}
QString EventEditor::location() const
{
    return m_location;
}

void EventEditor::setStartTime(const QDateTime &startTime)
{
    if (m_startTime == startTime)
        return;

    m_startTime = startTime;
    emit startTimeChanged();
}

QDateTime EventEditor::startTime() const
{
    return m_startTime;
}

void EventEditor::setEndTime(const QDateTime &endTime)
{
    if (m_endTime == endTime)
        return;

    m_endTime = endTime;
    emit endTimeChanged();
}

QDateTime EventEditor::endTime() const
{
    return m_endTime;
}

void EventEditor::setFolderId(int folderId)
{
    if (m_folderId == folderId)
        return;

    m_folderId = folderId;
    emit folderIdChanged();
}

int EventEditor::folderId() const
{
    return m_folderId;
}

void EventEditor::setAccountId(int accountId)
{
    if (m_accountId == accountId)
        return;

    m_accountId = accountId;
    emit accountIdChanged();
}

int EventEditor::accountId() const
{
    return m_accountId;
}

void EventEditor::setMode(Mode mode)
{
    if (m_mode == mode)
        return;

    m_mode = mode;
    emit modeChanged();
}

EventEditor::Mode EventEditor::mode() const
{
    return m_mode;
}
