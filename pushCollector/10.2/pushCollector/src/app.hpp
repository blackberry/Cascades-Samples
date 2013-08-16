/*!
* Copyright (c) 2012, 2013 Research In Motion Limited.
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

#ifndef APP_HPP
#define APP_HPP

#include "service/ConfigurationService.hpp"
#include "service/MessageReader.hpp"
#include "service/PushHandler.hpp"
#include "service/UnregisterService.hpp"
#include "vo/CommandMessage.hpp"

#include <bb/cascades/GroupDataModel>

#include <bb/data/JsonDataAccess>

#include <bb/system/InvokeManager>
#include <bb/system/InvokeTargetReply>
#include <bb/system/SystemCredentialsPrompt>

using namespace pushcollector;

class PushContentController;

class App : public QObject
{
    Q_OBJECT

    // The data model that contains all received pushes
    Q_PROPERTY(bb::cascades::GroupDataModel* model READ model CONSTANT)
    Q_PROPERTY(bool modelIsEmpty READ modelIsEmpty NOTIFY modelIsEmptyChanged)

    // The title and body text for the notification dialog
    Q_PROPERTY(QString notificationTitle READ notificationTitle NOTIFY notificationChanged)
    Q_PROPERTY(QString notificationBody READ notificationBody NOTIFY notificationChanged)

    // The body text for the notification toast
    Q_PROPERTY(QString notificationToastBody READ notificationToastBody NOTIFY notificationToastChanged)

    // The title and body text for the progress dialog
    Q_PROPERTY(QString progressDialogTitle READ progressDialogTitle NOTIFY progressDialogChanged)
    Q_PROPERTY(QString progressDialogBody READ progressDialogBody NOTIFY progressDialogChanged)

    // The controller object for the push content page
    Q_PROPERTY(PushContentController* currentPushContent READ currentPushContent CONSTANT)

    // The configuration provider application ID.
    Q_PROPERTY(QString appId READ appId WRITE setAppId NOTIFY appIdChanged)

    // The configuration Push Proxy Gateway(PPG) URL.
    Q_PROPERTY(QString ppgUrl READ ppgUrl WRITE setPpgUrl NOTIFY ppgUrlChanged)

    // The configuration push initiator URL.
    Q_PROPERTY(QString pushInitiatorUrl READ pushInitiatorUrl WRITE setPushInitiatorUrl NOTIFY pushInitiatorUrlChanged)

    // The configuration 'useSdk' value.
    Q_PROPERTY(bool useSdk READ useSdk WRITE setUseSdk NOTIFY useSdkChanged)

    // The configuration 'launchApplicationOnPush' value.
    Q_PROPERTY(bool launchApplicationOnPush READ launchApplicationOnPush WRITE setLaunchApplicationOnPush NOTIFY launchApplicationOnPushChanged)

    // The configuration 'usingPublicPpg' value.
    Q_PROPERTY(bool usePublicPpg READ usePublicPpg WRITE setUsePublicPpg NOTIFY usePublicPpgChanged)

public:
    App();

    /**
     * Saves the Configuration to the persistent store.
     */
    Q_INVOKABLE void saveConfiguration();

    /**
     * Loads the Configuration from the persistent store.
     */
    Q_INVOKABLE void loadConfiguration();

    /**
     * Returns a value that indicates whether or not the Configuration settings are valid.
     *
     * @return true if valid; false otherwise
     */
    Q_INVOKABLE bool validateConfiguration();

    /**
     * Calls the push service create channel
     */
    Q_INVOKABLE void createChannel();

    /**
     * Calls the push service destroy channel
     */
    Q_INVOKABLE void destroyChannel();

    Q_INVOKABLE void deletePush(const QVariantMap &item);

    Q_INVOKABLE void deleteAllPushes();

    Q_INVOKABLE void markAllPushesAsRead();

    /**
     * Marks the passed push as current one and prepares the controller
     * object of the PushContentPage.
     */
    Q_INVOKABLE void selectPush(const QVariantList &indexPath);

    Q_INVOKABLE QString convertToUtf8String(const QVariant &pushContent);

public Q_SLOTS:
    void onRegisterPromptFinished(bb::system::SystemUiResult::Type value);
    void onUnregisterPromptFinished(bb::system::SystemUiResult::Type value);
    void onInvoked(const bb::system::InvokeRequest &request);
    void onAllPushesRemoved();
    void onMessageReceived(const StatusMessage &statusMsg);
    void onInvokeResult();

Q_SIGNALS:
    void modelIsEmptyChanged();
    void notificationChanged();
    void notificationToastChanged();
    void progressDialogChanged();

    void appIdChanged();
    void ppgUrlChanged();
    void pushInitiatorUrlChanged();
    void useSdkChanged();
    void launchApplicationOnPushChanged();
    void usePublicPpgChanged();

    void openProgressDialog();
    void closeProgressDialog();

private:
    void sendCommandToHeadless(const CommandMessage &command, const User &user = User());
    void checkForMessagesFromHeadless();
    void loadModelData();
    void loadUser();
    bool validateUser(const QString &dialogTitle, const QString &username, const QString &password);
    void setPromptDefaultText(bb::system::SystemCredentialsPrompt* prompt,const QString &username, const QString &password);
    void showDialog(const QString &title, const QString &message);
    void showToast(const QString &message);
    void openProgressDialog(const QString &title, const QString &message);

    // used to open and display a push when a notification is selected in the BlackBerry Hub
    void openPush(int pushSeqNum);

    // a helper function which marks the push as read, and updates the displayed push content
    void updatePushContent(Push &push, const QVariantList &indexPath);

    // The accessor methods of the properties
    bb::cascades::GroupDataModel* model() const;
    bool modelIsEmpty() const;

    QString notificationTitle() const;
    QString notificationBody() const;
    QString notificationToastBody() const;
    QString progressDialogTitle() const;
    QString progressDialogBody() const;

    PushContentController* currentPushContent() const;

    QString appId() const;
    void setAppId(const QString &appId);
    QString ppgUrl() const;
    void setPpgUrl(const QString &ppgUrl);
    QString pushInitiatorUrl() const;
    void setPushInitiatorUrl(const QString &pushInitiatorUrl);
    bool useSdk() const;
    void setUseSdk(bool value);
    bool launchApplicationOnPush() const;
    void setLaunchApplicationOnPush(bool launchAppOnPush);
    bool usePublicPpg() const;
    void setUsePublicPpg(bool usingPublicPpg);

    // The manager object to react to invocations
    bb::system::InvokeManager *m_invokeManager;

    // The credentials dialog to register to the push service
    bb::system::SystemCredentialsPrompt *m_registerPrompt;

    // The credentials dialog to unregister from the push service
    bb::system::SystemCredentialsPrompt *m_unregisterPrompt;

    MessageReader *m_messageReader;

    // Used to convert data from JSON format to Qt C++ objects
    bb::data::JsonDataAccess *m_jsonDA;

    PushHandler *m_pushHandler;

    bb::system::InvokeTargetReply *m_invokeTargetReply;

    // The wrapper classes for loading/storing configuration values
    ConfigurationService m_configurationService;
    Configuration m_configuration;

    // The wrapper class for the current user
    User m_user;
    UnregisterService m_unregisterService;

    bool m_shouldRegisterToLaunch;
    bool m_shouldUnregisterFromLaunch;

    // Whether or not the application has at some point in time been running in the foreground
    bool m_hasBeenInForeground;

    // Whether or not the Configuration is in the process of being saved
    bool m_configSaveAction;

    // The controller object for the push content page
    PushContentController* m_pushContentController;

    // The property values
    bb::cascades::GroupDataModel *m_model;
    QString m_notificationTitle;
    QString m_notificationBody;
    QString m_notificationToastBody;
    QString m_progressDialogTitle;
    QString m_progressDialogBody;
    QString m_appId;
    QString m_ppgUrl;
    QString m_pushInitiatorUrl;
    bool m_useSdk;
    bool m_launchApplicationOnPush;
    bool m_usePublicPpg;

};

#endif
