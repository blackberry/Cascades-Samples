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

#include <bb/cascades/GroupDataModel>

using namespace bb::cascades;

/*
 * @brief Declaration of our application's class (as opposed to the BB Cascades
 *  application class that contains our application).
 */
class App: public QObject
{
    // Classes that inherit from QObject must have the Q_OBJECT macro so
    // the meta-object compiler (MOC) can add supporting code to the application.
    Q_OBJECT

    // A property that is used by the list view in QML
    Q_PROPERTY(bb::cascades::DataModel* dataModel READ dataModel CONSTANT)

public:
    App();

    // "Q_INVOKABLE" allows these functions to be called from main.qml
    Q_INVOKABLE bool createRecord(const QString &firstName, const QString &lastName);
    Q_INVOKABLE void readRecords();
    Q_INVOKABLE bool updateRecord(const QString &key, const QString &firstName, const QString &lastName);
    Q_INVOKABLE bool deleteRecord(const QString &key);

private:
    // Functions to call upon initialization to setup the model and database
    void initDataModel();
    bool initDatabase();

    // To alert the user if something has gone wrong
    void alert(const QString &message);

    // The getter method for the property
    bb::cascades::GroupDataModel* dataModel() const;

    // The data shown by the list view.
    GroupDataModel* m_dataModel;
};

#endif
