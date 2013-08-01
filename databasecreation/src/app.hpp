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

#ifndef APP_HPP
#define APP_HPP

#include <QObject>
#include <bb/data/SqlConnection>

/*
 * @brief Declaration of our application's class (as opposed to the BB Cascades
 *  application class that contains our application).
 */

class App: public QObject
{
    // Classes that inherit from QObject must have the Q_OBJECT macro so
    // the meta-object compiler (MOC) can add supporting code to the application.
    Q_OBJECT

public:
    App();

    // "Q_INVOKABLE" allows these functions to be called from main.qml
    Q_INVOKABLE bool createDatabase();
    Q_INVOKABLE void dropTable();
    Q_INVOKABLE void createTable();
    Q_INVOKABLE void createRecord(const QString &firstName, const QString &lastName);
    Q_INVOKABLE void createTableAsync(); // This is an example of how you make asynchronous calls to the database.

private slots:
    // This is the callback used for executing asynchronous queries.
    void onCreateTableReply(const bb::data::DataAccessReply &reply);

private:
    // Helper method to show a alert dialog
    void alert(const QString &message);

    // The connection to the SQL database
    bb::data::SqlConnection* m_sqlConnection;
};

#endif
