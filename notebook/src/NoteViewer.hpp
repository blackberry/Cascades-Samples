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

#ifndef NOTEVIEWER_HPP
#define NOTEVIEWER_HPP

#include <bb/pim/notebook/NotebookEntryId>
#include <bb/pim/notebook/NotebookEntryStatus>
#include <bb/pim/notebook/NotebookService>

#include <QtCore/QObject>

/**
 * @short A controller object to encapsulate the logic of the note viewer
 *
 * The NoteViewer class is an UI-independent representation of the note viewer,
 * that provides all the functionality and data as slots and properties.
 * It encapsulates all the logic of loading a note from the persistent storage,
 * provide its data as properties and update the properties if the note has
 * changed in the storage backend.
 */
//! [0]
class NoteViewer : public QObject
{
    Q_OBJECT

    // The data properties of the note that is displayed
    Q_PROPERTY(QString title READ title NOTIFY titleChanged)
    Q_PROPERTY(QString description READ description NOTIFY descriptionChanged)
    Q_PROPERTY(QString dueDateTime READ dueDateTime NOTIFY dueDateTimeChanged)
    Q_PROPERTY(QString status READ status NOTIFY statusChanged)

public:
    NoteViewer(bb::pim::notebook::NotebookService *service, QObject *parent = 0);

    // Sets the ID of the note that should be displayed.
    void setNoteId(const bb::pim::notebook::NotebookEntryId &noteId);

    // A helper method to translate a note status into a user visible string
    static QString statusToString(bb::pim::notebook::NotebookEntryStatus::Type status);

Q_SIGNALS:
    // The change notification signals of the properties
    void titleChanged();
    void descriptionChanged();
    void dueDateTimeChanged();
    void statusChanged();

private Q_SLOTS:
    /**
     * This slot is invoked whenever the note service reports that a note has been changed.
     */
    void noteChanged(const QList<bb::pim::notebook::NotebookEntryId> &note);

private:
    // The accessor methods of the properties
    QString title() const;
    QString description() const;
    QString dueDateTime() const;
    QString status() const;

    // Loads the note from the persistent storage and updates the properties
    void updateNote();

    // The central object to access the note service
    bb::pim::notebook::NotebookService* m_notebookService;

    // The ID of the note that is displayed
    bb::pim::notebook::NotebookEntryId m_noteId;

    // The property values
    QString m_title;
    QString m_description;
    QDateTime m_dueDateTime;
    bb::pim::notebook::NotebookEntryStatus::Type m_status;
};
//! [0]

#endif
