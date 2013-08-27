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

#include "EventViewer.hpp"

#include <bb/pim/calendar/CalendarEvent>
#include <bb/pim/calendar/EventRefresh>
#include <bb/pim/calendar/EventSearchParameters>

using namespace bb::pim::calendar;

//! [0]
EventViewer::EventViewer(CalendarService *service, QObject *parent)
    : QObject(parent)
    , m_calendarService(service)
{
    // Ensure to invoke the eventsChanged() method whenever an event has been changed
    bool ok = connect(m_calendarService, SIGNAL(eventsRefreshed(bb::pim::calendar::EventRefresh)), SLOT(eventsChanged(bb::pim::calendar::EventRefresh)));
    Q_ASSERT(ok);
    Q_UNUSED(ok);

}
//! [0]

//! [3]
void EventViewer::setEventKey(const EventKey &eventKey)
{
    if (m_eventKey.eventId() == eventKey.eventId() &&
        m_eventKey.accountId() == eventKey.accountId())
        return;

    m_eventKey = eventKey;

    // Trigger a refetch of the event for the new ID
    updateEvent();
}
//! [3]

//! [2]
void EventViewer::eventsChanged(const EventRefresh &refresh)
{
    /**
     * Call updateEvent() only if the event we are currently displaying
     * has been changed.
     */
    if (refresh.account() == m_eventKey.accountId()) {
        if (refresh.updatedEventIds().contains(m_eventKey.eventId()))
            updateEvent();
    }
}
//! [2]

QString EventViewer::subject() const
{
    return m_subject;
}

QString EventViewer::location() const
{
    return m_location;
}

QString EventViewer::startTime() const
{
    if (!m_startTime.isValid())
        return tr("n/a");
    else
        return m_startTime.toString();
}

QString EventViewer::endTime() const
{
    if (!m_endTime.isValid())
        return tr("n/a");
    else
        return m_endTime.toString();
}

//! [1]
void EventViewer::updateEvent()
{
    // Store previous values
    const QString oldSubject = m_subject;
    const QString oldLocation = m_location;
    const QDateTime oldStartTime = m_startTime;
    const QDateTime oldEndTime = m_endTime;

    // Fetch new values from persistent storage
    const CalendarEvent event = m_calendarService->event(m_eventKey.accountId(), m_eventKey.eventId());

    m_subject = event.subject();
    m_location = event.location();
    m_startTime = event.startTime();
    m_endTime = event.endTime();

    // Check whether values have changed
    if (oldSubject != m_subject)
        emit subjectChanged();

    if (oldLocation != m_location)
        emit locationChanged();

    if (oldStartTime != m_startTime)
        emit startTimeChanged();

    if (oldEndTime != m_endTime)
        emit endTimeChanged();
}
//! [1]
