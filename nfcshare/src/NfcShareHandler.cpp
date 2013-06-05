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

#include "NfcShareHandler.hpp"

//! [0]
NfcShareHandler::NfcShareHandler(QObject *parent)
    : QObject(parent)
    , m_shareMode(DataShare)
    , m_fileListModel(new FileListModel(this))
    , m_systemToast(new bb::system::SystemToast(this))
    , m_nfcShareManager(new bb::system::NfcShareManager(this))
{
    connect(m_fileListModel, SIGNAL(changed()), this, SLOT(prepareFileShareContent()));

    // Connect signal for finished (success) notifications
    connect(m_nfcShareManager, SIGNAL(finished(bb::system::NfcShareSuccess::Type)),
            this, SLOT(sharingFinished(bb::system::NfcShareSuccess::Type)));

    // Connect signal for error notifications
    connect(m_nfcShareManager, SIGNAL(error(bb::system::NfcShareError::Type)),
            this, SLOT(sharingError(bb::system::NfcShareError::Type)));
}
//! [0]

//! [1]
void NfcShareHandler::prepareDataShareContent()
{
    if (m_data.isEmpty())
        return;

    // Prepare new data share content
    bb::system::NfcShareDataContent dataShareContent;

    // The data content needs to be stored as a QByteArray
    const QByteArray data(m_data.toUtf8());

    // We will share our data as text/plain data
    dataShareContent.setMimeType(QLatin1String("text/plain"));
    dataShareContent.setData(data);

    // Set the share content to our new data share content
    m_nfcShareManager->setShareContent(dataShareContent);
}
//! [1]
//! [2]
void NfcShareHandler::prepareFileShareContent()
{
    // Prepare new file share content
    bb::system::NfcShareFilesContent fileShareContent;

    // Get the current list of files stored by the FileListModel
    const QList<QUrl> files = m_fileListModel->files();
    if (files.isEmpty())
        return;

    fileShareContent.setFileUrls(files);

    // Set the share content to our new file share content
    m_nfcShareManager->setShareContent(fileShareContent);
}
//! [2]
//! [3]
void NfcShareHandler::sharingFinished(bb::system::NfcShareSuccess::Type result)
{
    // TODO: clean up - some bluetooth notifications overlap with ours
    // we will display a System Toast when a transaction has been completed successfully
    switch (result) {
        case bb::system::NfcShareSuccess::File:
            displayNotification(tr("NFC file transfer started successfully"));
            break;
        case bb::system::NfcShareSuccess::Snep:
            displayNotification(tr("NFC data was transfered successfully"));
            break;
        case bb::system::NfcShareSuccess::TagWrite:
            displayNotification(tr("NFC data was written successfully"));
            break;
        default:
            displayNotification(tr("Unknown share success"));
            break;
    }
}

void NfcShareHandler::sharingError(bb::system::NfcShareError::Type error)
{
    switch (error) {
        /**
         * Unrecoverable NFC errors
         */
        case bb::system::NfcShareError::NoContentToShare:
        case bb::system::NfcShareError::Unknown:
            displayNotification(tr("Failed to share data over NFC"));
            break;
        case bb::system::NfcShareError::RegisterFileSharing:
            displayNotification(tr("Failed to prepare for file sharing"));
            break;
        case bb::system::NfcShareError::RegisterDataSharing:
            displayNotification(tr("Failed to prepare for data sharing"));
            break;
            /**
             * Recoverable NFC errors - try again
             */
        case bb::system::NfcShareError::MessageSize:
            displayNotification(tr("NFC Tag is too small for a shared message"));
            break;
        case bb::system::NfcShareError::TagLocked:
            displayNotification(tr("Failed to write to a locked NFC Tag"));
            break;
        case bb::system::NfcShareError::UnsupportedTagType:
            displayNotification(tr("Failed to write to a unsupported NFC Tag"));
            break;
        case bb::system::NfcShareError::DataTransferFailed: //includes REJECTED and IO ERROR
        case bb::system::NfcShareError::HandoverFailed:
        case bb::system::NfcShareError::BluetoothFileTransferFailed:
            displayNotification(tr("Failed to share data over NFC - tap again"));
            break;
        default:
            displayNotification(tr("Unknown nfc share error"));
            break;
    }
}

void NfcShareHandler::displayNotification(const QString& notification)
{
    //TODO: clean - a bunch of these toasts can pile up and require the
    // user to close them all

    // Create a System Toast to display our notifications.
    // The toast will be placed in the middle center of the screen and will NOT auto dismiss
    // the user will have to click 'Ok' to dismiss the toast.
    m_systemToast->setBody(notification);
    m_systemToast->setPosition(bb::system::SystemUiPosition::MiddleCenter);

    // The modality should be set to Application, a Global Modality will prevent
    // the toast from disappearing when the application is thumbnailed.
    m_systemToast->setModality(bb::system::SystemUiModality::Application);

    bb::system::SystemUiButton* toastButton = m_systemToast->button();
    toastButton->setLabel(tr("Ok"));

    // We need to enable the button or it will auto dismiss
    toastButton->setEnabled(true);

    // Display the System Toast
    m_systemToast->show();
}
//! [3]

//! [4]
void NfcShareHandler::setShareMode(ShareMode mode)
{
    if (m_shareMode == mode)
        return;

    m_shareMode = mode;

    // Clear all previous set data
    //m_nfcShareManager->reset();

    // HACK: Needed in R9, since reset() does not work as supposed
    m_nfcShareManager->setShareMode(bb::system::NfcShareMode::Disabled);

    switch (m_shareMode) {
        case DataShare:
            m_nfcShareManager->setShareMode(bb::system::NfcShareMode::Data);
            prepareDataShareContent();
            break;
        case FileShare:
            m_nfcShareManager->setShareMode(bb::system::NfcShareMode::File);
            prepareFileShareContent();
            break;
    }

    emit shareModeChanged();
}
//! [4]
NfcShareHandler::ShareMode NfcShareHandler::shareMode() const
{
    return m_shareMode;
}

FileListModel* NfcShareHandler::fileModel() const
{
    return m_fileListModel;
}

void NfcShareHandler::setData(const QString &data)
{
    if (m_data == data)
        return;

    m_data = data;
    prepareDataShareContent();

    emit dataChanged();
}

QString NfcShareHandler::data() const
{
    return m_data;
}
