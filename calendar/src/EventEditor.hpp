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

#ifndef EVENTEDITOR_HPP
#define EVENTEDITOR_HPP

#include <bb/cascades/DropDown>
#include <bb/pim/calendar/CalendarService>
#include <bb/pim/calendar/EventKey>

#include <QtCore/QObject>

/**
 * @short A controller object to encapsulate the logic of the event editor
 *
 * The EventEditor class is an UI-independent representation of the event editor,
 * that provides all the functionality and data as slots and properties.
 * It encapsulates all the logic of creating a new event or updating an existing
 * one.
 */
//! [0]
class EventEditor : public QObject
{
    Q_OBJECT

    // The data properties of the event that is created or updated
    Q_PROPERTY(QString subject READ subject WRITE setSubject NOTIFY subjectChanged)
    Q_PROPERTY(QString location READ location WRITE setLocation NOTIFY locationChanged)
    Q_PROPERTY(QDateTime startTime READ startTime WRITE setStartTime NOTIFY startTimeChanged)
    Q_PROPERTY(QDateTime endTime READ endTime WRITE setEndTime NOTIFY endTimeChanged)
    Q_PROPERTY(int folderId READ folderId WRITE setFolderId NOTIFY folderIdChanged)
    Q_PROPERTY(int accountId READ accountId WRITE setAccountId NOTIFY accountIdChanged)

    // Defines whether the editor is in 'create' or 'edit' mode
    Q_PROPERTY(Mode mode READ mode WRITE setMode NOTIFY modeChanged)

    Q_ENUMS(Mode)


public:
    /**
     * Describes the mode of the event editor.
     * The mode information are used to adapt the behavior of the editor and
     * provide hints to the UI.
     */
    enum Mode {
        CreateMode,
        EditMode
    };

    EventEditor(bb::pim::calendar::CalendarService *service, QObject *parent = 0);

    void setMode(Mode mode);
    Mode mode() const;

    /**
     * This method fills the given DropDown control with Option objects that represent
     * the available calendar folders.
     */
    Q_INVOKABLE void initializeFolderDropDown(bb::cascades::DropDown *downDown);

public Q_SLOTS:
    /**
     * Loads the event with the given key.
     */
    void loadEvent(const bb::pim::calendar::EventKey &eventKey);

    /**
     * Save the currently loaded event if in 'edit' mode or creates a new one
     * if in 'create' mode.
     */
    void saveEvent();

    /**
     * Resets all fields of the event editor.
     */
    void reset();

Q_SIGNALS:
    // The change notification signals of the properties
    void subjectChanged();
    void locationChanged();
    void startTimeChanged();
    void endTimeChanged();
    void folderIdChanged();
    void accountIdChanged();
    void modeChanged();

private:
    // The accessor methods of the properties
    void setSubject(const QString &subject);
    QString subject() const;
    void setLocation(const QString &location);
    QString location() const;
    void setStartTime(const QDateTime &startTime);
    QDateTime startTime() const;
    void setEndTime(const QDateTime &endTime);
    QDateTime endTime() const;
    void setFolderId(int folderId);
    int folderId() const;
    void setAccountId(int accountId);
    int accountId() const;

    // The central object to access the calendar service
    bb::pim::calendar::CalendarService* m_calendarService;

    // The ID of the currently loaded event (if in 'edit' mode)
    bb::pim::calendar::EventKey m_eventKey;

    // The property values
    QString m_subject;
    QString m_location;
    QDateTime m_startTime;
    QDateTime m_endTime;
    int m_folderId;
    int m_accountId;

    Mode m_mode;
};
//! [0]

#endif
