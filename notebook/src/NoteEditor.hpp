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

#ifndef NOTEEDITOR_HPP
#define NOTEEDITOR_HPP

#include <bb/pim/notebook/NotebookEntryDescription>
#include <bb/pim/notebook/NotebookEntryId>
#include <bb/pim/notebook/NotebookService>

#include <QtCore/QObject>

/**
 * @short A controller object to encapsulate the logic of the note editor
 *
 * The NoteEditor class is an UI-independent representation of the note editor,
 * that provides all the functionality and data as slots and properties.
 * It encapsulates all the logic of creating a new note or updating an existing
 * one.
 */
//! [0]
class NoteEditor : public QObject
{
    Q_OBJECT

    // The data properties of the note that is created or updated
    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)
    Q_PROPERTY(QString description READ description WRITE setDescription NOTIFY descriptionChanged)
    Q_PROPERTY(QDateTime dueDateTime READ dueDateTime WRITE setDueDateTime NOTIFY dueDateTimeChanged)
    Q_PROPERTY(bool completed READ completed WRITE setCompleted NOTIFY completedChanged)

    // Defines whether the editor is in 'create' or 'edit' mode
    Q_PROPERTY(Mode mode READ mode WRITE setMode NOTIFY modeChanged)

    Q_ENUMS(Mode)

public:
    /**
     * Describes the mode of the note editor.
     * The mode information are used to adapt the behavior of the editor and
     * provide hints to the UI.
     */
    enum Mode {
        CreateMode,
        EditMode
    };

    NoteEditor(bb::pim::notebook::NotebookService *service, QObject *parent = 0);

    void setMode(Mode mode);
    Mode mode() const;

public Q_SLOTS:
    /**
     * Loads the note with the given ID.
     */
    void loadNote(const bb::pim::notebook::NotebookEntryId &noteId);

    /**
     * Save the currently loaded note if in 'edit' mode or creates a new one
     * if in 'create' mode.
     */
    void saveNote();

    /**
     * Resets all fields of the note editor.
     */
    void reset();

Q_SIGNALS:
    // The change notification signals of the properties
    void titleChanged();
    void descriptionChanged();
    void dueDateTimeChanged();
    void completedChanged();
    void modeChanged();

private:
    // The accessor methods of the properties
    void setTitle(const QString &title);
    QString title() const;

    void setDescription(const QString &description);
    QString description() const;

    void setDueDateTime(const QDateTime &dueDateTime);
    QDateTime dueDateTime() const;

    void setCompleted(bool completed);
    bool completed() const;

    // The central object to access the note service
    bb::pim::notebook::NotebookService *m_notebookService;

    // The ID of the currently loaded note (if in 'edit' mode)
    bb::pim::notebook::NotebookEntryId m_noteId;

    // The property values
    QString m_title;
    bb::pim::notebook::NotebookEntryDescription m_description;
    QDateTime m_dueDateTime;
    bool m_completed;

    Mode m_mode;
};
//! [0]

#endif
