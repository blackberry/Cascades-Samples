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

#ifndef NOTEBOOK_HPP
#define NOTEBOOK_HPP

#include <bb/cascades/GroupDataModel>
#include <bb/pim/notebook/NotebookEntryId>
#include <bb/pim/notebook/NotebookService>

#include <QtCore/QObject>

class NoteEditor;
class NoteViewer;

/**
 * @short The controller class that makes access to notes available to the UI.
 */
//! [0]
class NoteBook : public QObject
{
    Q_OBJECT

    // The model that provides the filtered list of notes
    Q_PROPERTY(bb::cascades::GroupDataModel *model READ model CONSTANT);

    // The pattern to filter the list of notes
    Q_PROPERTY(QString filter READ filter WRITE setFilter NOTIFY filterChanged);

    // The viewer object for the current note
    Q_PROPERTY(NoteViewer* noteViewer READ noteViewer CONSTANT);

    // The editor object for the current note
    Q_PROPERTY(NoteEditor* noteEditor READ noteEditor CONSTANT);

public:
    NoteBook(QObject *parent = 0);

public Q_SLOTS:
    /**
     * Marks the note with the given @p indexPath as current.
     */
    void setCurrentNote(const QVariantList &indexPath);

    /**
     * Prepares the note editor to create a new note.
     */
    void createNote();

    /**
     * Prepares the note editor to edit the current note.
     */
    void editNote();

    /**
     * Prepares the note viewer to display the current note.
     */
    void viewNote();

    /**
     * Deletes the current note.
     */
    void deleteNote();

Q_SIGNALS:
    // The change notification signal for the property
    void filterChanged();

private Q_SLOTS:
    // Filters the notes in the model according to the filter property
    void filterNotes();

private:
    // The accessor methods of the properties
    bb::cascades::GroupDataModel* model() const;
    QString filter() const;
    void setFilter(const QString &filter);
    NoteViewer* noteViewer() const;
    NoteEditor* noteEditor() const;

    // The central object to access the notebook service
    bb::pim::notebook::NotebookService* m_notebookService;

    // The property values
    bb::cascades::GroupDataModel* m_model;
    QString m_filter;

    // The controller object for viewing a note
    NoteViewer* m_noteViewer;

    // The controller object for editing a note
    NoteEditor* m_noteEditor;

    // The ID of the current note
    bb::pim::notebook::NotebookEntryId m_currentNoteId;
};
//! [0]

#endif
