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
#ifndef APPSETTINGS_H_
#define APPSETTINGS_H_

#include <QObject>
#include <QVariantMap>
#include <QUrl>
#include <bb/data/SqlConnection>

/**
 * AppSettings Description
 *
 * This class handles application wide settings that persist between runs.
 */
class AppSettings: public QObject
{
Q_OBJECT

    /**
     * The currently selected home location. A QVariantMap with "city" and "region" as keywords.
     */
    Q_PROPERTY(QString home READ home WRITE setHome NOTIFY homeChanged FINAL)


    /**
     * The application home directory property, used to find the path to the
     * local database.
     */
    Q_PROPERTY( QString homeDir READ homeDir CONSTANT FINAL)

public:
    AppSettings(QObject *parent = 0);
    virtual ~AppSettings();

    /**
     * Returns the city that is currently stored in the data base as the "Home" town.
     *
     * @return the current home city.
     */
    QString home();

    /**
     * Sets the home city and stores it to persistent store.
     *
     * @param home The new home city.
     */
    Q_SLOT void setHome(const QString home);

    /**
     * This function return the path to the application home directory.
     *
     * @return A QString with the path to the application home directory.
     */
    Q_SLOT QString homeDir();

signals:
    /**
     * Signal emitted when the home city changes.
     *
     * @param home The new home city.
     */
    void homeChanged(QString home);

private:

    /**
     * Default values for main page data
     */
    static const QString mDefaultHomeCity;

    /**
     * The memberVariable for the home location.
     */
    QString mHomeCity;

    /**
     * Data base connector
     */
    bb::data::SqlConnection *mSqlConnector;
};

#endif /* APPSETTINGS_H_ */
