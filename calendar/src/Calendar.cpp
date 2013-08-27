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

#include "Calendar.hpp"

#include "EventEditor.hpp"
#include "EventViewer.hpp"

#include <bb/pim/calendar/CalendarEvent>
#include <bb/pim/calendar/CalendarFolder>
#include <bb/pim/calendar/EventKey>
#include <bb/pim/calendar/EventSearchParameters>

using namespace bb::cascades;
using namespace bb::pim::calendar;

Q_DECLARE_METATYPE(bb::pim::calendar::CalendarEvent)

//! [0]
Calendar::Calendar(QObject *parent)
    : QObject(parent)
    , m_model(new GroupDataModel(this))
    , m_calendarService(new CalendarService())
    , m_eventViewer(new EventViewer(m_calendarService, this))
    , m_eventEditor(new EventEditor(m_calendarService, this))
{
    // Disable grouping in data model
    m_model->setGrouping(ItemGrouping::None);

    // Ensure to invoke the filterEvents() method whenever an event has been added, changed or removed
    bool ok = connect(m_calendarService, SIGNAL(eventsRefreshed(bb::pim::calendar::EventRefresh)), SLOT(filterEvents()));
    Q_ASSERT(ok);
    Q_UNUSED(ok);

    // Fill the data model with events initially
    setFilter("today");
    filterEvents();
}
//! [0]

//! [1]
void Calendar::setCurrentEvent(const QVariantList &indexPath)
{
    if (indexPath.isEmpty()) {
        m_currentEventKey = EventKey();
    } else {
        const QVariantMap entry = m_model->data(indexPath).toMap();
        m_currentEventKey.setEventId(entry.value("eventId").toInt());
        m_currentEventKey.setAccountId(entry.value("accountId").toInt());
    }
}
//! [1]

//! [2]
void Calendar::createEvent()
{
    // Prepare the event editor for creating a new event
    m_eventEditor->reset();
    m_eventEditor->setMode(EventEditor::CreateMode);
}
//! [2]

//! [3]
void Calendar::editEvent()
{
    // Prepare the event editor for editing the current event
    m_eventEditor->loadEvent(m_currentEventKey);
    m_eventEditor->setMode(EventEditor::EditMode);
}
//! [3]

//! [4]
void Calendar::viewEvent()
{
    // Prepare the event viewer for displaying the current event
    m_eventViewer->setEventKey(m_currentEventKey);
}
//! [4]

//! [5]
void Calendar::deleteEvent()
{
    // Fetch the event object that we can pass to deleteEvent()
    const CalendarEvent event = m_calendarService->event(m_currentEventKey.accountId(), m_currentEventKey.eventId());

    m_calendarService->deleteEvent(event);
}
//! [5]

bb::cascades::GroupDataModel* Calendar::model() const
{
    return m_model;
}

QString Calendar::filter() const
{
    return m_filter;
}

//! [6]
void Calendar::setFilter(const QString &filter)
{
    if (m_filter == filter)
        return;

    m_filter = filter;
    emit filterChanged();

    // Calculate the search range depending on the filter input
    const QDate today = QDate::currentDate();
    const QTime midnight(0, 0, 0);

    if (m_filter == QLatin1String("today")) {
        m_searchStartTime = QDateTime(today, midnight);
        m_searchEndTime = QDateTime(today.addDays(1), midnight);
    } else if (m_filter == QLatin1String("week")) {
        const QDate thisMonday = today.addDays(-(today.dayOfWeek() - 1));
        m_searchStartTime = QDateTime(thisMonday, midnight);
        m_searchEndTime = QDateTime(thisMonday.addDays(7), midnight);
    } else if (m_filter == QLatin1String("month")) {
        const QDate firstDayInMonth = QDate(today.year(), today.month(), 1);
        m_searchStartTime = QDateTime(firstDayInMonth, midnight);
        m_searchEndTime = QDateTime(firstDayInMonth.addDays(firstDayInMonth.daysInMonth()), midnight);
    }

    // Update the model now that the filter criterion has changed
    filterEvents();
}
//! [6]

EventViewer* Calendar::eventViewer() const
{
    return m_eventViewer;
}

EventEditor* Calendar::eventEditor() const
{
    return m_eventEditor;
}

//! [7]
void Calendar::filterEvents()
{
    // Setup the search parameters with time range as specified by filter criterion
    EventSearchParameters searchParameters;
    searchParameters.setStart(m_searchStartTime);
    searchParameters.setEnd(m_searchEndTime);
    searchParameters.setDetails(DetailLevel::Weekly);

    const QList<CalendarEvent> events = m_calendarService->events(searchParameters);

    // Clear the old events information from the model
    m_model->clear();

    // Iterate over the list of events
    foreach (const CalendarEvent &event, events) {
        // Copy the data into a model entry
        QVariantMap entry;
        entry["eventId"] = event.id();
        entry["accountId"] = event.accountId();
        entry["subject"] = event.subject();
        entry["startTime"] = event.startTime().toString(Qt::DefaultLocaleShortDate);
        entry["endTime"] = event.endTime().toString(Qt::DefaultLocaleShortDate);

        // Add the entry to the model
        m_model->insert(entry);
    }
}
//! [7]
