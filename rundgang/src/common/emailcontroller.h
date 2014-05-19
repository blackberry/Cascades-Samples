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
#ifndef EMAILCONTROLLER_H_
#define EMAILCONTROLLER_H_

#include <QObject>
#include <QString>
#include <QVariantMap>
#include <bb/cascades/pickers/ContactPicker>

namespace bb
{
    namespace cascades
    {
        namespace pickers
        {
            class ContactPicker;
            class SelectedContactAttribute;
        }
    }
}

namespace bb
{
    namespace system
    {
        class InvokeManager;
    }
}

/**
 * EmailController Description:
 *
 * Class that assist in setting up a ContactPicker for only
 * displaying contacts with email set on them. Also has a functionality
 * for getting emails from contact id:s and attributes.
 */
class EmailController: public QObject
{
    Q_OBJECT

public:
    EmailController(QObject *parent = 0);
    virtual ~EmailController();

    /**
     * Function for used for setting up email filtering on a ContactPicker Control.
     *
     * @param picker The picker for which the filter should be set up
     */
    Q_INVOKABLE void setupEmailFilter(bb::cascades::pickers::ContactPicker *picker);

    /**
     * Uses the ContactsService to get email addresses from a list of contact ids.
     *
     * @param contactIds A list of Contact ids for which emails should be retrieved
     * @return a list of emails for the contacts
     */
    Q_INVOKABLE QVariantList getEmailsForContacts(const QVariantList contactIds);

    /**
     * Uses the ContactsService to get the email address from contact with id.
     *
     * @param contactId The integer Contact id for which the email should be retrieved
     * @return  a list of emails for the contact
     */
    Q_INVOKABLE QVariantList getEmailForContact(int contactId);

    /**
     * Convenience function for converting a SelectedContactAttribute to a QString, which
     * can not be done in QML.
     *
     * @returns The email address if the value stored in the SelectedContactAttribute is of Email
     *          Attribute Kind, otherwise an empty string is returned.
     */
    Q_INVOKABLE QString getEmailForAttribute(bb::cascades::pickers::SelectedContactAttribute selected);

    /**
     * Invokable function that launch the hybrid email composer with an email
     * populated by the parameter data.
     *
     *  @param data The data object that will populate the email.
     */
    Q_INVOKABLE void invokeEmailComposer(QVariant data);

private:
    // Invoke manager object used to launch the email card.
    bb::system::InvokeManager *mInvokeManager;

};

#endif /* EMAILCONTROLLER_H_ */
