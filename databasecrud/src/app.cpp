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

#include "Person.hpp"

#include <bb/cascades/AbstractPane>
#include <bb/cascades/Application>
#include <bb/cascades/QmlDocument>
#include <bb/data/SqlDataAccess>
#include <bb/data/DataAccessError>
#include <bb/system/SystemDialog>

#include <QtSql/QtSql>
#include <QDebug>

using namespace bb::cascades;
using namespace bb::data;
using namespace bb::system;

const QString DB_PATH = "./data/customerDatabase.db";

//! [0]
App::App()
    : m_dataModel(0)
{
    // Initialize the Group Data Model before setitng up our QML Scene
    // as the QML scene will bind to the data model
    initDataModel();

    // Create a QMLDocument from the definition in main.qml
    QmlDocument *qml = QmlDocument::create("asset:///main.qml").parent(this);

    //-- setContextProperty expose C++ object in QML as an variable
    // this must come before the next line so the root is instantiated after app is defined.
    qml->setContextProperty("_app", this);

    // Creates the root node object as defined in main.qml
    AbstractPane *root = qml->createRootObject<AbstractPane>();

    // Give the application the root node to display.
    Application::instance()->setScene(root);

    // Initialize the database, ensure a connection can be established
    // and that all the required tables and initial data exists
    const bool dbInited = initDatabase();

    // Inform the UI if the database was successfully initialized or not
    root->setProperty("databaseOpen", dbInited);
}
//! [0]
void App::initDataModel()
{
    // Note: The Group Data Model is joining this objects tree as a child (for memory management)
    m_dataModel = new GroupDataModel(this);
    m_dataModel->setSortingKeys(QStringList() << "customerID");
    m_dataModel->setGrouping(ItemGrouping::None);
}
//! [1]
bool App::initDatabase()
{
    // Initialize the database and create any tables needed for the app to function
    // properly if they do not already exist.
    // IMPORTANT NOTE: This function 'drops' the 'customers' table and recreates it
    // each time the application starts. This is done to ensure the application starts
    // with the same table each time for experimental purposes. This is not typical in
    // most applications however.
    QSqlDatabase database = QSqlDatabase::addDatabase("QSQLITE");
    database.setDatabaseName(DB_PATH);

    // If we cannot open a connection to the database, then fail initialization
    // and display an error message
    if (database.open() == false) {
        const QSqlError error = database.lastError();
        alert(tr("Error opening connection to the database: %1").arg(error.text()));
        qDebug() << "\nDatabase NOT opened.";
        return false; // return as if we cannot open a connection to the db, then below calls
                      // will also fail
    }

    // Drop the 'customers' table if it exists so that the application
    // always start with an empty table.
    // Note: A typical application would NOT do this.
    // open the default database.

    SqlDataAccess *sqlda = new SqlDataAccess(DB_PATH);
    sqlda->execute("DROP TABLE IF EXISTS customers");
    if(!sqlda->hasError()) {
        qDebug() << "Table dropped.";
    } else {
        const DataAccessError error = sqlda->error();
        alert(tr("Drop table error: %1").arg(error.errorMessage()));//.arg(error.text()));
    }

    // Create the 'customers' table that was just dropped (if it existed)
    // with no data
    const QString createSQL = "CREATE TABLE customers "
                              "  (customerID INTEGER PRIMARY KEY AUTOINCREMENT, "
                              "  firstName VARCHAR, "
                              "  lastName VARCHAR);";
    sqlda->execute(createSQL);
    if(!sqlda->hasError()) {
        qDebug() << "Table created.";
    } else {
        const DataAccessError error = sqlda->error();
        alert(tr("Create table error: %1").arg(error.errorMessage()));//.arg(error.text()));
        return false;
    }

    return true;
}
//! [1]
// -----------------------------------------------------------------------------------------------
// CRUD Functions
//! [2]
bool App::createRecord(const QString &firstName, const QString &lastName)
{
    // 1. Verify the users input is valid.
    //    The SqlQuery's bind functionality will escape a users input ensuring that
    //    characters such as a quote will be properly accepted in the database and
    //    prevent Sql Injection attacks. However, this cannot be relied upon to validate
    //    all the data. In this case, we ensure that at least the firstname OR lastname
    //    contains some form of text.
    if (firstName.trimmed().isEmpty() && lastName.trimmed().isEmpty()) {
        alert(tr("You must provide a first or last name."));
        return false;
    }

    // 2. Creating a SqlDataAccess instance automatically opens a connection
    //    to the database.
    SqlDataAccess *sqlda = new SqlDataAccess(DB_PATH);
    // NOTE: Some applications might verify that the table being inserted to exists
    // at this point, however our application verifies all tables exist at application
    // startup.

    // 3. Create an QVariantList to store your values which will be passed to the
    //    execution query by position. You can also use the other execution method
    //    which uses a QVariantMap and basically is the same as values bound by name.
    //    IMPORTANT NOTE: If ever accepting user information without using bindings,
    //    be sure to 'escape' your queries.
    QVariantList contact;
    contact << firstName << lastName;
    sqlda->execute("INSERT INTO customers"
                  "    (firstName, lastName) "
                  "    VALUES (:firstName, :lastName)", contact);

    // 4. Execute the query and check the result
    bool success = false;
    if(!sqlda->hasError()) {
        alert(tr("Create record succeeded."));
        success = true;
    } else {
        // If 'exec' fails, error information can be accessed via the error function
        // the last error is reset every time exec is called.
        const DataAccessError error = sqlda->error();
        alert(tr("Create record error: %1").arg(error.errorMessage()));
    }
    return success;
}

bool App::updateRecord(const QString &customerID, const QString &firstName, const QString &lastName)
{
    // 1. In this case, the customerID is submitted as a string. The user
    //    might submit an empty id or characters. In these cases it is best to try to filter this input.
    //      IMPORTANT NOTE: In any application ALL user input should be filtered!
    bool intConversionGood = false;
    const int customerIDKey = customerID.toInt(&intConversionGood);
    if (!intConversionGood) {
        alert(tr("You must provide valid integer key."));
        return false;
    }

    // 2. Creating a SqlDataAccess instance automatically opens a connection
    //    to the database.
    SqlDataAccess *sqlda = new SqlDataAccess(DB_PATH);

    // 3. Create an Sql query string which will be used to execute query.
    //    In this example we bind parameters in the query using a QVariantMap as the argument
    //    to the execute method. A large advantage of bindings (aside from performance enhancements)
    //    is that input is automatically
    //    escaped avoiding potential issues with odd characters (quotes) and prevents
    //    SQL Injection attacks.
    //    IMPORTANT NOTE: If ever accepting user information without using bindings,
    //    be sure to 'escape' your queries.
    const QString sqlCommand = "UPDATE customers "
                               "    SET firstName = :firstName, lastName = :lastName"
                               "    WHERE customerID = :customerID";
    QVariantMap bindings;
    bindings["firstName"] = firstName;
    bindings["lastName"] = lastName;
    bindings["customerID"] = customerIDKey;

    // 4. Execute the query and check the result for errors
    bool updated = false;
    sqlda->execute(sqlCommand, bindings);
    if (!sqlda->hasError()) {
        // 5. Verify that a customer with that ID exists.
        const QString sqlVerify = "SELECT firstName FROM customers WHERE customerID = :customerID";
        QVariantList args;
        args << customerIDKey;
        QVariant result = sqlda->execute(sqlVerify, args);
        if (!result.isNull() && result.value<QVariantList>().size() > 0) {
            alert(tr("Customer with id=%1 was updated.").arg(customerID));
            updated = true;
        } else {
            alert(tr("Customer with id=%1 was not found.").arg(customerID));
        }
    } else {
        alert(tr("SQL error: %1").arg(sqlda->error().errorMessage()));
    }

    return updated;
}

bool App::deleteRecord(const QString &customerID)
{
    // 1. In this case, the customerID is submitted as a string. The user
    //    might submit an empty id. In these cases it is best to try to filter this input.
    //    IMPORTANT NOTE: In any application ALL user input should be filtered!
    bool intConversionGood = false;
    const int customerIDnumber = customerID.toInt(&intConversionGood);
    if (!intConversionGood) {
        alert(tr("You must provide valid integer key."));
        return false;
    }

    // 2. Creating a SqlDataAccess instance automatically opens a connection
    //    to the database.
    SqlDataAccess *sqlda = new SqlDataAccess(DB_PATH);

    // Verify the customer with ID exists before deleting row. This way we can check
    // afterwards that deletion was successful.
    const QString sqlVerify = "SELECT firstName FROM customers WHERE customerID = :customerID";
    QVariantList argsList;
    argsList << customerIDnumber;
    QVariant verificationResult = sqlda->execute(sqlVerify, argsList);
    if(verificationResult.isNull() || verificationResult.value<QVariantList>().size() == 0) {
        alert(tr("Customer with id=%1 was not found.").arg(customerID));
        return false;
    }

    // 3. Create an Sql query string which will be used to execute query.
    //    In this example we bind parameters in the query using a QVariantMap as the argument
    //    to the execute method. A large advantage of bindings (aside from performance enhancements)
    //    is that input is automatically
    //    escaped avoiding potential issues with odd characters (quotes) and prevents
    //    SQL Injection attacks.
    //    IMPORTANT NOTE: If ever accepting user information without using bindings,
    //    be sure to 'escape' your queries.
    QVariantMap bindings;
    bindings["customerID"] = customerIDnumber;
    sqlda->execute("DELETE FROM customers WHERE customerID=:customerID", bindings);



    // 4. Execute the query and check the result for errors
    bool deleted = false;
    if (!sqlda->hasError()) {
        verificationResult = sqlda->execute(sqlVerify, argsList);
        if (!verificationResult.isNull() && verificationResult.value<QVariantList>().size() == 0) {
            alert(tr("Customer with id=%1 was deleted.").arg(customerID));
            deleted = true;
        } else {
            alert(tr("Customer with id=%1 was not found.").arg(customerID));
        }
    } else {
        alert(tr("SQL error: %1").arg(sqlda->error().errorMessage()));
    }

    return deleted;
}
//! [2]
//! [3]
// Read all records from the database.
// Clear the data model and refill it.
void App::readRecords()
{
    // 1. Create the local DB connection via SqlDataAccess instance. Note, creating instance
    //    Will automatically open a connection to the database.
    SqlDataAccess *sqlda = new SqlDataAccess(DB_PATH);

    // 2. Create a query to search for the records
    //    IMPORTANT NOTE: If accepting user input and not using bindings, be sure
    //    to escape it to allow quote characters, and prevent SQL Injection attacks.
    //    The below example is not a prepared statement and does not use bindings as
    //    there is no user input to accept.

    const QString sqlQuery = "SELECT customerID, firstName, lastName FROM customers";

    // 3. Call 'execute' on the SqlDataAccess. Note, that when using a SELECT action,
    //    the retrieved records are stored in the QVariantList as QVariantMap objects.
    QVariant result = sqlda->execute(sqlQuery);
    if (!sqlda->hasError()) {
        int recordsRead = 0;
        // The data will be displayed in a group data model
        // Clear any previous reads from the data model first
        m_dataModel->clear();
        if( !result.isNull() ) {
            // If the query is successful and results are not null
            // you can access the records through the QVariantList
            // by accessing the QVariantMap object at each index for the records values.
            // ImportantNote: The alternative is to add to the model directly exposing the records values for reference via qml.
            //                You essentially get the same result as with the Person object but with less hassle.
            // m_dataModel->insertList(result.value<QVariantList>());
            QVariantList list = result.value<QVariantList>();
            recordsRead = list.size();
            for(int i = 0; i < recordsRead; i++) {
                QVariantMap map = list.at(i).value<QVariantMap>();
                Person *person = new Person(map["customerID"].toString(),
                                 map["firstName"].toString(),
                                 map["lastName"].toString());
                Q_UNUSED(person);
                //NOTE: When adding an object to a DataModel, the DataModel sets
                //    itself as the parent of the object if no parent has already been
                //    set. Therefore, when clearing or removing an item from the data model
                //    the data model will destroy the object if it is the parent of the object.
                m_dataModel->insert(person);
            }
        }

        qDebug() << "Read " << recordsRead << " records succeeded";

        if (recordsRead == 0) {
            alert(tr("The customer table is empty."));
        }
    } else {
        alert(tr("Read records failed: %1").arg(sqlda->error().errorMessage()));
    }
}
//! [3]
GroupDataModel* App::dataModel() const
{
    return m_dataModel;
}

// -----------------------------------------------------------------------------------------------
// Alert Dialog Box Functions
void App::alert(const QString &message)
{
    SystemDialog *dialog; // SystemDialog uses the BB10 native dialog.
    dialog = new SystemDialog(tr("OK"), 0); // New dialog with on 'Ok' button, no 'Cancel' button
    dialog->setTitle(tr("Alert")); // set a title for the message
    dialog->setBody(message); // set the message itself
    dialog->setDismissAutomatically(true); // Hides the dialog when a button is pressed.

    // Setup slot to mark the dialog for deletion in the next event loop after the dialog has been accepted.
    bool ok = connect(dialog, SIGNAL(finished(bb::system::SystemUiResult::Type)), dialog, SLOT(deleteLater()));
    Q_ASSERT(ok);
    dialog->show();
}
