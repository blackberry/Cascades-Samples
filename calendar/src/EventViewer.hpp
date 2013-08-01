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

#ifndef EVENTVIEWER_HPP
#define EVENTVIEWER_HPP

#include <bb/pim/calendar/CalendarService>
#include <bb/pim/calendar/EventKey>

#include <QtCore/QObject>

/**
 * @short A controller object to encapsulate the logic of the event viewer
 *
 * The EventViewer class is an UI-independent representation of the event viewer,
 * that provides all the functionality and data as slots and properties.
 * It encapsulates all the logic of loading an event from the persistent storage,
 * provide its data as properties and update the properties if the event has
 * changed in the storage backend.
 */
//! [0]
class EventViewer : public QObject
{
    Q_OBJECT

    // The data properties of the event that is displayed
    Q_PROPERTY(QString subject READ subject NOTIFY subjectChanged)
    Q_PROPERTY(QString location READ location NOTIFY locationChanged)
    Q_PROPERTY(QString startTime READ startTime NOTIFY startTimeChanged)
    Q_PROPERTY(QString endTime READ endTime NOTIFY endTimeChanged)

public:
    EventViewer(bb::pim::calendar::CalendarService *service, QObject *parent = 0);

    // Sets the ID of the event that should be displayed.
    void setEventKey(const bb::pim::calendar::EventKey &eventKey);

Q_SIGNALS:
    // The change notification signals of the properties
    void subjectChanged();
    void locationChanged();
    void startTimeChanged();
    void endTimeChanged();

private Q_SLOTS:
    /**
     * This slot is invoked whenever the calendar service reports that an event has been changed.
     */
    void eventsChanged(const bb::pim::calendar::EventRefresh&);

private:
    // The accessor methods of the properties
    QString subject() const;
    QString location() const;
    QString startTime() const;
    QString endTime() const;

    // Loads the event from the persistent storage and updates the properties
    void updateEvent();

    // The central object to access the calendar service
    bb::pim::calendar::CalendarService* m_calendarService;

    // The ID of the event that is displayed
    bb::pim::calendar::EventKey m_eventKey;

    // The property values
    QString m_subject;
    QString m_location;
    QDateTime m_startTime;
    QDateTime m_endTime;
};
//! [0]

#endif
