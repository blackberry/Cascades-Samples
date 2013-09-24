/* Copyright (c) 2012, 2013  BlackBerry Limited.
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
#include "app.hpp"

#include "FileStorage.hpp"
#include "Person.hpp"
#include "SettingsStorage.hpp"
#include "Storage.hpp"

#include <bb/cascades/AbstractPane>
#include <bb/cascades/Application>
#include <bb/cascades/Page>
#include <bb/cascades/QmlDocument>
#include <bb/system/SystemDialog>

#include <QDebug>

using namespace bb::cascades;
using namespace bb::system;
//! [0]
App::App(QObject *parent)
    : QObject(parent)
    , m_lastCustomerID(0)
    , m_storageLocation(StoreInQSettings)
    , m_storage(new SettingsStorage)
{
    // Initialize the data model before the UI is loaded
    // and built so its ready to be used.
    initDataModel();

    QmlDocument *qml = QmlDocument::create("asset:///main.qml").parent(this);
    qml->setContextProperty("_app", this);

    AbstractPane *root = qml->createRootObject<AbstractPane>();
    Application::instance()->setScene(root);
}
//! [0]
App::~App()
{
    delete m_storage;
}

//! [1]
// Create a unique identifier and save the person information
// to the data model and store the datamodel in the
// selected storage location.
bool App::addObject(const QString &firstName, const QString &lastName)
{
    bool added = false;

    m_lastCustomerID++;
    const QString id = QString::number(m_lastCustomerID);

    // Note: The person object will be destroyed
    // when the data model is destroyed.
    Person *person = new Person(id, firstName, lastName);
    // To keep the person in memory assign it a parent in
    // the parameter list like this:
    //         Person *person = new Person(id, firstName, lastName, parent);

    m_dataModel->insert(person);

    added = m_storage->save(m_lastCustomerID, m_dataModel);

    return added;
}
//! [1]
//! [2]
// Read all objects from file.
// Clear the data model and refill it.
void App::refreshObjects()
{
    const int objectsReadCount = load();

    if (objectsReadCount == 0) {
        alert(tr("The customer list is empty."));
    } else {
        alert(tr("%1 objects loaded.").arg(objectsReadCount));
    }
}
//! [2]
// Clear all the objects in the current storage location.
void App::clearObjects()
{
    m_lastCustomerID = 0;
    m_dataModel->clear();
    m_storage->clear();
}

//! [4]
// Change the first and last name of the person with the given id.
bool App::updateObject(const QString &id, const QString &firstName, const QString &lastName)
{
    bool updated = false;
    bool saved = false;

    if (!validateID(id))
        return false;

    Person *person = new Person(id, firstName, lastName);

    // Find person in the datamodel.
    // Only the id is used by find(). This is because Person
    // defines equality (==) as having the same id. (See the definition of "=="
    // in the person class.)
    const QVariantList updateIndexPath = m_dataModel->find(person);

    // update the item if found
    if (updateIndexPath.isEmpty()) {
        alert(tr("Object ID not found."));
        updated = false;
    } else {
        updated = m_dataModel->updateItem(updateIndexPath, person);
    }

    // Save the datamodel if we updated something.
    if (updated) {
        saved = m_storage->save(m_lastCustomerID, m_dataModel);
    }

    return (updated && saved);
}

bool App::deleteObject(const QString &customerID)
{
    bool deleted = false;
    bool saved = false;

    if (!validateID(customerID))
        return false;

    // Create a person object with the required id.
    // The name can be left out because find() will use the == operator
    // defined in the Person class. See Person.cpp
    Person *person = new Person(customerID, QString(), QString());

    const QVariantList deleteIndexPath = m_dataModel->find(person);

    if (deleteIndexPath.isEmpty()) {
        alert(tr("Object ID not found."));
    } else {
        deleted = m_dataModel->removeAt(deleteIndexPath);
    }

    if (deleted) {
        saved = m_storage->save(m_lastCustomerID, m_dataModel);
    }

    return (deleted && saved);
}
//! [4]
//! [5]
void App::setStorageLocation(StorageLocations location)
{
    // If location changed, then reload the data model
    // to get the objects there.
    if (m_storageLocation != location) {
        m_storageLocation = location;

        delete m_storage;
        if (m_storageLocation == StoreInQSettings) {
            m_storage = new SettingsStorage();
        } else {
            m_storage = new FileStorage();
        }

        refreshObjects();
    }
}
//! [5]
void App::initDataModel()
{
    // Create a data model sorted by customer id.
    m_dataModel = new GroupDataModel(this);
    m_dataModel->setSortingKeys(QStringList() << "customerID");
    m_dataModel->setGrouping(ItemGrouping::None);

    load();
}

void App::addToDataModel(const QString &id, const QString &firstName, const QString &lastName)
{
    m_dataModel->insert(new Person(id, firstName, lastName));
}
//! [3]
int App::load()
{
    m_dataModel->clear();

    const int loaded = m_storage->load(m_lastCustomerID, m_dataModel);
    return loaded;
}
//! [3]
bool App::validateID(const QString &customerID)
{
    // Check if the user entered something.
    // In a real application you might have stricter criteria.
    bool intConversionGood = false;

    customerID.toInt(&intConversionGood);
    if (!intConversionGood) {
        alert(tr("You must provide an integer key."));
    }

    return intConversionGood;
}

GroupDataModel* App::dataModel() const
{
    return m_dataModel;
}

// Show an alert dialog.
void App::alert(const QString &message)
{
    qDebug() << "alert: " << message;
    SystemDialog *dialog; // SystemDialog uses the BB10 native dialog.
    dialog = new SystemDialog(tr("OK"), 0); // New dialog with on 'Ok' button, no 'Cancel' button
    dialog->setTitle(tr("Alert")); // set a title for the message
    dialog->setBody(message); // set the message itself
    dialog->setDismissAutomatically(true); // Hides the dialog when a button is pressed.

    // Setup slot to mark the dialog for deletion in the next event loop after the dialog has been accepted.
    bool ok = connect(dialog, SIGNAL(finished(bb::system::SystemUiResult::Type)), dialog, SLOT(deleteLater()));
    Q_ASSERT(ok);
    Q_UNUSED(ok);
    dialog->show();
}
