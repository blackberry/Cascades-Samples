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

#include "ContactsDisplay.hpp"

#include <bb/cascades/Application>
#include <bb/cascades/QmlDocument>
#include <bb/cascades/AbstractPane>
#include <bb/platform/bbm/Context>
#include <bb/platform/bbm/ContactService>
#include <bb/platform/bbm/ImageType>
#include <bb/cascades/ListView>

using namespace bb::cascades;

//! [0]
ContactsDisplay::ContactsDisplay(bb::platform::bbm::Context &context, QObject *parent)
	: QObject(parent)
	, m_context(&context)
	, m_contactsDataModel(new QListDataModel<Contact*>())
	, m_contactService(0)
{
	m_contactsDataModel->setParent(this);
}
//! [0]

//! [1]
void ContactsDisplay::show()
{
	// Create the UI
	QmlDocument *qml = QmlDocument::create("asset:///ContactList.qml").parent(this);
	AbstractPane *root = qml->createRootObject<AbstractPane>();
	Application::instance()->setScene(root);

	m_contactService = new bb::platform::bbm::ContactService(m_context, this);

	// Connect the update signals to the model update slot
	bool ok = QObject::connect(m_contactService, SIGNAL(contactListUpdated()), this, SLOT (updateModel()));
	Q_ASSERT(ok);

	ok = QObject::connect(m_contactService, SIGNAL(applicationEnabled(const QString&)), this, SLOT (updateModel()));
	Q_ASSERT(ok);

	ok = QObject::connect(m_contactService, SIGNAL(applicationDisabled(const QString&)), this, SLOT (updateModel()));
	Q_ASSERT(ok);

	ok = QObject::connect(m_contactService, SIGNAL(contactUpdated(const QString&)), this, SLOT(contactUpdated(const QString&)));
	Q_ASSERT(ok);

	if (m_contactService->isValid()) {
		updateModel();
	}

	// Retrieve the Field.qml custom component and set it's value
	QObject* contactCountField = root->findChild<QObject*>("contactCount");
	QString contCount = QString("%1").arg(m_contactService->contactCount());

	contactCountField->setProperty("value", contCount);

	// Retrieve the ListView and set its datamodel
	ListView* contactListView = root->findChild<ListView*>("contactListView");
	contactListView->setDataModel(m_contactsDataModel);

	qDebug() << contCount;
}
//! [1]

//! [2]
void ContactsDisplay::updateModel() {
	QList<bb::platform::bbm::Contact> contacts = m_contactService->contacts();
	m_contactsDataModel->clear();

	// For each bbm::Contact created a Contact and add it to the ListView datamodel
	for (int i = 0; i < contacts.size(); i++)
	{
		Contact* bbm_contact = new Contact(contacts.at(i));
		m_contactsDataModel->append(bbm_contact);
		// Signal-Slot connection for updating contact avatar
		bool ok = connect(m_contactService, SIGNAL(displayPictureUpdated(const QString&, const bb::platform::bbm::ImageType::Type
															              , const QByteArray&))
										   	   	   	 , bbm_contact
										   	   	   	 , SLOT(avatarUpdated(const QString&
										   	   	   	   	    		   	  , const bb::platform::bbm::ImageType::Type
										   	   	   	   	    		   	  , const QByteArray&)));
		Q_ASSERT(ok);
		m_contactService->requestDisplayPicture(bbm_contact->handle());
	}
}
//! [2]

//! [3]
void ContactsDisplay::contactUpdated(const QString& handle) {
	for(int i = 0; i < m_contactsDataModel->size(); i++) {
		QVariantList index;
		index << i;
		Contact *contact = m_contactsDataModel->value(i);
		// Once the right contact handle is found, replace the old Contact instance
		// with the new updated one.
		if(QString::compare(contact->handle(), handle) == 0) {
			Contact* new_contact = new Contact(m_contactService->contact(handle));
			// Signal-Slot connection for updating contact avatar
			bool ok = connect(m_contactService, SIGNAL(displayPictureUpdated(const QString&, const bb::platform::bbm::ImageType::Type
																              , const QByteArray&))
											            , new_contact
											            , SLOT(avatarUpdated(const QString&
													                         , const bb::platform::bbm::ImageType::Type
													                         , const QByteArray&)));
			Q_ASSERT(ok);
			m_contactService->requestDisplayPicture(new_contact->handle());
			m_contactsDataModel->replace(i, new_contact);
		}
	}
}
//! [3]
