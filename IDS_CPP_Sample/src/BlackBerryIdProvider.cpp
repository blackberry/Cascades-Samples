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

#include "BlackBerryIdProvider.hpp"
#include <bb/platform/identity/BlackBerryId>
#include <bb/platform/identity/BlackBerryIdPropertyType>
#include <bb/platform/identity/IdentityServiceResult>


using namespace bb::platform::identity;
//! [0]
BlackBerryIdProvider::BlackBerryIdProvider()
{
	// Instantiates an IdentityServiceProvider using
	// BlackBerryID as the provider.
	provider = new IdentityServiceProvider( BlackBerryId::providerName() );
	connectSignals();
}
//! [0]
BlackBerryIdProvider::~BlackBerryIdProvider()
{
	disconnectSignals();
}


int BlackBerryIdProvider::getUserInfo()
{
	QStringList propertyList;
	// Request first name and user name - first name is optional for
	// BlackBerry ID users.
	propertyList << BlackBerryId::firstNamePropertyKey() << BlackBerryId::userNamePropertyKey();
	return getUserProperties( propertyList );
}

int BlackBerryIdProvider::lastError()
{
	// If an IdentityServiceProvider API call fails, need to check
	// error() to find out why.
	return provider->error();
}

void BlackBerryIdProvider::connectSignals()
{
	qRegisterMetaType<bb::platform::identity::IdentityServicePropertyResult::Type>("bb::platform::identity::IdentityServicePropertyResult::Type");

	QObject::connect( provider, SIGNAL( propertiesRetrieved( int, QMap<QString, QString> ) ),
						this, SLOT( getPropertiesSucceeded( int, QMap<QString, QString> ) ) );

	QObject::connect( provider, SIGNAL( propertyRetrievalFailed( int, bb::platform::identity::IdentityServicePropertyResult::Type, QString ) ),
						this, SLOT( getPropertiesFail( int, bb::platform::identity::IdentityServicePropertyResult::Type, QString ) ) );
}

void BlackBerryIdProvider::disconnectSignals()
{
	QObject::disconnect( provider, SIGNAL( propertiesRetrieved( int, QMap<QString, QString> ) ),
						this, SLOT( getPropertiesSucceeded( int, QMap<QString, QString> ) ) );

	QObject::disconnect( provider, SIGNAL( propertyRetrievalFailed( int, bb::platform::identity::IdentityServicePropertyResult::Type, QString ) ),
						this, SLOT( getPropertiesFail( int, bb::platform::identity::IdentityServicePropertyResult::Type, QString ) ) );
}

//! [1]
int BlackBerryIdProvider::getUserProperties( const QStringList &propertyList )
{
	int requestId = provider->requestProperties( BlackBerryIdPropertyType::Core, propertyList );
	if( requestId == 0 ) {
		// When requestProperties fails immediately (ie. due to bad input)
		// a request Id of 0 is returned and lastError() tells why.
		qDebug() << "Failed to get user properties: " + lastError();
		QString errorMessage = "Failed to request user properties: " + lastError();

		emit getUserInfoFailed( errorMessage );
	}
	return requestId;
}



// Slots
void BlackBerryIdProvider::getPropertiesSucceeded( int requestId, QMap<QString, QString> userProperties )
{
	// Successfully retrieved user properties
	QVariantList propertyList;

	QMapIterator<QString, QString> i( userProperties );

	while( i.hasNext() ) {
		i.next();

		QVariantMap userProperty;
		userProperty["name"] = i.key();
		userProperty["value"] = i.value();
		propertyList << userProperty;
	}

	emit getUserInfoSuccess( requestId, propertyList );
}
//! [1]
//! [2]
void BlackBerryIdProvider::getPropertiesFail( int requestId, bb::platform::identity::IdentityServicePropertyResult::Type result, QString info )
{
	// Failed to get user properties for some reason.
	// See the header file for the full list of possible result codes that can be returned.
	Q_UNUSED( requestId );
	Q_UNUSED( info );
	QString errorMessage;

	switch( (int) result ) {
		case IdentityServicePropertyResult::UserCouldNotBeAuthenticated:
			qDebug() << "Failure: " +  result;
			errorMessage = "You didn't login to BBID :(";
			break;
		default:
			qDebug() << "Failure: " +  result;
			errorMessage = "Failed - Unable to get user properties";
			break;
	}

	emit getUserInfoFailed( errorMessage );
}
//! [2]
