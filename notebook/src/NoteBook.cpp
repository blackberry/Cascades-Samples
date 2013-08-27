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

#include "NoteBook.hpp"

#include "NoteEditor.hpp"
#include "NoteViewer.hpp"

#include <bb/pim/notebook/NotebookEntry>

using namespace bb::cascades;
using namespace bb::pim::notebook;

//! [0]
NoteBook::NoteBook(QObject *parent)
    : QObject(parent)
    , m_notebookService(new NotebookService(this))
    , m_model(new GroupDataModel(this))
    , m_noteViewer(new NoteViewer(m_notebookService, this))
    , m_noteEditor(new NoteEditor(m_notebookService, this))
{
    // Disable grouping in data model
    m_model->setGrouping(ItemGrouping::None);

    // Ensure to invoke the filterNotes() method whenever a note has been added, changed or removed
    bool ok = connect(m_notebookService, SIGNAL(notebookEntriesAdded(QList<bb::pim::notebook::NotebookEntryId>)), SLOT(filterNotes()));
    Q_ASSERT(ok);
    ok = connect(m_notebookService, SIGNAL(notebookEntriesUpdated(QList<bb::pim::notebook::NotebookEntryId>)), SLOT(filterNotes()));
    Q_ASSERT(ok);
    ok = connect(m_notebookService, SIGNAL(notebookEntriesDeleted(QList<bb::pim::notebook::NotebookEntryId>)), SLOT(filterNotes()));
    Q_ASSERT(ok);

    // Fill the data model with notes initially
    filterNotes();
}
//! [0]

//! [1]
void NoteBook::setCurrentNote(const QVariantList &indexPath)
{
    // Extract the ID of the selected note from the model
    if (indexPath.isEmpty()) {
        m_currentNoteId = NotebookEntryId();
    } else {
        const QVariantMap entry = m_model->data(indexPath).toMap();
        m_currentNoteId = entry.value("noteId").value<NotebookEntryId>();
    }
}
//! [1]

//! [2]
void NoteBook::createNote()
{
    // Prepare the note editor for creating a new note
    m_noteEditor->reset();
    m_noteEditor->setMode(NoteEditor::CreateMode);
}
//! [2]

//! [3]
void NoteBook::editNote()
{
    // Prepare the note editor for editing the current note
    m_noteEditor->loadNote(m_currentNoteId);
    m_noteEditor->setMode(NoteEditor::EditMode);
}
//! [3]

//! [4]
void NoteBook::viewNote()
{
    // Prepare the note viewer for displaying the current note
    m_noteViewer->setNoteId(m_currentNoteId);
}
//! [4]

//! [5]
void NoteBook::deleteNote()
{
    m_notebookService->deleteNotebookEntry(m_currentNoteId);
}
//! [5]

bb::cascades::GroupDataModel* NoteBook::model() const
{
    return m_model;
}

QString NoteBook::filter() const
{
    return m_filter;
}

//! [6]
void NoteBook::setFilter(const QString &filter)
{
    if (m_filter == filter)
        return;

    m_filter = filter;
    emit filterChanged();

    // Update the model now that the filter criterion has changed
    filterNotes();
}
//! [6]

NoteViewer* NoteBook::noteViewer() const
{
    return m_noteViewer;
}

NoteEditor* NoteBook::noteEditor() const
{
    return m_noteEditor;
}

//! [7]
void NoteBook::filterNotes()
{
    NotebookEntryFilter filter;

    // Use the entered filter string as search string
    filter.setSearchString(m_filter);

    const QList<NotebookEntry> notes = m_notebookService->notebookEntries(filter);

    // Clear the old note information from the model
    m_model->clear();

    // Iterate over the list of notes
    foreach (const NotebookEntry &note, notes) {
        // Copy the data into a model entry
        QVariantMap entry;
        entry["noteId"] = QVariant::fromValue(note.id());
        entry["title"] = note.title();
        entry["status"] = NoteViewer::statusToString(note.status());

        // Add the entry to the model
        m_model->insert(entry);
    }
}
//! [7]
