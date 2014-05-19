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
#include "appsettings.h"
#include "globalconnection.h"

#include <QDir>
#include <bb/data/DataAccessReply>

const QString AppSettings::mDefaultHomeCity("London");

AppSettings::AppSettings(QObject* parent) : QObject(parent)

{
    // Set up SQL connection for accessing settings database, used for storing home in order to use
    // it for joining with weather data table
    mSqlConnector = GlobalConnection::instance()->sqlConnection();

    // Get the home location from the database.
    QVariantList dataList;
    dataList << "home";
    bb::data::DataAccessReply reply =  mSqlConnector->executeAndWait ("SELECT * FROM settings WHERE key=:home", dataList);

    if(!reply.hasError()) {
        QVariantList resultList = reply.result().toList();
        QVariantMap resultMap = resultList.at(0).toMap();
        mHomeCity = resultMap["value"].toString();
    } else {
        qWarning() << "AppSettings: Home location could not be read from database, setting default.";
        mHomeCity = mDefaultHomeCity;
    }
}

AppSettings::~AppSettings()
{
}

QString AppSettings::home()
{
    return mHomeCity;
}

void AppSettings::setHome(QString home)
{
    if (home != mHomeCity) {
        // Store the home location in the data base in order to join it with weather data.
        QVariantList dataList;
        dataList << home << "home";
        QString query = "UPDATE settings SET value=:city WHERE key=:home";
        bb::data::DataAccessReply reply =  mSqlConnector->executeAndWait (query, dataList);

        if(reply.hasError()) {
            qWarning() << "AppSettings: ERROR Home location could not be read from database.";
            return;
        }

        mHomeCity = home;
        emit homeChanged(mHomeCity);
    }
}

QString AppSettings::homeDir()
{
    return QDir::homePath();
}
