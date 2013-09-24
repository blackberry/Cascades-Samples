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
#include "emailcontroller.h"

#include <bb/cascades/Pickers/ContactPicker>
#include <bb/cascades/Pickers/ContactSelectionMode>
#include <bb/pim/contacts/ContactService>
#include <bb/pim/contacts/Contact>

#include <bb/system/InvokeManager>
#include <bb/system/InvokeTargetReply>

#include <bb/PpsObject>

#include <QDebug>

using namespace bb::cascades::pickers;
using namespace bb::pim::contacts;
using namespace bb::system;

EmailController::EmailController(QObject* parent) :
        QObject(parent)
{
    // InvokeManager used to launch the email hydbrid composer card.
    mInvokeManager = new InvokeManager(this);
}

EmailController::~EmailController()
{
}

void EmailController::setupEmailFilter(bb::cascades::pickers::ContactPicker* picker)
{

    // Set the picker to filter on emails.
    picker->setKindFilters(QSet<bb::pim::contacts::AttributeKind::Type>()
                    << bb::pim::contacts::AttributeKind::Email);
}

QVariantList EmailController::getEmailsForContacts(const QVariantList contactIds)
{
    QVariantList emailList;

    // Loop over all the contacts in the list and get the corresponding
    // email addresses.
    for (int i = 0; i < contactIds.length(); i++) {
        QVariant item = contactIds.at(i);
        int contactId = item.toInt();
        QVariantList contactList = getEmailForContact(contactId);
        emailList.append(contactList);
    }

    return emailList;
}

QVariantList EmailController::getEmailForContact(int contactId)
{
    QVariantList emailList;

    // Get the contact and its email list then iterate through it and
    // add all the emails to a list.
    Contact contact = ContactService().contactDetails(contactId);
    QList<ContactAttribute> email_list = contact.emails();
    foreach(ContactAttribute attr, email_list)
    {
        emailList << QVariant(attr.value());
    }

    return emailList;
}

QString EmailController::getEmailForAttribute(
        bb::cascades::pickers::SelectedContactAttribute selected)
{
    if (selected.attributeKind() == bb::pim::contacts::AttributeKind::Email) {
        return selected.value();
    }

    // Return an empty string if the attribute is not an email, this object is
    // designed to only handle emails.
    return "";
}


void EmailController::invokeEmailComposer(QVariant data)
{
    QVariantMap dataMap = data.toMap();
    InvokeRequest request;

    // Set up the Invocation request for an the email composer using
    // the incoming data.
    QString invokeTargetId = "sys.pim.uib.email.hybridcomposer";
    QString invokeActionId = "bb.action.COMPOSE";
    QString mimeType = "message/rfc822";

    request.setAction(invokeActionId);
    request.setTarget(invokeTargetId);
    request.setMimeType(mimeType);

    // The QVariantMap has to be encoded into a JSON format in order for the
    // email composer to handle it a PpsObject is used to encode it in the correct way.
    bool encodingResult;
    QVariantMap emailDataMap;
    emailDataMap["data"] = dataMap;
    request.setData(bb::PpsObject::encode(emailDataMap, &encodingResult));

    if (!encodingResult) {
        qDebug() << "Encoding of the email data failed";
        return;
    }

    // Finally make the invocation which will result in the an email being set up.
    mInvokeManager->invoke(request);
}
