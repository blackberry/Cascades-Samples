/* Copyright (c) 2013 Research In Motion Limited.
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

#include "Contact.hpp"

//! [0]
Contact::Contact(const bb::platform::bbm::Contact &contact) :
		m_bbmspContact(contact) {
	// Load the place holder for the display image (avatar)
	// Image by Decosigner: http://openclipart.org/detail/104977/help-orb-button-by-decosigner
	m_avatar = bb::cascades::Image(QUrl("asset:///images/avatarPlaceholder.png"));
}
//! [0]

Contact::~Contact() {
}

QString Contact::ppid() {
	return m_bbmspContact.ppId();
}

QString Contact::handle() {
	return m_bbmspContact.handle();
}

QString Contact::appVersion() {
	return m_bbmspContact.applicationVersion();
}

QString Contact::platformVersion() {
	return QString::number(m_bbmspContact.platformVersion());
}

//! [1]
QString Contact::displayName() {
	return m_bbmspContact.displayName();
}

QString Contact::personalMessage() {
	return m_bbmspContact.personalMessage();
}
//! [1]

QString Contact::statusMessage() {
	return m_bbmspContact.statusMessage();
}

bool Contact::busy() const {
	if (m_bbmspContact.status() == bb::platform::bbm::UserStatus::Busy) {
		return true;
	}
	return false;
}

//! [2]
void Contact::avatarUpdated(const QString& handle,
		const bb::platform::bbm::ImageType::Type imageType,
		const QByteArray& displayPicture) {

	Q_UNUSED(imageType);
	//Verify the update handle corresponds to this contact handle
	if (QString::compare(m_bbmspContact.handle(), handle) == 0) {
		// Verify that there is an image to be set.
		if(displayPicture.size() != 0) {
			setAvatar(displayPicture);
		}
	}
}

void Contact::setAvatar(const QByteArray &imageData) {
	m_avatar = bb::cascades::Image(imageData);
	Q_EMIT avatarChanged();
}
//! [2]

QVariant Contact::avatar() {
	return QVariant::fromValue(m_avatar);
}
