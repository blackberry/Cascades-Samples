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
#include <bb/data/SqlConnection>
#include <bb/data/SqlDataAccess>
#include <bb/system/SystemDialog>

#include <QtSql/QtSql>
#include <QDebug>

using namespace bb::cascades;
using namespace bb::system;
using namespace bb::data;
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
    database.setDatabaseName("./data/customerDatabase.db");

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
    QSqlQuery query(database);
    if (query.exec("DROP TABLE IF EXISTS customers")) {
        qDebug() << "Table dropped.";
    } else {
        const QSqlError error = query.lastError();
        alert(tr("Drop table error: %1").arg(error.text()));
    }

    // Create the 'customers' table that was just dropped (if it existed)
    // with no data
    const QString createSQL = "CREATE TABLE customers "
                              "  (customerID INTEGER PRIMARY KEY AUTOINCREMENT, "
                              "  firstName VARCHAR, "
                              "  lastName VARCHAR);";
    if (query.exec(createSQL)) {
        qDebug() << "Table created.";
    } else {
        const QSqlError error = query.lastError();
        alert(tr("Create table error: %1").arg(error.text()));
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

    // 2. Get the local DB connection. Note, called database()
    //    Will automatically open a connection to the database
    //    IMPORTANT NOTE: A QSqlQuery object can be created without a reference to
    //    the QSqlDatabase object (it will assume the default database connection
    //    if one is not provided), but, unlike a call to QSqlDatabase::database()
    //    it will not automatically open a connection to the database if it is
    //    currently closed.
    QSqlDatabase database = QSqlDatabase::database();

    // NOTE: Some applications might verify that the table being inserted to exists
    // at this point, however our application verifies all tables exist at application
    // startup.

    // 3. Create an QSqlQuery object with which you can execute queries
    //    In this example we bind parameters in the query. A large advantage to using
    //    bindings (aside from performance enhancements) is that input is automatically
    //    escaped avoiding potential issues with odd characters (quotes) and prevents
    //    SQL Injection attacks.
    //    Note that for databases that do not support bindings, Qt simulates the binding
    //    effects.
    //    IMPORTANT NOTE: If ever accepting user information without using bindings,
    //    be sure to 'escape' your queries.
    QSqlQuery query(database);
    query.prepare("INSERT INTO customers"
                  "    (firstName, lastName) "
                  "    VALUES (:firstName, :lastName)");
    query.bindValue(":firstName", firstName);
    query.bindValue(":lastName", lastName);

    // 4. Execute the query and check the result
    bool success = false;
    if (query.exec()) {
        alert(tr("Create record succeeded."));
        success = true;
    } else {
        // If 'exec' fails, error information can be accessed via the lastError function
        // the last error is reset every time exec is called.
        const QSqlError error = query.lastError();
        alert(tr("Create record error: %1").arg(error.text()));
    }

    // 5. Optionally close the database connection if we no longer plan to use it
    //    Note that calling QSqlDatabase::database() will automatically re-open
    //    the connection for us.
    //    NOTE: Closing the database invalidates any QSqlQuery objects you have created
    //    with this database connection.
    database.close();

    return success;
}

bool App::updateRecord(const QString &customerID, const QString &firstName, const QString &lastName)
{
    // 1. Verify the users input is valid.
    //    The SqlQuery's bind functionality will escape a users input ensuring that
    //    characters such as a quote will be properly accepted in the database and
    //    prevent Sql Injection attacks. However, this cannot be relied upon to validate
    //    all the data. In this case, the customerID is submitted as a string. The user
    //    might submit an empty id or characters. In these cases it is best to try to filter this input.
    //    The SqlQuery bindings do not protect your application from these kinds of inputs.
    //      IMPORTANT NOTE: In any application ALL user input should be filtered!
    bool intConversionGood = false;
    const int customerIDKey = customerID.toInt(&intConversionGood);
    if (!intConversionGood) {
        alert(tr("You must provide valid integer key."));
        return false;
    }

    // 2. Get the local DB connection. Note, called database()
    //    Will automatically open a connection to the database
    //    IMPORTANT NOTE: A QSqlQuery object can be created without a reference to
    //    the QSqlDatabase object (it will assume the default database connection
    //    if one is not provided), but, unlike a call to QSqlDatabase::database()
    //    it will not automatically open a connection to the database if it is
    //    currently closed.
    QSqlDatabase database = QSqlDatabase::database();

    // 3. Create an QSqlQuery object with which you can execute queries
    //    In this example we bind parameters in the query. A large advantage to using
    //    bindings (aside from performance enhancements) is that input is automatically
    //    escaped avoiding potential issues with odd characters (quotes) and prevents
    //    SQL Injection attacks.
    //    Note that for databases that do not support bindings, Qt simulates the binding
    //    effects.
    //    IMPORTANT NOTE: If ever accepting user information without using bindings,
    //    be sure to 'escape' your queries.
    QSqlQuery query(database);
    const QString sqlCommand = "UPDATE customers "
                               "    SET firstName = :firstName, lastName = :lastName"
                               "    WHERE customerID = :customerID";
    query.prepare(sqlCommand);

    // Note int he below bindings that firstName, lastName are strings, while customerIDKey
    // is an integer. The bindValue function is operator overloaded to accept multiple datatypes.
    query.bindValue(":firstName", firstName);
    query.bindValue(":lastName", lastName);
    query.bindValue(":customerID", customerIDKey);

    // 4. Execute the query and check the result for errors
    bool updated = false;
    if (query.exec()) {
        // 5. Verify that a row was modified, if not, there was no customer
        //    with the specified ID
        if (query.numRowsAffected() > 0) {
            alert(tr("Customer with id=%1 was updated.").arg(customerID));
            updated = true;
        } else {
            alert(tr("Customer with id=%1 was not found.").arg(customerID));
        }
    } else {
        alert(tr("SQL error: %1").arg(query.lastError().text()));
    }

    // 6. Optionally close the database connection if we no longer plan to use it
    //    Note that calling QSqlDatabase::database() will automatically re-open
    //    the connection for us.
    //    NOTE: Closing the database invalidates any QSqlQuery objects you have created
    //    with this database connection.
    database.close();

    return updated;
}

bool App::deleteRecord(const QString &customerID)
{
    // 1. Verify the users input is valid.
    //    The SqlQuery's bind functionality will escape a users input ensuring that
    //    characters such as a quote will be properly accepted in the database and
    //    prevent Sql Injection attacks. However, this cannot be relied upon to validate
    //    all the data. In this case, the customerID is submitted as a string. The user
    //    might submit an empty id. In these cases it is best to try to filter this input.
    //    The SqlQuery bindings do not protect your application from these kinds of inputs.
    //      IMPORTANT NOTE: In any application ALL user input should be filtered!
    bool intConversionGood = false;
    const int customerIDnumber = customerID.toInt(&intConversionGood);
    if (!intConversionGood) {
        alert(tr("You must provide valid integer key."));
        return false;
    }

    // 2. Get the local DB connection. Note, called database()
    //    Will automatically open a connection to the database
    //    IMPORTANT NOTE: A QSqlQuery object can be created without a reference to
    //    the QSqlDatabase object (it will assume the default database connection
    //    if one is not provided), but, unlike a call to QSqlDatabase::database()
    //    it will not automatically open a connection to the database if it is
    //    currently closed.
    QSqlDatabase database = QSqlDatabase::database(); // open the default database.

    // 3. Create an QSqlQuery object with which you can execute queries
    //    In this example we bind parameters in the query. A large advantage to using
    //    bindings (aside from performance enhancements) is that input is automatically
    //    escaped avoiding potential issues with odd characters (quotes) and prevents
    //    SQL Injection attacks.
    //    Note that for databases that do not support bindings, Qt simulates the binding
    //    effects.
    //    IMPORTANT NOTE: If ever accepting user information without using bindings,
    //    be sure to 'escape' your queries.
    QSqlQuery query(database);
    query.prepare("DELETE FROM customers WHERE customerID=:customerID");
    query.bindValue(":customerID", customerIDnumber);

    // 4. Execute the query and check the result for errors
    bool deleted = false;
    if (query.exec()) {
        // 5. Verify that a row was modified, if not, there was no customer
        //    with the specified ID
        if (query.numRowsAffected() > 0) {
            alert(tr("Customer with id=%1 was deleted.").arg(customerID));
            deleted = true;
        } else {
            alert(tr("Customer with id=%1 was not found.").arg(customerID));
        }
    } else {
        alert(tr("SQL error: %1").arg(query.lastError().text()));
    }

    // 6. Optionally close the database connection if we no longer plan to use it
    //    Note that calling QSqlDatabase::database() will automatically re-open
    //    the connection for us.
    //    NOTE: Closing the database invalidates any QSqlQuery objects you have created
    //    with this database connection.
    database.close();

    return deleted;
}
//! [2]
//! [3]
// Read all records from the database.
// Clear the data model and refill it.
void App::readRecords()
{
    // 1. Get the local DB connection. Note, called database()
    //    Will automatically open a connection to the database
    //    IMPORTANT NOTE: A QSqlQuery object can be created without a reference to
    //    the QSqlDatabase object (it will assume the default database connection
    //    if one is not provided), but, unlike a call to QSqlDatabase::database()
    //    it will not automatically open a connection to the database if it is
    //    currently closed.
    QSqlDatabase database = QSqlDatabase::database(); // opens the default database.

    // 2. Create a query to search for the records
    //    IMPORTANT NOTE: If accepting user input and not using bindings, be sure
    //    to escape it to allow quote characters, and prevent SQL Injection attacks.
    //    The below example is not a prepared statement and does not use bindings as
    //    there is no user input to accept.
    QSqlQuery query(database);
    const QString sqlQuery = "SELECT customerID, firstName, lastName FROM customers";

    // 3. Call 'exec' on the SQL Query. Note, that when using a SELECT action,
    //    the retrieved records are stored in the query and accessible by several
    //    different functions (see QSqlQuery documentation for more information).
    if (query.exec(sqlQuery)) {

        // Get the field indexes. We know the order of the fields, and could skip this step.
        // However this will still work if the fields change order in the query string.
        const int customerIDField = query.record().indexOf("customerID");
        const int firstNameField = query.record().indexOf("firstName");
        const int lastNameField = query.record().indexOf("lastName");

        // The data will be displayed in a group data model
        // Clear any previous reads from the data model first
        m_dataModel->clear();

        // 4. Start navigating through the records by calling the 'next' function.
        //    The next function will position the 'query' at the next record result
        //    allowing you to access the record data via the 'value' function.
        //    The next record will return true as long as it continues to point to valid
        //    record. When there are no longer any records it will return false.
        int recordsRead = 0;
        while (query.next()) {
            // 5. Access the data (stored in the query) via the field indexes
            //    and add the data to the model.
            //    NOTE: When adding an object to a DataModel, the DataModel sets
            //    itself as the parent of the object if no parent has already been
            //    set. Therefore, when clearing or removing an item from the data model
            //    the data model will destory the object if it is the parent of the object.
            Person *person = new Person(query.value(customerIDField).toString(),
                                        query.value(firstNameField).toString(),
                                        query.value(lastNameField).toString());
            m_dataModel->insert(person);
            recordsRead++;
        }
        qDebug() << "Read " << recordsRead << " records succeeded";

        if (recordsRead == 0) {
            alert(tr("The customer table is empty."));
        }
    } else {
        alert(tr("Read records failed: %1").arg(query.lastError().text()));
    }

    // 6. Optionally close the database connection if we no longer plan to use it
    //    Note that calling QSqlDatabase::database() will automatically re-open
    //    the connection for us.
    //    NOTE: Closing the database invalidates any QSqlQuery objects you have created
    //    with this database connection.
    database.close();
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
