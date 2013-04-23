/* Copyright (c) 2012 Research In Motion Limited.
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

#ifndef BUCKETSETTINGS_H_
#define BUCKETSETTINGS_H_

#include <QtCore/QObject>
#include <QSettings>

/**
 * BucketSettings Description:
 *
 * The Bucket list application settings object, used to store state parameters
 * between application sessions.
 */
class BucketSettings: public QObject
{
Q_OBJECT

/**
 * The property that decides if an action in the bucket list should be posted to the
 * BBM Social Platform. False initially.
 */
Q_PROPERTY(bool postToBBM READ postToBBM WRITE setPostToBBM NOTIFY postToBBMChanged)

/**
 * The property that controls if a UI for connecting to BBM should be shown. True initially
 * once the user has decided to connect or not it is set to false.
 */
Q_PROPERTY(bool connectToBBM READ connectToBBM WRITE setConnectToBBM NOTIFY connectToBBMChanged)

public:
    BucketSettings(QObject *parent = 0);
    ~BucketSettings();

    /**
     * Sets the property that decides if posting to BBM should be done or not.
     *
     * @param postToBBM If true application will communicate with the BBM app otherwise not
     */
    void setPostToBBM(bool postToBBM);

    /**
     * Returns the property deciding if posting to BBM should be done or not.
     *
     * @return True if posting should be done.
     */
    bool postToBBM();

    /**
     * Sets the property that decides if a UI for connecting to the BBM social platform should be shown.
     *
     * @param connectToBBM The boolean controlling if the UI should be shown or not
     */
    void setConnectToBBM(bool connectToBBM);

    /**
     * Returns the property that decides if a UI for connecting to the BBM social platform should be shown.
     *
     * @return True if the UI should be shown otherwise false
     */
    bool connectToBBM();

signals:

    /**
     *
     * @param postToBBM
     */
    void postToBBMChanged(bool postToBBM);

    /**
     *
     * @param connectToBBM
     */
    void connectToBBMChanged(bool connectToBBM);

private:
    // The Qt application settings object used for storing app data.
    QSettings mSettings;

    // Property variables
    bool mPostToBBM;
    bool mConnectToBBM;
};

#endif /* BUCKETSETTINGS_H_ */
