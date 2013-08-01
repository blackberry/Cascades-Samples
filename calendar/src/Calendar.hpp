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

#ifndef CALENDAR_HPP
#define CALENDAR_HPP

#include <bb/cascades/GroupDataModel>
#include <bb/pim/calendar/CalendarService>
#include <bb/pim/calendar/EventKey>

#include <QtCore/QObject>

class EventEditor;
class EventViewer;

/**
 * @short The controller class that makes access to calendar data available to the UI.
 */
//! [0]
class Calendar : public QObject
{
    Q_OBJECT

    // The model that provides the filtered list of events
    Q_PROPERTY(bb::cascades::GroupDataModel *model READ model CONSTANT);

    // The pattern to filter the list of events
    Q_PROPERTY(QString filter READ filter WRITE setFilter NOTIFY filterChanged);

    // The viewer object for the current event
    Q_PROPERTY(EventViewer* eventViewer READ eventViewer CONSTANT);

    // The editor object for the current event
    Q_PROPERTY(EventEditor* eventEditor READ eventEditor CONSTANT);

public:
    Calendar(QObject *parent = 0);

public Q_SLOTS:
    /**
     * Marks the event with the given @p indexPath as current.
     */
    void setCurrentEvent(const QVariantList &indexPath);

    /**
     * Prepares the event editor to create a new event.
     */
    void createEvent();

    /**
     * Prepares the event editor to edit the current event.
     */
    void editEvent();

    /**
     * Prepares the event viewer to display the current event.
     */
    void viewEvent();

    /**
     * Deletes the current event.
     */
    void deleteEvent();

Q_SIGNALS:
    // The change notification signal for the property
    void filterChanged();

private Q_SLOTS:
    // Filters the events in the model according to the filter property
    void filterEvents();

private:
    // The accessor methods of the properties
    bb::cascades::GroupDataModel* model() const;
    QString filter() const;
    void setFilter(const QString &filter);
    EventViewer* eventViewer() const;
    EventEditor* eventEditor() const;

    // The property values
    bb::cascades::GroupDataModel* m_model;
    QString m_filter;

    // The central object to access the calendar service
    bb::pim::calendar::CalendarService* m_calendarService;

    // The controller object for viewing an event
    EventViewer* m_eventViewer;

    // The controller object for editing an event
    EventEditor* m_eventEditor;

    // The ID of the current event
    bb::pim::calendar::EventKey m_currentEventKey;

    // The time range for event lookups (based on the filter criterion)
    QDateTime m_searchStartTime;
    QDateTime m_searchEndTime;
};
//! [0]

#endif
