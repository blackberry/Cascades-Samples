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

#ifndef NFCSHAREHANDLER_HPP
#define NFCSHAREHANDLER_HPP

#include "FileListModel.hpp"

#include <QtCore/QObject>

#include <bb/system/NfcShareManager>
#include <bb/system/SystemToast>

/*!
 * @brief A class to encapsulate the logic of sharing text data or files via NFC.
 */
class NfcShareHandler: public QObject
{
    Q_OBJECT

    // Describe the current mode of sharing
    Q_PROPERTY(ShareMode shareMode READ shareMode WRITE setShareMode NOTIFY shareModeChanged)

    // The list of files that shall be shared when in FileShare mode
    Q_PROPERTY(FileListModel* fileModel READ fileModel CONSTANT)

    // The textual that shall be shared when in DataShare mode
    Q_PROPERTY(QString data READ data WRITE setData NOTIFY dataChanged)

public:
    // Describes the possible sharing types
    enum ShareMode
    {
        DataShare,
        FileShare
    };
    Q_ENUMS(ShareMode)

    /*!
     * Creates a new NFC share handler.
     *
     * @param parent The parent object.
     */
    NfcShareHandler(QObject *parent = 0);

Q_SIGNALS:
    /*!
     * The change notification signals of the properties.
     */
    void shareModeChanged();
    void dataChanged();

private Q_SLOTS:
    /*!
     * Slot to prepare Data Share Content when new data is available
     */
    void prepareDataShareContent();

    /*!
     * Slot to prepare File Share Content when the FileListModel has been updated
     */
    void prepareFileShareContent();

    /*!
     * Slot used to display success notifications.
     *
     * @param result the type of success notification to display
     */
    void sharingFinished(bb::system::NfcShareSuccess::Type result);

    /*!
     * Slot used to display error notifications.
     *
     * @param error the type of error notification to display
     */
    void sharingError(bb::system::NfcShareError::Type error);

private:
    /*!
     * Private utility function to display notifications using
     * System Toasts.
     *
     * @param notification the message to be displayed in the System Toast
     */
    void displayNotification(const QString& notification);

    /*! The accessor methods of the property */
    void setShareMode(ShareMode mode);
    ShareMode shareMode() const;
    FileListModel* fileModel() const;
    void setData(const QString &data);
    QString data() const;

    /*! The property values */
    ShareMode m_shareMode;
    QString m_data;

    /*! The FileListModel represents a set of files to be sent through file sharing */
    FileListModel* m_fileListModel;
    bb::system::SystemToast* m_systemToast;

    /*! The NFC Share Manager is out main entry point into the Qt NFC Share Library */
    bb::system::NfcShareManager* m_nfcShareManager;
};

#endif
