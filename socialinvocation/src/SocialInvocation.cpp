/* Copyright (c) 2013 BlackBerry Limited.
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
#include "SocialInvocation.hpp"

#include <bb/system/InvokeRequest>
#include <bb/cascades/Application>
#include <bb/system/CardDoneMessage>
#include <bb/data/JsonDataAccess>
#include <bb/system/SystemToast>

using namespace bb::cascades;
using namespace bb::system;
using namespace bb::data;

//! [0]
SocialInvocation::SocialInvocation(QObject *parent) :
		QObject(parent), m_invokeManager(new InvokeManager(this)) {

	bool ok = connect(m_invokeManager,
				      SIGNAL(childCardDone(const bb::system::CardDoneMessage&)), this,
				      SLOT(childCardDone(const bb::system::CardDoneMessage&)));
	Q_ASSERT(ok);
	Q_UNUSED(ok);
}
//! [0]


void SocialInvocation::childCardDone(const bb::system::CardDoneMessage &doneMessage)
{
	SystemToast* toast = new SystemToast(this);
	if (doneMessage.dataType().compare("application/json") == 0)
	{
		// The data type from the venue search card is application/json
		// so we need to convert the json data to a map here. This will match
		// the json format from the Foursquare docs at http://developer.foursquare.com
		JsonDataAccess jda;
		const QVariantMap venueMap = jda.loadFromBuffer(doneMessage.data()).toMap();
		toast->setBody(QString("User Picked: ") + venueMap.value("name","").toString());
	}
	else
	{
		toast->setBody(doneMessage.data());
	}
	toast->show();
}

QByteArray SocialInvocation::encodeQString(const QString& toEncode) const {
	return toEncode.toUtf8();
}

//! [1]
void SocialInvocation::invoke(const QString &target, const QString &action,
		const QString &mimetype, const QString &uri) {
	// Create a new invocation request
	InvokeRequest request;

	request.setTarget(target);
	request.setAction(action);

	if (target == QLatin1String("com.rim.bb.app.facebook")) {
		QVariantMap payload;

		if (!uri.isEmpty()) {
			payload["object_type"] = mimetype;
			payload["object_id"] = uri;
		} else {
			// Open the BlackBerry North America page by default
			payload["object_type"] = "page";
			payload["object_id"] = "328506290597521";
		}

		request.setMetadata(payload);
	} else {
		request.setUri(uri);
	}

	m_invokeManager->invoke(request);
}
// The Foursquare invocation calls are based on the sample available here:
// https://github.com/kylefowler/foursquare-bb10-sdk
//
// Launches a native Foursquare venue search in your app.
// This card will call back to your childCardDone slot with the appropriate
// response for the actions taken by the user.
//
// URI Params:
// query: (optional) prime the venue search with a query
// client_id: (required) the client id from your oauth consumer
// client_secret: (required) the client secret from your oauth consumer
// oauth_token: (required if no client_id/client_secret) pass this if you
//              already have an authenticated user token, this way venue
//              search results will be fitted to the user requesting them
//              for a higher quality queryless search
//
// Response:
// When the user selects a venue, you will get the venue information in
// JSON format back through the childCardDone slot in the data object.
// The venue format will match what is listed here in the core venue fields:
// https://developer.foursquare.com/docs/responses/venue
//
// If the user cancels the search without any action: the reason message will be "canceled"
// If any of the parameters are missing you will get a reason message of "invalid_credentials"
void SocialInvocation::invokeFoursquareVenueCard(const QString &venue) {
	InvokeRequest cardRequest;
	cardRequest.setTarget("com.foursquare.blackberry.venuesearch.card");
	cardRequest.setAction("bb.action.VIEW");
	cardRequest.setMimeType("venuesearch/foursquare");

	// The client_id and client_secret are the Foursquare API credentials
	// that you receive when registering your app with Foursquare.
	//
	// You can register your app with Foursquare here:
	// https://foursquare.com/developers/apps
	//
	// For more information on Foursquare API credentials, see here:
	// https://developer.foursquare.com/overview/auth

	QUrl uri = QUrl("foursquare://venues/search");

	// Replace the following values with your app's client ID and secret
	uri.addQueryItem("client_id", "UFVANV2FBBFRPXSBXHCCKECVUDANDKP5KQFKICRCA1VAFV4V");
	uri.addQueryItem("client_secret","11AY4DWL0A2CV1NXPKDMS2PJTEACRZJP0BMFXORNCKBSNVMH");

	uri.addQueryItem("query", venue);
	cardRequest.setUri(uri);

	m_invokeManager->invoke(cardRequest);
}

// Login/Connect with Foursquare
// This card will call back to your childCardDone slot with the appropriate
// response for the actions taken by the user.
//
// Pass in your client_id from your Foursquare API consumer.
//
// If the user authorizes your app or has already authorized you:
// The response reason will be "success" and the data block will have a
// json encoded access token which you can use for authenticated Foursquare
// requests. That response string looks something like this:
// { access_token: "masdfvasvawefafawvwef90we0900990092012" }
//
// If the user denies the authentication: the response reason will be "denied".
// If the user cancels the login without any action: the reason message will be "canceled"
void SocialInvocation::onSSO() {
	InvokeRequest cardRequest;
	cardRequest.setTarget("com.foursquare.blackberry.sso.card");
	cardRequest.setAction("bb.action.VIEW");
	cardRequest.setMimeType("sso/foursquare");

	// Pass in the client_id that you receive when registering
	// your app with Foursquare.
	//
	// You can register your app with Foursquare here:
	// https://foursquare.com/developers/apps
	//
	// For more information on Foursquare API credentials, see here:
	// https://developer.foursquare.com/overview/auth
	cardRequest.setData(QString("UFVANV2FBBFRPXSBXHCCKECVUDANDKP5KQFKICRCA1VAFV4V").toUtf8());

	m_invokeManager->invoke(cardRequest);
}
//! [1]
