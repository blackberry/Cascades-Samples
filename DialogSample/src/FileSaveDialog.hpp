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

#ifndef FILESAVEDIALOG_HPP_
#define FILESAVEDIALOG_HPP_

#include "Dialog.hpp"

/*
 * The FileSave dialog displays an input box with the name of the file to save
 * This dialog has no title, and does not have a file selection option.
 */
class FileSaveDialog : public Dialog
{
    Q_OBJECT

    /*
     * QML exposed property for setting and retrieving the filename
     */
    Q_PROPERTY(QString filename READ getFilename WRITE setFilename)
public:
    /*
     * Ctor & Dtor
     */
    FileSaveDialog(QObject* parent = 0);
    virtual ~FileSaveDialog();

public:
    /*
     * Setter and Getter for the filename QML property
     */
    QString getFilename() const;
    void setFilename(QString const& value);

protected:
    /*
     * Implements the run loop. Dialog stays open until a button is clicked
     */
    virtual void run();

protected:
    QString m_filename;
};

#endif /* FILESAVEDIALOG_HPP_ */
