/*
 * Copyright (c) 2011-2013 BlackBerry Limited.
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

// Navigation pane project template
#ifndef BlackBerryIdProvider_HPP_
#define BlackBerryIdProvider_HPP_

#include <QObject>
#include <QVariantList>

#include <bb/platform/identity/IdentityService>
#include <bb/platform/identity/IdentityServiceProvider>
#include <bb/platform/identity/IdentityServicePropertyResult>

using namespace bb::platform::identity;

//! [0]
class BlackBerryIdProvider : public QObject
{
    Q_OBJECT
public:
    BlackBerryIdProvider();
    virtual ~BlackBerryIdProvider();

    Q_INVOKABLE int getUserInfo();
    Q_INVOKABLE int lastError();

public Q_SLOTS:
	// Triggered when getuserProperties succeeds, prepares a user property
	// list for QML and fires getUserInfoSuccess.
	void getPropertiesSucceeded( int requestId, QMap<QString, QString> userProperties );
	// Triggered when getUserProperties fails, handles the failure and fires
	// getUserInfoFailed.
	void getPropertiesFail( int requestId, bb::platform::identity::IdentityServicePropertyResult::Type result, QString info );

Q_SIGNALS:
	void getUserInfoSuccess( int requestId, QVariantList userProperties );
	void getUserInfoFailed( const QString &errorMessage );

private:
    int getUserProperties( const QStringList &propertyList );
    void connectSignals();
	void disconnectSignals();

	// An instance of an IdentityServiceProvider
	// to use with BlackBerryId
	IdentityServiceProvider *provider;
};
//! [0]
#endif /* BlackBerryIdProvider_HPP_ */
