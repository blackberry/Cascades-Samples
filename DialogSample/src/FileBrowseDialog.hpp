/* Copyright (c) 2012 Research In Motion Limited.
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

#ifndef FILEBROWSEDIALOG_HPP_
#define FILEBROWSEDIALOG_HPP_

#include <QThread>
#include <QVariant>
#include <bps/dialog.h>

/*
 * The file browse dialog displays a dialog to browse and select
 * files from shared folders on the system.
 */
class FileBrowseDialog : public QThread
{
    Q_OBJECT

    /*
     * QML property to allow multiple selection
     */
    Q_PROPERTY(bool multiselect READ getMultiSelect WRITE setMultiSelect)

    /*
     * QML property to read the selected filenames
     */
    Q_PROPERTY(QVariant filepaths READ getFilePaths)

    /*
     * QML property to set or get the file filters. This is an
     * list array variant.
     */
    Q_PROPERTY(QVariant filters READ getFilters WRITE setFilters)
public:
    /*
     * Ctor and Dtor
     */
    FileBrowseDialog(QObject* parent = 0);
    virtual ~FileBrowseDialog();

    /*
     * Exposed to QML to start the run loop which creates and displays the dialog.
     * The dialog is shown until a button is clicked.
     */
    Q_INVOKABLE void show();

public:
    /*
     * Getter for the selected filenames QML property
     */
    QVariant getFilePaths() const;

    /*
     * Setter and Getter for the filters QML property
     */
    QVariant getFilters() const;
    void setFilters(QVariant const& value);

    /*
     * Getter and Setter for the multiselect QML property
     */
    bool getMultiSelect() const;
    void setMultiSelect(bool value);

signals:
    /*
     * Signal emitted when the OK button has been clicked on the browse dialog
     * The OK button is not enabled unless a file is selected
     */
    void selectionCompleted();

    /*
     * Signal emitted when the cancel button has been clicked on the browse dialog
     */
    void selectionCancelled();

protected:
    /*
     * Implements the run loop. Dialog stays open until a button is clicked.
     */
    virtual void run();

protected:
    dialog_instance_t m_dialog;
    bool m_multiSelect;
    QVariantList m_filePaths;
    QVariantList m_filters;
};

#endif /* FILEBROWSEDIALOG_HPP_ */
