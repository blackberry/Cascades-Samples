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

#include "NoteViewer.hpp"

#include <bb/pim/notebook/NotebookEntry>

using namespace bb::pim::notebook;

//! [0]
NoteViewer::NoteViewer(NotebookService *service, QObject *parent)
    : QObject(parent)
    , m_notebookService(service)
    , m_status(NotebookEntryStatus::NotCompleted)
{
    // Ensure to invoke the noteChanged() method whenever a note has been changed
    bool ok = connect(m_notebookService, SIGNAL(notebookEntriesUpdated(QList<bb::pim::notebook::NotebookEntryId>)), SLOT(noteChanged(QList<bb::pim::notebook::NotebookEntryId>)));
    Q_ASSERT(ok);
    Q_UNUSED(ok);
}
//! [0]

QString NoteViewer::statusToString(NotebookEntryStatus::Type status)
{
    switch (status) {
        case NotebookEntryStatus::NotActionable:
            return tr("Not actionable");
        case NotebookEntryStatus::NotCompleted:
            return tr("Not completed");
        case NotebookEntryStatus::Completed:
            return tr("Completed");
    }

    return QString();
}

//! [1]
void NoteViewer::updateNote()
{
    // Store previous values
    const QString oldTitle = m_title;
    const QString oldDescription = m_description;
    const QDateTime oldDueDateTime = m_dueDateTime;
    const NotebookEntryStatus::Type oldStatus = m_status;

    // Fetch new values from persistent storage
    const NotebookEntry note = m_notebookService->notebookEntry(m_noteId);

    m_title = note.title();
    m_description = note.description().plainText();
    m_dueDateTime = note.dueDateTime();
    m_status = note.status();

    // Check whether values have changed
    if (oldTitle != m_title)
        emit titleChanged();

    if (oldDescription != m_description)
        emit descriptionChanged();

    if (oldDueDateTime != m_dueDateTime)
        emit dueDateTimeChanged();

    if (oldStatus != m_status)
        emit statusChanged();
}
//! [1]

//! [2]
void NoteViewer::noteChanged(const QList<NotebookEntryId> &entries)
{
    /**
     * Call updateNote() only if the note we are currently displaying
     * has been changed.
     */
    if (entries.contains(m_noteId))
        updateNote();
}
//! [2]

//! [3]
void NoteViewer::setNoteId(const NotebookEntryId &noteId)
{
    m_noteId = noteId;

    // Trigger a refetch of the note for the new ID
    updateNote();
}
//! [3]

QString NoteViewer::title() const
{
    return m_title;
}

QString NoteViewer::description() const
{
    return m_description;
}

QString NoteViewer::dueDateTime() const
{
    if (!m_dueDateTime.isValid())
        return tr("n/a");
    else
        return m_dueDateTime.date().toString();
}

QString NoteViewer::status() const
{
    return statusToString(m_status);
}
