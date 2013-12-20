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
#ifndef BlackBerryProfileProvider_HPP_
#define BlackBerryProfileProvider_HPP_

#include <QObject>
#include <QVariantList>

#include <bb/platform/identity/IdentityService>
#include <bb/platform/identity/IdentityServiceProvider>
#include <bb/platform/identity/IdentityServiceResult>
#include <bb/platform/identity/IdentityServicePropertyResult>

using namespace bb::platform::identity;

//! [0]
class BlackBerryProfileProvider : public QObject
{
    Q_OBJECT
public:
    BlackBerryProfileProvider();
    virtual ~BlackBerryProfileProvider();

    Q_INVOKABLE int getPressCount() const;
    Q_INVOKABLE int incrementPressCount();
    Q_INVOKABLE int lastError();

public Q_SLOTS:
	// Triggered when getData succeeds, prepares a QString
	// from the byte array to return to QML - fired through
	// pressCountRetrieved.
	void getDataComplete( int requestId, const QByteArray &data );

	// Triggered when getData fails, handles the failure and fires
	// pressCountRetrievalFailed.
	void getDataFail( int requestId, bb::platform::identity::IdentityServicePropertyResult::Type result, QString info );

	// Triggered when setData fails, handles the failure and fires
	// pressCountUpdateFailed.
	void setDataFail( int requestId, bb::platform::identity::IdentityServicePropertyResult::Type result, QString info );

	// Triggered when createData fails, handles the failure and fires
	// pressCountCreateFailed.
	void createDataFail( int requestId, bb::platform::identity::IdentityServicePropertyResult::Type result, QString info );

Q_SIGNALS:
	void pressCountRetrieved( const QString &data );
	void pressCountRetrievalFailed( const QString &errorMessage );
	void pressCountUpdated( int requestId );
	void pressCountUpdateFailed( const QString &errorMessage );
	void pressCountCreated( int requestId );
	void pressCountCreateFailed( const QString &errorMessage );

private:
    int getData( const QString &name );
    int setData( const QString &name, const QString &value );
    int createData( const QString &name, const QString &value );

    void connectSignals();
	void disconnectSignals();

	// An instance of an IdentityServiceProvider
	// to use with BlackBerryProfile
	IdentityServiceProvider *provider;

	// Number of times the button has been pressed, stored in BlackBerry Profile so
	// it is accessible across devices for the same user.
	int pressCount;
};
//! [0]
#endif /* BlackBerryProfileProvider_HPP_ */
