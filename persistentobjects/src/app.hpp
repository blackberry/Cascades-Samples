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

class Storage;

using namespace bb::cascades;

/*
 * @brief Declaration of our application's class (as opposed to the BB Cascades
 *  application class that contains our application).
 */
class App : public QObject
{
    // Classes that inherit from QObject must have the Q_OBJECT macro so
    // the meta-object compiler (MOC) can add supporting code to the application.
    Q_OBJECT

    Q_PROPERTY(bb::cascades::DataModel* dataModel READ dataModel CONSTANT)

public:
    // Describes the possible storage locations
    enum StorageLocations
    {
        StoreInQSettings, ///< objects are stored in QSettings
        StoreInFile       ///< objects are stored in custom files
    };

    // This allows the enum to be referred to in the qml file.
    // Note: the class also has to be registered using qmlRegisterType().
    // See the main.cpp file.
    Q_ENUMS(StorageLocations)

    // Creates a new App object
    App(QObject *parent = 0);

    // destroys the App object
    ~App();

    // Creates a new person object and saves it.
    Q_INVOKABLE bool addObject(const QString &firstName, const QString &lastName);

    // Read all the objects from the selected storage location and
    // put them in the data model
    Q_INVOKABLE void refreshObjects();

    // Remove all the objects from the selected storage location.
    Q_INVOKABLE void clearObjects();

    // Change the first and last name of the person with the
    // provided id. Update the data model and storage.
    Q_INVOKABLE bool updateObject(const QString &id, const QString &firstName, const QString &lastName);

    // Delete the person with the given id from the selected storage location.
    Q_INVOKABLE bool deleteObject(const QString &id);

    // Change the location we're using for the data, and
    // refresh the list.
    Q_INVOKABLE void setStorageLocation(StorageLocations strLocation);

private:
    void initDataModel();
    void addToDataModel(const QString &key, const QString &firstName, const QString &lastName);

    int load();

    // A helper method to check whether @p id contains a valid numeric id.
    bool validateID(const QString &id);

    // A helper method to show an alert dialog.
    void alert(const QString &message);

    bb::cascades::GroupDataModel* dataModel() const;

    // The data shown by the list view.
    GroupDataModel* m_dataModel;

    int m_lastCustomerID;
    StorageLocations m_storageLocation;
    Storage* m_storage;
};

#endif
