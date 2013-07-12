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

#ifndef CONTACSDISPLAY_HPP
#define CONTACSDISPLAY_HPP

#include "Contact.hpp"

#include <bb/cascades/QListDataModel>

namespace bb {
namespace platform {
namespace bbm {
class Context;
class ContactService;
}
}
}

/**
 * This class deals mainly with the ContactService and
 * the updating of the ListView model to reflect up-to-date
 * content.
 */
//! [0]
class ContactsDisplay: public QObject
{
	Q_OBJECT

public:
	/**
	 * Default Constructor.
	 */
	ContactsDisplay(bb::platform::bbm::Context &context, QObject *parent = 0);

public Q_SLOTS:
	/**
	 * Creates the ContactList qml document and sets
	 * the application scene to it.
	 */
	void show();

	/**
	 * Updates the ListView model with your bbm contacts.
	 */
	void updateModel();

	/**
	 * Updates the contact in the datamodel with the
	 * contact changes.
	 */
	void contactUpdated(const QString &handle);

private:
	bb::platform::bbm::Context *m_context;
	bb::cascades::QListDataModel<Contact*>* m_contactsDataModel;
	bb::platform::bbm::ContactService * m_contactService;

};
//! [0]
#endif /* CONTACSDISPLAY_HPP */
