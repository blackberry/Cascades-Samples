/*
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

#include "app.hpp"

#include "PushContentController.hpp"
#include "PushUtility.hpp"

#include <bb/cascades/AbstractPane>
#include <bb/cascades/Application>
#include <bb/cascades/QmlDocument>
#include <bb/cascades/Sheet>

#include <bb/network/PushErrorCode>

#include <bb/platform/Notification>

#include <bb/system/InvokeRequest>
#include <bb/system/SystemDialog>
#include <bb/system/SystemUiButton>
#include <bb/system/SystemProgressDialog>
#include <bb/system/SystemToast>
#include <bb/cascades/datamanager/AsyncDataModel>
using namespace bb::network;
using namespace bb::cascades;
using namespace bb::system;
using namespace bb::platform;

App::App()
    : m_invokeManager(new InvokeManager(this))
    , m_registerPrompt(new SystemCredentialsPrompt(this))
    , m_unregisterPrompt(new SystemCredentialsPrompt(this))
	, m_messageReader(new MessageReader(this))
	, m_jsonDA(new bb::data::JsonDataAccess(this))
	, m_pushHandler(new PushHandler(this))
    , m_shouldRegisterToLaunch(false)
    , m_shouldUnregisterFromLaunch(false)
    , m_configSaveAction(false)
    , m_pushContentController(new PushContentController(this))
    , m_model(new GroupDataModel(this))
{
    qmlRegisterType<bb::system::SystemUiButton>("bb.system", 1, 0, "SystemUiButton");
    qmlRegisterType<SystemDialog>("bb.system", 1, 0, "SystemDialog");
    qmlRegisterType<SystemDialog>("bb.system", 1, 0, "SystemProgressDialog");
    qmlRegisterType<SystemDialog>("bb.system", 1, 0, "SystemToast");
    qmlRegisterType<PushContentController>();

    m_model->setSortingKeys(QStringList() << "pushdate" << "pushtime");
    m_model->setGrouping(ItemGrouping::ByFullValue);
    m_model->setSortedAscending(false);

    checkConnectResult(connect(m_model, SIGNAL(itemAdded(QVariantList)), SIGNAL(modelIsEmptyChanged())));
    checkConnectResult(connect(m_model, SIGNAL(itemUpdated(QVariantList)), SIGNAL(modelIsEmptyChanged())));
    checkConnectResult(connect(m_model, SIGNAL(itemRemoved(QVariantList)), SIGNAL(modelIsEmptyChanged())));
    checkConnectResult(connect(m_model, SIGNAL(itemsChanged(bb::cascades::DataModelChangeType::Type, QSharedPointer<bb::cascades::DataModel::IndexMapper>)), SIGNAL(modelIsEmptyChanged())));

    // initialize the registerPrompt
    m_registerPrompt->setTitle(tr("Register"));
    m_registerPrompt->confirmButton()->setLabel(tr("Register"));
    m_registerPrompt->setIncludeRememberMe(false);
    checkConnectResult(QObject::connect(m_registerPrompt, SIGNAL(finished(bb::system::SystemUiResult::Type)),
            this, SLOT(onRegisterPromptFinished(bb::system::SystemUiResult::Type))));

    // initialize the unregisterPrompt
    m_unregisterPrompt->setTitle(tr("Unregister"));
    m_unregisterPrompt->confirmButton()->setLabel(tr("Unregister"));
    m_unregisterPrompt->setIncludeRememberMe(false);
    checkConnectResult(QObject::connect(m_unregisterPrompt, SIGNAL(finished(bb::system::SystemUiResult::Type)),
            this, SLOT(onUnregisterPromptFinished(bb::system::SystemUiResult::Type))));

    // connect the message reader signals and slots
    checkConnectResult(QObject::connect(m_messageReader, SIGNAL(messageReceived(const StatusMessage&)),
                this, SLOT(onMessageReceived(const StatusMessage&))));

    // load the configuration settings
    loadConfiguration();

    // connect the push handler signals and slots
    checkConnectResult(QObject::connect(m_pushHandler, SIGNAL(allPushesRemoved()),
            this, SLOT(onAllPushesRemoved())));

    QmlDocument *qml = QmlDocument::create("asset:///main.qml");
    qml->setContextProperty("_pushAPIHandler", this);
    AbstractPane *root = qml->createRootObject<AbstractPane>();
    Application::instance()->setScene(root);

    // populate the model with the pushes that are stored in the database
    loadModelData();

    // connect the invoke manager signals and slot
    checkConnectResult(connect(m_invokeManager, SIGNAL(invoked(const bb::system::InvokeRequest&)),
            SLOT(onInvoked(const bb::system::InvokeRequest&))));

    // check for messages (The headless part could have put them there while the UI was not running)
    checkForMessagesFromHeadless();
}

void App::loadModelData(){
	m_model->clear();
	m_model->insertList(m_pushHandler->pushes());
}

void App::checkForMessagesFromHeadless()
{
    if (m_messageReader->hasMessage()){
    	onMessageReceived(m_messageReader->message());
    }
}

void App::onMessageReceived(const StatusMessage &statusMsg)
{
	// remove message
	m_messageReader->remove();

	// Determine the appropriate action to take by checking the status of the message
	if(QString::compare(PUSH_COLLECTOR_LAUNCH_CONFIG, statusMsg.status()) == 0){

		Sheet* configSheet = Application::instance()->findChild<Sheet*>("configurationSheet");
		if (configSheet) {
			configSheet->open();
		}

		showDialog(tr("Push Collector"), tr("No configuration settings were found. Please fill them in."));

	} else if(QString::compare(PUSH_COLLECTOR_CREATE_SESSION_COMPLETE, statusMsg.status()) == 0){

		emit closeProgressDialog();

		if (statusMsg.code() == PushErrorCode::NoError) {
			if (m_configSaveAction){
				showDialog(tr("Configuration"),tr("Configuration was saved. Please register now."));
			}
		} else{
			if (m_configSaveAction) {
				showDialog(tr("Configuration"), tr("Configuration was saved, but was unable to create push session. (Error code: %0)").arg(statusMsg.code()));
			} else {
				// Typically in your own application you wouldn't want to display this error to your users
				showToast(tr("Error: unable to create push session. (Error code: %0)").arg(statusMsg.code()));
			}
		}

		m_configSaveAction = false;

	} else if(QString::compare(PUSH_COLLECTOR_CREATE_CHANNEL_COMPLETE, statusMsg.status()) == 0){
		emit closeProgressDialog();
		showDialog(tr("Register"), statusMsg.message());

	} else if(QString::compare(PUSH_COLLECTOR_REGISTER_WITH_PUSH_INITIATOR, statusMsg.status()) == 0){
		openProgressDialog(tr("Register"), statusMsg.message());

	} else if(QString::compare(PUSH_COLLECTOR_REGISTER_WITH_PUSH_INITIATOR_COMPLETE, statusMsg.status()) == 0){
		emit closeProgressDialog();
		showDialog(tr("Register"), statusMsg.message());

	} else if(QString::compare(PUSH_COLLECTOR_DESTROY_CHANNEL_COMPLETE, statusMsg.status()) == 0){
		emit closeProgressDialog();
		showDialog(tr("Unregister"), statusMsg.message());

	} else if(QString::compare(PUSH_COLLECTOR_DEREGISTER_WITH_PUSH_INITIATOR, statusMsg.status()) == 0){
		openProgressDialog(tr("Unregister"), statusMsg.message());

	} else if(QString::compare(PUSH_COLLECTOR_DEREGISTER_WITH_PUSH_INITIATOR_COMPLETE, statusMsg.status()) == 0){
		emit closeProgressDialog();
		showDialog(tr("Unregister"), statusMsg.message());

	} else if(QString::compare(PUSH_COLLECTOR_SIM_CHANGE, statusMsg.status()) == 0) {

	    SystemDialog simChangeDialog;
	    simChangeDialog.setTitle("Push Collector");
	    simChangeDialog.setBody("The SIM card was changed and, as a result, the current user has been unregistered. Would you like to re-register?");
	    simChangeDialog.confirmButton()->setLabel("Yes");
	    simChangeDialog.cancelButton()->setLabel("No");

	    if (simChangeDialog.exec() == SystemUiResult::ConfirmButtonSelection) {
	        createChannel();
	    }

	} else if(QString::compare(PUSH_COLLECTOR_REFRESH_MODEL, statusMsg.status()) == 0){
		loadModelData();

	} else if(QString::compare(PUSH_COLLECTOR_NO_PUSH_SERVICE_CONNECTION, statusMsg.status()) == 0){

		QString message;
	    if (m_configSaveAction) {
	    	message = tr("Configuration was saved, but lost connection to the Push Agent. Trying to reconnect...");
	    } else {
	    	message = tr("Lost connection to the Push Agent. Trying to reconnect...");
	    }
	    showToast(message);
	    m_configSaveAction = false;

	} else if(QString::compare(PUSH_COLLECTOR_SHOW_DIALOG, statusMsg.status()) == 0){
		emit closeProgressDialog();
		showDialog(tr("Push Collector"), statusMsg.message());

	} else {
		emit closeProgressDialog();
		showToast(statusMsg.message());
		m_configSaveAction = false;
	}
}

void App::sendCommandToHeadless(const CommandMessage &command, const User &user)
{
	QVariantList invokeData;
	invokeData.append(QVariant(command.toMap()));

	if (!user.isEmpty()){
		invokeData.append(QVariant(user.toMap()));
	}

	QByteArray buffer;
	m_jsonDA->saveToBuffer(invokeData, &buffer);

    InvokeRequest request;
    request.setTarget(INVOKE_TARGET_KEY_PUSH);
    request.setAction(BB_PUSH_COLLECTOR_COMMAND_ACTION);
    request.setMimeType("text/plain");
    request.setData(buffer);
    m_invokeTargetReply = m_invokeManager->invoke(request);

    // Connect to the reply finished signal.
    checkConnectResult(QObject::connect(m_invokeTargetReply, SIGNAL(finished()), this, SLOT(onInvokeResult())));
}

void App::onInvokeResult()
{
	// Check for errors
	switch(m_invokeTargetReply->error()) {
		// Invocation could not find the target
		// did we use the right target ID?
	case InvokeReplyError::NoTarget: {
			qDebug() << "invokeFinished(): Error: no target";
			break;
		}
		// There was a problem with the invoke request
		// did we set all the values correctly?
	case InvokeReplyError::BadRequest: {
			qDebug() << "invokeFinished(): Error: bad request";
			break;
		}
		// Something went completely
		// wrong inside the invocation request
		// Find an alternate route :(
	case InvokeReplyError::Internal: {
			qDebug() << "invokeFinished(): Error: internal";
			break;
		}

	//Message received if the invoke request is successful
	default:
		qDebug() << "invokeFinished(): Invoke Succeeded";
		break;
	}

	// The reply is not needed now so we use deleteLater since we are in a slot.
	m_invokeTargetReply->deleteLater();
}

void App::saveConfiguration()
{
	m_configuration.setLaunchApplicationOnPush(m_launchApplicationOnPush);

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

    m_configuration.setUsingPublicPushProxyGateway(m_usePublicPpg);

    openProgressDialog(tr("Configuration"),tr("Saving configuration..."));
    m_configurationService.save(m_configuration);
    m_configSaveAction = true;

    CommandMessage command(PUSH_COLLECTOR_CREATE_SESSION, m_shouldRegisterToLaunch, m_shouldUnregisterFromLaunch, m_configSaveAction);
    sendCommandToHeadless(command);
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
            openProgressDialog(tr("Register"), tr("Creating push channel..."));

            CommandMessage command(PUSH_COLLECTOR_CREATE_CHANNEL);
            sendCommandToHeadless(command);
        }
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

            openProgressDialog(tr("Register"), tr("Creating push channel..."));

            CommandMessage command(PUSH_COLLECTOR_CREATE_CHANNEL);
            sendCommandToHeadless(command, m_user);
        } else {
            m_registerPrompt->show();
        }
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
            openProgressDialog(tr("Unregister"), tr("Destroying push channel..."));

            CommandMessage command(PUSH_COLLECTOR_DESTROY_CHANNEL);
            sendCommandToHeadless(command);
        }
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
            openProgressDialog(tr("Unregister"), tr("Destroying push channel..."));

            CommandMessage command(PUSH_COLLECTOR_DESTROY_CHANNEL);
            sendCommandToHeadless(command, m_user);
        } else {
            m_unregisterPrompt->show();
        }
    }
}

void App::onInvoked(const InvokeRequest &request)
{
	if (request.action().compare(BB_OPEN_INVOCATION_ACTION) == 0){
		qDebug() << "Received open action";
		// Received an invoke request to open an existing push (ie. from a notification in the BlackBerry Hub)
		// The payload from the open invoke is the seqnum for the push in the database
		openPush(request.data().toInt());
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
        m_pushHandler->remove(push.seqNum());
        m_model->remove(item);

        // The push has been deleted, so delete the notification
        Notification::deleteFromInbox(NOTIFICATION_PREFIX + QString::number(push.seqNum()));
    }
}

void App::deleteAllPushes()
{
    SystemDialog deleteAllDialog;
    deleteAllDialog.setTitle("Delete ALL");
    deleteAllDialog.setBody("Delete All Items?");
    deleteAllDialog.confirmButton()->setLabel("Delete");

    if (deleteAllDialog.exec() == SystemUiResult::ConfirmButtonSelection) {
        m_pushHandler->removeAll();
    }
}

void App::markAllPushesAsRead()
{
    if (m_model->size() > 0) {
        // All the pushes have been marked as open/read, so delete all the notifications for the app
    	Notification::deleteAllFromInbox();

        m_pushHandler->markAllAsRead();

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
    updatePushContent(push, indexPath);
}

void App::openPush(int pushSeqNum)
{
    Push push = m_pushHandler->push(pushSeqNum);
    QVariantList indexPath = m_model->findExact(push.toMap());
    updatePushContent(push, indexPath);
}

void App::updatePushContent(Push &push, const QVariantList &indexPath)
{
    push.setUnread(false);
    m_pushHandler->markAsRead(push.seqNum());

    m_model->updateItem(indexPath, push.toMap());

    // The push has been opened, so delete the notification
    Notification::deleteFromInbox(NOTIFICATION_PREFIX + QString::number(push.seqNum()));

    m_pushContentController->setCurrentPush(push);
}

void App::onAllPushesRemoved()
{
    // All the pushes have been deleted, so delete all the notifications for the app
    Notification::deleteAllFromInbox();
    m_model->clear();
}

QString App::convertToUtf8String(const QVariant &pushContent)
{
    return QString::fromUtf8(pushContent.toByteArray().data());
}

void App::showDialog(const QString &title, const QString &message)
{
    m_notificationTitle = title;
    m_notificationBody = message;

    emit notificationChanged();
}

void App::showToast(const QString &message)
{
	m_notificationToastBody = message;

	emit notificationToastChanged();
}

void App::openProgressDialog(const QString& title, const QString &message)
{
    m_progressDialogTitle = title;
    m_progressDialogBody = message;
    emit progressDialogChanged();

    emit openProgressDialog();
}

void App::loadUser()
{
    m_user = m_unregisterService.getCurrentlyRegisteredUser();
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

QString App::notificationToastBody() const
{
	return m_notificationToastBody;
}

QString App::progressDialogTitle() const
{
    return m_progressDialogTitle;
}

QString App::progressDialogBody() const
{
    return m_progressDialogBody;
}

PushContentController* App::currentPushContent() const
{
    return m_pushContentController;
}
