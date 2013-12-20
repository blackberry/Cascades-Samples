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
#include "BlackBerryProfileProvider.hpp"
#include <bb/platform/identity/BlackBerryProfile>
#include <bb/platform/identity/BlackBerryProfilePropertyType>
#include <bb/platform/identity/BlackBerryProfileCreateDataFlag>
#include <bb/platform/identity/BlackBerryProfileGetDataFlag>
#include <bb/platform/identity/BlackBerryProfileSetDataFlag>


using namespace bb::platform::identity;
//! [0]
BlackBerryProfileProvider::BlackBerryProfileProvider()
{
	// Instantiates an IdentityServiceProvider using
	// BlackBerryID as the provider.
	provider = new IdentityServiceProvider( BlackBerryProfile::providerName() );
	connectSignals();
	pressCount = 0;
}
//! [0]
BlackBerryProfileProvider::~BlackBerryProfileProvider()
{
	disconnectSignals();
}

int BlackBerryProfileProvider::getPressCount() const
{
	return provider->requestData( BlackBerryProfilePropertyType::Application,
										BlackBerryProfileGetDataFlag::CacheData, "pressCount" );
}

int BlackBerryProfileProvider::incrementPressCount()
{
	pressCount++;
	return provider->setData( BlackBerryProfilePropertyType::Application,
									BlackBerryProfileSetDataFlag::CacheData, "pressCount", QString::number( pressCount ) );
}

int BlackBerryProfileProvider::lastError()
{
	// If an IdentityServiceProvider API call fails, need to check
	// error() to find out why.
	return provider->error();
}

void BlackBerryProfileProvider::connectSignals()
{
	qRegisterMetaType<bb::platform::identity::IdentityServicePropertyResult::Type>("bb::platform::identity::IdentityServicePropertyResult::Type");

	QObject::connect( provider, SIGNAL( dataRetrieved( int, QByteArray ) ),
						this, SLOT( getDataComplete( int, QByteArray ) ) );

	QObject::connect( provider, SIGNAL( dataRetrievalFailed( int, bb::platform::identity::IdentityServicePropertyResult::Type, QString ) ),
						this, SLOT( getDataFail( int, bb::platform::identity::IdentityServicePropertyResult::Type, QString ) ) );

	QObject::connect( provider, SIGNAL( dataSet( int ) ),
						this, SIGNAL( pressCountUpdated( int ) ) );

	QObject::connect( provider, SIGNAL( dataSetFailed( int, bb::platform::identity::IdentityServicePropertyResult::Type, QString ) ),
						this, SLOT( setDataFail( int, bb::platform::identity::IdentityServicePropertyResult::Type, QString ) ) );


	QObject::connect( provider, SIGNAL( dataCreated( int ) ),
						this, SIGNAL( pressCountCreated( int ) ) );

	QObject::connect( provider, SIGNAL( dataCreateFailed( int, bb::platform::identity::IdentityServicePropertyResult::Type, QString ) ),
						this, SLOT( createDataFail( int, bb::platform::identity::IdentityServicePropertyResult::Type, QString ) ) );
}

void BlackBerryProfileProvider::disconnectSignals()
{
	qRegisterMetaType<bb::platform::identity::IdentityServicePropertyResult::Type>("bb::platform::identity::IdentityServicePropertyResult::Type");

	QObject::disconnect( provider, SIGNAL( dataRetrieved( int, QByteArray ) ),
						this, SLOT( getDataComplete( int, QByteArray ) ) );

	QObject::disconnect( provider, SIGNAL( dataRetrievalFailed( int, bb::platform::identity::IdentityServicePropertyResult::Type, QString ) ),
						this, SLOT( getDataFail( int, bb::platform::identity::IdentityServicePropertyResult::Type, QString ) ) );

	QObject::disconnect( provider, SIGNAL( dataSet( int ) ),
						this, SIGNAL( pressCountUpdated( int ) ) );

	QObject::disconnect( provider, SIGNAL( dataSetFailed( int, bb::platform::identity::IdentityServicePropertyResult::Type, QString ) ),
						this, SLOT( setDataFail( int, bb::platform::identity::IdentityServicePropertyResult::Type, QString ) ) );


	QObject::disconnect( provider, SIGNAL( dataCreated( int ) ),
						this, SIGNAL( pressCountCreated( int ) ) );

	QObject::disconnect( provider, SIGNAL( dataCreateFailed( int, bb::platform::identity::IdentityServicePropertyResult::Type, QString ) ),
						this, SLOT( createDataFail( int, bb::platform::identity::IdentityServicePropertyResult::Type, QString ) ) );
}
//! [1]
int BlackBerryProfileProvider::getData( const QString &name )
{
	int requestId = provider->requestData( BlackBerryProfilePropertyType::Application,
										BlackBerryProfileGetDataFlag::CacheData, name );

	if( requestId == 0 ) {
		// When requestData fails immediately (ie. due to bad input)
		// a request Id of 0 is returned and lastError() tells why.
		qDebug() << "Failed to get data: " + lastError();
		QString errorMessage = "Failed to get data: " + lastError();

		emit pressCountRetrievalFailed( errorMessage );
	}

	return requestId;
}

int BlackBerryProfileProvider::setData( const QString &name, const QString &value )
{
	int requestId = provider->setData( BlackBerryProfilePropertyType::Application,
									BlackBerryProfileSetDataFlag::CacheData, name, value );

	if( requestId == 0 ) {
		// When setData fails immediately (ie. due to bad input)
		// a request Id of 0 is returned and lastError() tells why.
		qDebug() << "Failed to set data: " + lastError();
		QString errorMessage = "Failed to set data: " + lastError();

		emit pressCountUpdateFailed( errorMessage );
	}
}

int BlackBerryProfileProvider::createData( const QString &name, const QString &value )
{
	int requestId = provider->createData( BlackBerryProfilePropertyType::Application,
										BlackBerryProfileCreateDataFlag::CacheData, name, value );

	if( requestId == 0 ) {
		// When createData fails immediately (ie. due to bad input)
		// a request Id of 0 is returned and lastError() tells why.
		qDebug() << "Failed to create data: " + lastError();
		QString errorMessage = "Failed to create data: " + lastError();

		emit pressCountCreateFailed( errorMessage );
	}
	return requestId;
}
//! [1]

// Slots
void BlackBerryProfileProvider::getDataComplete( int requestId, const QByteArray &data )
{
	Q_UNUSED( requestId );
	// Convert the byte array to string data to pass to QML
	std::string strData( data.constData(), data.length() );
	emit pressCountRetrieved( QString::fromStdString( strData ) );

	// Update press count
	pressCount = atoi( strData.c_str() );
}
//! [2]
void BlackBerryProfileProvider::getDataFail( int requestId, bb::platform::identity::IdentityServicePropertyResult::Type result, QString info )
{
	// Failed to get data for some reason.
	// See the header file for the full list of possible result codes that can be returned.
	Q_UNUSED( requestId );
	Q_UNUSED( info );
	QString errorMessage;

	switch( (int) result ) {
		case 50007:
			qDebug() << "Can't get data that doesn't exist, doing a create instead";
			errorMessage = "Property does not exist";
			int requestId;
			requestId = createData( "pressCount", QString::number( pressCount ) );
	        if( requestId == 0 ) {
	            qDebug() << "Failed to create user data: ", lastError();
	            errorMessage = "Unable to create profile information";
	        } else {
	            qDebug() << "Creating profile info for the first time";
	            errorMessage = "Creating ...";
	        }
	        break;
		case IdentityServicePropertyResult::UserCouldNotBeAuthenticated:
	        qDebug() << "Failure: " +  result;
			errorMessage = "You didn't login to BBID :(";
			break;
		default:
			qDebug() << "Failure: " +  result;
	        errorMessage = "Failed - Unable to store profile info";
	        break;
	}

	emit pressCountRetrievalFailed( errorMessage );
}


void BlackBerryProfileProvider::setDataFail( int requestId, bb::platform::identity::IdentityServicePropertyResult::Type result, QString info )
{
	// Failed to set data for some reason.
	// See the header file for the full list of possible result codes that can be returned.
	Q_UNUSED( requestId );
	Q_UNUSED( info );
	QString errorMessage;

	switch( (int) result ) {
		case 50007:
			qDebug() << "Can't set data that doesn't exist, doing a create instead";
			errorMessage = "Property does not exist";
			int requestId;
			requestId = createData( "pressCount", QString::number( pressCount ) );
			if( requestId == 0 ) {
	            qDebug() << "Failed to create user data: ", lastError();
	            errorMessage = "Unable to create profile information";
	        } else {
	            qDebug() << "Creating profile info for the first time";
	            errorMessage = "Creating ...";
	        }
	        break;
		case IdentityServicePropertyResult::UserCouldNotBeAuthenticated:
	        qDebug() << "Failure: " +  result;
			errorMessage = "You didn't login to BBID :(";
			break;
		default:
			qDebug() << "Failure: " +  result;
	        errorMessage = "Failed - Unable to store profile info";
	        break;
	}

	emit pressCountUpdateFailed( errorMessage );
}

void BlackBerryProfileProvider::createDataFail( int requestId, bb::platform::identity::IdentityServicePropertyResult::Type result, QString info )
{
	// Failed to create data for some reason.
	// See the header file for the full list of possible result codes that can be returned.
	Q_UNUSED( requestId );
	Q_UNUSED( info );
	QString errorMessage;

	switch( (int) result ) {
		case IdentityServicePropertyResult::AlreadyExists:
			qDebug() << "Can't create data that already exists - update instead";
			errorMessage = "Property exists";
			int requestId;
			requestId = setData( "pressCount", QString::number( pressCount ) );
	        if( requestId == 0 ) {
	            qDebug() << "Failed to update user data: ", lastError();
	            errorMessage = "Unable to update profile information";
	        } else {
	            qDebug() << "Updating profile info";
	            errorMessage = "Updating...";
	        }
	        break;
		case IdentityServicePropertyResult::UserCouldNotBeAuthenticated:
	        qDebug() << "Failure: " +  result;
			errorMessage = "You didn't login to BBID :(";
			break;
		default:
			qDebug() << "Failure: " +  result;
	        errorMessage = "Failed - Unable to store profile info";
	        break;
	}

	emit pressCountCreateFailed( errorMessage );
}
//! [2]
