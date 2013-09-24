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

#ifndef CONTACT_HPP
#define CONTACT_HPP

#include <bb/cascades/Image>
#include <bb/platform/bbm/Contact>
#include <bb/platform/bbm/ImageType>

//! [0]
class Contact: public QObject
{
	Q_OBJECT
	// Property to display contacts handle
	Q_PROPERTY(QString handle READ handle NOTIFY contactChanged)

	// Property to indicate users ppId
	Q_PROPERTY(QString ppid READ ppid NOTIFY contactChanged)

	// Property that indicates this app version running on the contacts device
	Q_PROPERTY(QString appVersion READ appVersion NOTIFY contactChanged)

	// BBM Social Platform version that is running on a contact's device
	Q_PROPERTY(QString platformVersion READ platformVersion NOTIFY contactChanged)

	// The contacts display name
	Q_PROPERTY(QString displayName READ displayName NOTIFY contactChanged)

	// The contacts personal message
	Q_PROPERTY(QString personalMessage READ personalMessage NOTIFY contactChanged)

	// The contacts status
	Q_PROPERTY(QString statusMessage READ statusMessage NOTIFY contactChanged)

	// Property to indicate contacts availability
	Q_PROPERTY(bool busy READ busy NOTIFY contactChanged)

	// The contacts avatar image
	Q_PROPERTY(QVariant avatar READ avatar NOTIFY avatarChanged)

public:
	Contact() { }

	/**
	 * Default constructor.
	 */
	Contact(const bb::platform::bbm::Contact &contact);
	~Contact();

	QString handle();
	QString ppid();
	QString appVersion();
	QString platformVersion();
	QString displayName();
	QString personalMessage();
	QString statusMessage();
	// Returns true if the contact's status is busy; otherwise, returns false.
	bool busy() const;
	QVariant avatar();

Q_SIGNALS:
	// Emitted when contact info changes
	void contactChanged();
	// Emitted when contact avatar changes
	void avatarChanged();

public Q_SLOTS:
 	// Corresponds to the bb::platform::bbm::ContactService::displayPictureUpdate() signal
	void avatarUpdated (const QString &handle, const bb::platform::bbm::ImageType::Type imageType, const QByteArray &displayPicture);

private:
	void setAvatar(const QByteArray &imageData);
	const bb::platform::bbm::Contact m_bbmspContact;
	bb::cascades::Image m_avatar;
};
//! [0]
Q_DECLARE_METATYPE(Contact *);

#endif /* CONTACT_HPP */
