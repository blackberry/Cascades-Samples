/*
* Copyright (c) 2012 Research In Motion Limited.
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

#include "app.hpp"

#include "PushContentController.hpp"

#include <bb/cascades/AbstractPane>
#include <bb/cascades/Application>
#include <bb/cascades/QmlDocument>
#include <bb/cascades/Sheet>

#include <bb/network/PushErrorCode>

#include <bb/system/InvokeRequest>
#include <bb/system/SystemDialog>
#include <bb/system/SystemUiButton>

using namespace bb::network;
using namespace bb::cascades;
using namespace bb::system;

App::App()
    : m_invokeManager(new InvokeManager(this))
    , m_registerPrompt(new SystemCredentialsPrompt(this))
    , m_unregisterPrompt(new SystemCredentialsPrompt(this))
    , m_shouldRegisterToLaunch(false)
    , m_shouldUnregisterFromLaunch(false)
    , m_pushContentController(new PushContentController(this))
    , m_model(new GroupDataModel(this))
{
    qmlRegisterType<bb::system::SystemUiButton>("bb.system", 1, 0, "SystemUiButton");
    qmlRegisterType<SystemDialog>("bb.system", 1, 0, "SystemDialog");
    qmlRegisterType<PushContentController>();

    // We set up the application Organization and name, this is used by QSettings
    // when saving values to the persistent store.
    QCoreApplication::setOrganizationName("Example");
    QCoreApplication::setApplicationName("Push Collector");

    m_model->setSortingKeys(QStringList() << "pushdate" << "pushtime");
    m_model->setGrouping(ItemGrouping::ByFullValue);
    m_model->setSortedAscending(false);

    connect(m_model, SIGNAL(itemAdded(QVariantList)), SIGNAL(modelIsEmptyChanged()));
    connect(m_model, SIGNAL(itemUpdated(QVariantList)), SIGNAL(modelIsEmptyChanged()));
    connect(m_model, SIGNAL(itemRemoved(QVariantList)), SIGNAL(modelIsEmptyChanged()));
    connect(m_model, SIGNAL(itemsChanged(bb::cascades::DataModelChangeType::Type, QSharedPointer<bb::cascades::DataModel::IndexMapper>)), SIGNAL(modelIsEmptyChanged()));

    // initialize the registerPrompt
    m_registerPrompt->setTitle(tr("Register"));
    m_registerPrompt->confirmButton()->setLabel(tr("Register"));
    m_registerPrompt->setIncludeRememberMe(false);
    QObject::connect(m_registerPrompt, SIGNAL(finished(bb::system::SystemUiResult::Type)),
            this, SLOT(onRegisterPromptFinished(bb::system::SystemUiResult::Type)));

    // initialize the unregisterPrompt
    m_unregisterPrompt->setTitle(tr("Unregister"));
    m_unregisterPrompt->confirmButton()->setLabel(tr("Unregister"));
    m_unregisterPrompt->setIncludeRememberMe(false);
    QObject::connect(m_unregisterPrompt, SIGNAL(finished(bb::system::SystemUiResult::Type)),
            this, SLOT(onUnregisterPromptFinished(bb::system::SystemUiResult::Type)));

    // load the configuration settings
    loadConfiguration();

    // connect the push notification service signals and slots
    QObject::connect(&m_pushNotificationService, SIGNAL(createSessionCompleted(const bb::network::PushStatus&)),
            this, SLOT(onCreateSessionCompleted(const bb::network::PushStatus&)));
    QObject::connect(&m_pushNotificationService, SIGNAL(createChannelCompleted(const bb::network::PushStatus&, const QString)),
            this, SLOT(onCreateChannelCompleted(const bb::network::PushStatus&, const QString)));
    QObject::connect(&m_pushNotificationService, SIGNAL(destroyChannelCompleted(const bb::network::PushStatus&)),
            this, SLOT(onDestroyChannelCompleted(const bb::network::PushStatus&)));
    QObject::connect(&m_pushNotificationService, SIGNAL(registerToLaunchCompleted(const bb::network::PushStatus&)),
            this, SLOT(onRegisterToLaunchCompleted(const bb::network::PushStatus&)));
    QObject::connect(&m_pushNotificationService, SIGNAL(unregisterFromLaunchCompleted(const bb::network::PushStatus&)),
            this, SLOT(onUnregisterFromLaunchCompleted(const bb::network::PushStatus&)));
    QObject::connect(&m_pushNotificationService, SIGNAL(piRegistrationCompleted(int, const QString)),
            this, SLOT(onPIRegistrationCompleted(int, const QString)));
    QObject::connect(&m_pushNotificationService, SIGNAL(piDeregistrationCompleted(int, const QString)),
            this, SLOT(onPIDeregistrationCompleted(int, const QString)));
    QObject::connect(&m_pushNotificationService, SIGNAL(simChanged()),
            this, SLOT(onSimChanged()));
    QObject::connect(&m_pushNotificationService, SIGNAL(pushTransportReady(bb::network::PushCommand::Type)),
    			this, SLOT(onPushTransportReady(bb::network::PushCommand::Type)));

    QmlDocument *qml = QmlDocument::create("asset:///main.qml");
    qml->setContextProperty("_pushAPIHandler", this);
    AbstractPane *root = qml->createRootObject<AbstractPane>();
    Application::instance()->setScene(root);

    // populate the model with the pushes that are stored in the database
    m_model->insertList(m_pushNotificationService.pushes());

    // connect to InvokeManager "invoked" signal to handle incoming push notifications.
    // We will ignore non-push invoke requests.
    connect(m_invokeManager, SIGNAL(invoked(const bb::system::InvokeRequest&)),
            SLOT(onInvoked(const bb::system::InvokeRequest&)));

    initializePushSession();
}

void App::saveConfiguration()
{
    // First, load the previous configuration so we can determine whether or not to register to launch
    // or unregister from launch
    if (m_configurationService.hasConfiguration()) {
        Configuration storedConfig = m_configurationService.configuration();

        if (!storedConfig.launchApplicationOnPush() && m_configuration.launchApplicationOnPush()) {
            m_shouldRegisterToLaunch = true;
            m_shouldUnregisterFromLaunch = false;
        } else if (storedConfig.launchApplicationOnPush() && !m_configuration.launchApplicationOnPush()) {
            m_shouldRegisterToLaunch = false;
            m_shouldUnregisterFromLaunch = true;
        } else {
            m_shouldRegisterToLaunch = false;
            m_shouldUnregisterFromLaunch = false;
        }

    } else {
        m_shouldRegisterToLaunch = m_configuration.launchApplicationOnPush();

        // There is no configuration currently stored, so register to launch was never previously called
        // so we wouldn't need to call unregister
        m_shouldUnregisterFromLaunch = false;
    }

    if (m_usePublicPpg || m_useSdk)
        m_configuration.setProviderApplicationId(m_appId);
    else
        m_configuration.setProviderApplicationId(QString());

    if (m_usePublicPpg)
        m_configuration.setPpgUrl(m_ppgUrl);
    else
        m_configuration.setPpgUrl(QString());

    if (m_useSdk)
        m_configuration.setPushInitiatorUrl(m_pushInitiatorUrl);
    else
        m_configuration.setPushInitiatorUrl(QString());

    m_configuration.setLaunchApplicationOnPush(m_launchApplicationOnPush);
    m_configuration.setUsingPublicPushProxyGateway(m_usePublicPpg);

    m_configurationService.save(m_configuration);
    showDialog(tr("Configuration"), tr("Configuration was saved."));
    m_pushNotificationService.createSession();
}

void App::loadConfiguration()
{
    m_configuration = m_configurationService.configuration();

    m_appId = m_configuration.providerApplicationId();
    m_ppgUrl = m_configuration.ppgUrl();
    m_pushInitiatorUrl = m_configuration.pushInitiatorUrl();
    m_useSdk = !m_pushInitiatorUrl.isEmpty();
    m_launchApplicationOnPush = m_configuration.launchApplicationOnPush();
    m_usePublicPpg = m_configuration.usingPublicPushProxyGateway();

    emit appIdChanged();
    emit ppgUrlChanged();
    emit pushInitiatorUrlChanged();
    emit useSdkChanged();
    emit launchApplicationOnPushChanged();
    emit usePublicPpgChanged();
}

bool App::validateConfiguration()
{
    bool isValid = false;

    // Trim the values
    m_appId = m_appId.trimmed();
    m_ppgUrl = m_ppgUrl.trimmed();
    m_pushInitiatorUrl = m_pushInitiatorUrl.trimmed();

    if ((m_usePublicPpg || m_useSdk) && m_appId.isEmpty()) {
        showDialog(tr("Configuration"), tr("Error: No provider application ID was specified."));
    } else if ((m_usePublicPpg || m_useSdk) && m_appId.indexOf("||") != -1) {
        showDialog(tr("Configuration"), tr("Error: A provider application ID is not allowed to contain '||'."));
    } else if (m_usePublicPpg && m_ppgUrl.isEmpty()) {
        showDialog(tr("Configuration"), tr("Error: No PPG URL was specified."));
    } else if (m_usePublicPpg && !m_ppgUrl.startsWith("http://")) {
        showDialog(tr("Configuration"), tr("Error: The PPG URL must start with http://."));
    } else if (m_usePublicPpg && !QUrl(m_ppgUrl).isValid()) {
        showDialog(tr("Configuration"), tr("Error: The PPG URL is invalid."));
    } else if (m_usePublicPpg && m_ppgUrl.endsWith("/")) {
        showDialog(tr("Configuration"), tr("Error: The PPG URL should not end with a /. One will be automatically added to the end."));
    } else if (m_useSdk && m_pushInitiatorUrl.isEmpty()) {
        showDialog(tr("Configuration"), tr("Error: No Push Initiator URL was specified."));
    } else if (m_useSdk && !m_pushInitiatorUrl.startsWith("http://") && !m_pushInitiatorUrl.startsWith("https://")) {
        showDialog(tr("Configuration"), tr("Error: The Push Initiator URL must start with http:// or https://."));
    } else if (m_useSdk && !QUrl(m_pushInitiatorUrl).isValid()) {
        showDialog(tr("Configuration"), tr("Error: The Push Initiator URL is invalid."));
    } else if (m_useSdk && m_pushInitiatorUrl.endsWith("/")) {
        showDialog(tr("Configuration"), tr("Error: The Push Initiator URL should not end with a /. One will be automatically added to the end."));
    } else {
        isValid = true;
    }

    return isValid;
}

void App::onCreateSessionCompleted(const bb::network::PushStatus &status)
{
    if (status.code() == PushErrorCode::NoError) {
        if (m_shouldRegisterToLaunch) {
            m_pushNotificationService.registerToLaunch();
        } else if (m_shouldUnregisterFromLaunch) {
            m_pushNotificationService.unregisterFromLaunch();
        }
    } else {
        // Typically in your own application you wouldn't want to display this error to your users
        showDialog(tr("Configuration"), m_notificationBody + tr("Error: unable to create push session. (Error code: %0)").arg(status.code()));
    }
}

void App::createChannel()
{
    if (!m_configurationService.hasConfiguration()) {
        showDialog(tr("Push Collector"), tr("Please fill in the config before attempting to register."));
    } else {
        if (!m_configuration.pushInitiatorUrl().isEmpty()) {
            // The Push Service SDK will be used on the server-side so prompt for a username and password

            loadUser();
            setPromptDefaultText(m_registerPrompt,m_user.userId(),m_user.password());
            m_registerPrompt->show();

        } else {
            // No username and password is needed when the
            // Push Service SDK is not being used on the server-side so
            // Jump straight to creating a push channel
            openActivityDialog(tr("Register"), tr("Creating push channel..."));
            m_pushNotificationService.createChannel();
        }
    }
}

void App::onCreateChannelCompleted(const bb::network::PushStatus &status, const QString &token)
{
    qDebug()<< "creatChannelComplete status: " << status.code();
    qDebug()<< "createChannelComplete token: " << token;

    emit closeActivityDialog();

    if (status.code() == PushErrorCode::NoError) {
        if (!m_configuration.pushInitiatorUrl().isEmpty()) {
            // Now, attempt to subscribe to the Push Initiator
            openActivityDialog(tr("Register"), tr("Subscribing to Push Initiator..."));

            // This is very important: the token returned in the create channel success event is what
            // the Push Initiator should use when initiating a push to the BlackBerry Push Service.
            // This token must be communicated back to the Push Initiator's server-side application.
            m_pushNotificationService.subscribeToPushInitiator(m_user, token);
        } else {
            showDialog(tr("Register"), tr("Register succeeded."));
        }
    } else {
        // Typically in your own application you wouldn't want to display this error to your users
        showDialog(tr("Register"), tr("Create channel failed with error code: %0").arg(status.code()));
    }

}

void App::onRegisterPromptFinished(bb::system::SystemUiResult::Type value)
{
    if (value == SystemUiResult::ConfirmButtonSelection) {
        QString username = m_registerPrompt->usernameEntry().trimmed();
        QString password = m_registerPrompt->passwordEntry().trimmed();
        setPromptDefaultText(m_registerPrompt,username,password);

        if (validateUser(tr("Register"), username, password)) {
            m_user.setUserId(username);
            m_user.setPassword(password);

            openActivityDialog(tr("Register"), tr("Creating push channel..."));
            m_pushNotificationService.createChannel();
        } else {
            m_registerPrompt->show();
        }
    }
}

void App::onPIRegistrationCompleted(int code, const QString &description)
{
    emit closeActivityDialog();

    if (code == 200) {
        showDialog(tr("Register"), tr("Register succeeded."));
    } else {
        // Typically in your own application you wouldn't want to display this error to your users
        showDialog(tr("Register"), tr("Subscribe to the Push Initiator failed with error code: %0. Reason: %1").arg(QString::number(code), description));
    }
}

void App::destroyChannel()
{
    if (!m_configurationService.hasConfiguration()) {
        showDialog(tr("Push Collector"), tr("Please fill in the config before attempting to unregister."));
    } else {
        if (!m_configuration.pushInitiatorUrl().isEmpty()) {
            // The Push Service SDK will be used on the server-side so prompt for a username and password

            loadUser();
            setPromptDefaultText(m_unregisterPrompt,m_user.userId(),m_user.password());
            m_unregisterPrompt->show();

        } else {
            // No username and password is needed when the
            // Push Service SDK is not being used
            // Jump straight to destroying a push channel
            openActivityDialog(tr("Unregister"), tr("Destroying push channel..."));
            m_pushNotificationService.destroyChannel();
        }
    }
}

void App::onDestroyChannelCompleted(const bb::network::PushStatus &status)
{
    qDebug() << "onDestroyChannelCompleted: " << status.code();

    emit closeActivityDialog();
    if (status.code() == PushErrorCode::NoError) {
        if (!m_configuration.pushInitiatorUrl().isEmpty()) {
            // The Push Service SDK will be used to unsubscribe to the Push Initiator's server-side application since a
            // Push Initiator URL was specified

            openActivityDialog(tr("Unregister"), tr("Unsubscribing from Push Initiator..."));
            // Now, attempt to unsubscribe from the Push Initiator
            m_pushNotificationService.unsubscribeFromPushInitiator(m_user);
        } else {
            showDialog(tr("Unregister"), tr("Unregister succeeded."));
        }
    } else {
        // Typically in your own application you wouldn't want to display this error to your users
        showDialog(tr("Unregister"), tr("Destroy channel failed with error code: %0").arg(status.code()));
    }
}

void App::onUnregisterPromptFinished(bb::system::SystemUiResult::Type value)
{
    if (value == SystemUiResult::ConfirmButtonSelection) {
        QString username = m_unregisterPrompt->usernameEntry().trimmed();
        QString password = m_unregisterPrompt->passwordEntry().trimmed();
        setPromptDefaultText(m_unregisterPrompt,username,password);

        if (validateUser(tr("Unregister"), username, password)) {
            m_user.setUserId(username);
            m_user.setPassword(password);
            openActivityDialog(tr("Unregister"), tr("Destroying push channel..."));
            m_pushNotificationService.destroyChannel();
        } else {
            m_unregisterPrompt->show();
        }
    }
}

void App::onPIDeregistrationCompleted(int code, const QString &description)
{
    emit closeActivityDialog();

    if (code == 200) {
        showDialog(tr("Unregister"), tr("Unregister succeeded."));
    } else {
        // Typically in your own application you wouldn't want to display this error to your users
        showDialog(tr("Unregister"), tr("Unsubscribe from the Push Initiator failed with error code: %0. Reason: %1").arg(QString::number(code), description));
    }
}

void App::onRegisterToLaunchCompleted(const bb::network::PushStatus &status)
{
    if (status.code() != PushErrorCode::NoError) {
        // Typically in your own application you wouldn't want to display this error to your users
        showDialog(tr("Configuration"), m_notificationBody + tr("Register to launch failed with error code: %0.").arg(status.code()));
    }
}

void App::onUnregisterFromLaunchCompleted(const bb::network::PushStatus &status)
{
    if (status.code() != PushErrorCode::NoError) {
        // Typically in your own application you wouldn't want to display this error to your users
        showDialog(tr("Configuration"), m_notificationBody + tr("Unregister from launch failed with error code: %0.").arg(status.code()));
    }
}

void App::onInvoked(const InvokeRequest &request)
{
    if (m_configurationService.hasConfiguration()) {
        // The underlying PushService instance might not have been
        // initialized when an invoke first comes in
        // Make sure that we initialize it here if it hasn't been already
        // It requires an application ID (for consumer applications) so we have to check
        // that configuration settings have already been stored
        m_pushNotificationService.initializePushService();

        if (request.action().compare(BB_PUSH_INVOCATION_ACTION) == 0) {
            PushPayload payload(request);
            if (payload.isValid()) {
                pushNotificationHandler(payload);
            }
        }
    }
}

void App::pushNotificationHandler(bb::network::PushPayload &pushPayload)
{
    // Check for a duplicate push
    PushHistoryItem pushHistoryItem(pushPayload.id());

    if (m_pushNotificationService.checkForDuplicatePush(pushHistoryItem)) {
        // A duplicate was found, stop processing. Silently discard this push from the user
        qWarning() << QString("Duplicate push was found with ID: %0.").arg(pushPayload.id());
        return;
    }

    // Convert from PushPayload to Push so that it can be stored in the database
    Push push(pushPayload);

    // Save the push and set the sequence number (ID) of the push
    push.setSeqNum(m_pushNotificationService.savePush(push));

    m_model->insert(push.toMap());

    // If an acknowledgement of the push is required (that is, the push was sent as a confirmed push
    // - which is equivalent terminology to the push being sent with application level reliability),
    // then you must either accept the push or reject the push
    if (pushPayload.isAckRequired()) {
        // In our sample, we always accept the push, but situations might arise where an application
        // might want to reject the push (for example, after looking at the headers that came with the push
        // or the data of the push, we might decide that the push received did not match what we expected
        // and so we might want to reject it)
        m_pushNotificationService.acceptPush(pushPayload.id());
    }
}

void App::deletePush(const QVariantMap &item)
{
    SystemDialog deleteDialog;
    deleteDialog.setTitle("Delete");
    deleteDialog.setBody("Delete Item?");
    deleteDialog.confirmButton()->setLabel("Delete");

    if (deleteDialog.exec() == SystemUiResult::ConfirmButtonSelection) {
        Push push(item);
        m_pushNotificationService.removePush(push.seqNum());
        m_model->remove(item);
    }

}

void App::deleteAllPushes()
{
    SystemDialog deleteAllDialog;
    deleteAllDialog.setTitle("Delete ALL");
    deleteAllDialog.setBody("Delete All Items?");
    deleteAllDialog.confirmButton()->setLabel("Delete");

    if (deleteAllDialog.exec() == SystemUiResult::ConfirmButtonSelection) {
        m_pushNotificationService.removeAllPushes();
        m_model->clear();
    }
}

void App::markAllPushesAsRead()
{
    if (m_model->size() > 0) {
        m_pushNotificationService.markAllPushesAsRead();

        for (QVariantList indexPath = m_model->first(); !indexPath.isEmpty(); indexPath = m_model->after(indexPath)) {
            QVariantMap item =  m_model->data(indexPath).toMap();
            item["unread"] = false;
            m_model->updateItem(indexPath,item);
        }
    }
}

void App::selectPush(const QVariantList &indexPath)
{
    const QVariantMap item = m_model->data(indexPath).toMap();
    Push push(item);
    push.setUnread(false);

    m_model->updateItem(indexPath, push.toMap());
    m_pushNotificationService.markPushAsRead(push.seqNum());

    m_pushContentController->setCurrentPush(push);
}

void App::onSimChanged()
{
    // Remove the currently registered user (if there is one)
    // and unsubscribe the user from the Push Initiator since
    // switching SIMs might indicate we are dealing with
    // a different user
    m_pushNotificationService.handleSimChange();

    SystemDialog simChangeDialog;
    simChangeDialog.setTitle("Push Collector");
    simChangeDialog.setBody("The SIM card was changed and, as a result, the current user has been unregistered. Would you like to re-register?");
    simChangeDialog.confirmButton()->setLabel("Yes");
    simChangeDialog.cancelButton()->setLabel("No");

    if (simChangeDialog.exec() == SystemUiResult::ConfirmButtonSelection) {
        createChannel();
    }
}

void App::onPushTransportReady(bb::network::PushCommand::Type command)
{
	QString message = "The push transport is now available. Please try ";
	if (command == PushCommand::CreateChannel){
		message += "registering ";
	} else {
		message += "unregistering ";
	}

	message += "again.";

	showDialog(tr("Push Collector"), message);
}

void App::initializePushSession()
{
    // If the app already has config info saved then create the session
    if (m_configurationService.hasConfiguration()) {

        m_shouldRegisterToLaunch = m_configuration.launchApplicationOnPush();
        m_shouldUnregisterFromLaunch = !m_configuration.launchApplicationOnPush();
        m_pushNotificationService.createSession();

    } else {

        Sheet* configSheet = Application::instance()->findChild<Sheet*>("configurationSheet");
        if (configSheet) {
            configSheet->open();
        }

        showDialog(tr("Push Collector"), tr("No configuration settings were found. Please fill them in."));
    }
}

void App::quit()
{
    QCoreApplication::quit();
}

void App::showDialog(const QString &title, const QString &message)
{
    m_notificationTitle = title;
    m_notificationBody = message;

    emit notificationChanged();
}

void App::openActivityDialog(const QString& title, const QString &message)
{
    m_activityDialogTitle = title;
    m_activityDialogBody = message;
    emit activityDialogChanged();

    emit openActivityDialog();
}

void App::loadUser()
{
    m_user = m_pushNotificationService.getCurrentlyRegisteredUser();
}

void App::setPromptDefaultText(SystemCredentialsPrompt* prompt,const QString &username, const QString &password)
{
    if (prompt) {
        prompt->usernameField()->setDefaultText(username);
        prompt->passwordField()->setDefaultText(password);
    }
}


bool App::validateUser(const QString &dialogTitle, const QString &username, const QString &password)
{
    bool isValid = false;

    if (username.isEmpty()) {
        showDialog(dialogTitle, tr("Error: No username was specified."));
    } else if (password.isEmpty()) {
        showDialog(dialogTitle, tr("Error: No password was specified."));
    } else {
        isValid = true;
    }

    return isValid;
}

QString App::appId() const
{
    return m_appId;
}

void App::setAppId(const QString &appId)
{
    if (m_appId == appId)
        return;

    m_appId = appId;
    emit appIdChanged();
}

QString App::ppgUrl() const
{
    return m_ppgUrl;
}

void App::setPpgUrl(const QString &ppgUrl)
{
    if (m_ppgUrl == ppgUrl)
        return;

    m_ppgUrl = ppgUrl;
    emit ppgUrlChanged();
}

QString App::pushInitiatorUrl() const
{
    return m_pushInitiatorUrl;
}

void App::setPushInitiatorUrl(const QString &pushInitiatorUrl)
{
    if (m_pushInitiatorUrl == pushInitiatorUrl)
        return;

    m_pushInitiatorUrl = pushInitiatorUrl;
    emit pushInitiatorUrlChanged();
}

bool App::launchApplicationOnPush() const
{
    return m_launchApplicationOnPush;
}

void App::setLaunchApplicationOnPush(bool launchAppOnPush)
{
    if (m_launchApplicationOnPush == launchAppOnPush)
        return;

    m_launchApplicationOnPush = launchAppOnPush;
    emit launchApplicationOnPushChanged();
}

bool App::useSdk() const
{
    return m_useSdk;
}

void App::setUseSdk(bool value)
{
    if (m_useSdk == value)
        return;

    m_useSdk = value;
    emit useSdkChanged();
}

bool App::usePublicPpg() const
{
    return m_usePublicPpg;
}

void App::setUsePublicPpg(bool usingPublicPpg)
{
    if (m_usePublicPpg == usingPublicPpg)
        return;

    m_usePublicPpg = usingPublicPpg;
    emit usePublicPpgChanged();
}

bb::cascades::GroupDataModel* App::model() const
{
    return m_model;
}

bool App::modelIsEmpty() const
{
    return (m_model->size() == 0);
}

QString App::notificationTitle() const
{
    return m_notificationTitle;
}

QString App::notificationBody() const
{
    return m_notificationBody;
}

QString App::activityDialogTitle() const
{
    return m_activityDialogTitle;
}

QString App::activityDialogBody() const
{
    return m_activityDialogBody;
}

PushContentController* App::currentPushContent() const
{
    return m_pushContentController;
}
