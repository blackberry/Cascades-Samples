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

/**
 * AppSettings Description
 *
 * This class handles application wide settings that persist between runs.
 */
class AppSettings: public QObject
{
    Q_OBJECT

    /**
     * The gravity property stores the setting for if the starship gravity should be on or off.
     */
    Q_PROPERTY(bool gravity READ gravity WRITE setGravity NOTIFY gravityChanged FINAL)

    /**
     * The powerDivert property tells where to divert power to (hyperdrive = 0 and sauna = 1).
     */
    Q_PROPERTY(int powerDivert READ powerDivert WRITE setPowerDivert NOTIFY powerDivertChanged FINAL)

    /**
     * The uranuscanner property used to store if scanning for Uranus or not.
     */
    Q_PROPERTY(bool uranuscanner READ uranuscanner WRITE setUranuscanner NOTIFY uranuscannerChanged FINAL)

    /**
     * This property contain the current setting of the warp drive.
     */
    Q_PROPERTY(float warpDriveSpeedScanner READ warpDriveSpeedScanner WRITE setWarpDriveSpeedScanner NOTIFY warpDriveSpeedScannerChanged)

public:
    AppSettings(QObject *parent = 0);

    /**
     * The gravity setting in the starship.
     *
     * @return True if gravity is on otherwise False
     */
    bool gravity() const;

    /**
     * The warp drive scanner setting.
     * @return A float representing the current setting of the warp engine.
     */
    int powerDivert() const;

    /**
     * The Uranus scanner.
     * @return True if the scanner is on otherwise False
     */
    bool uranuscanner() const;

    /**
     * The warp drive scanner setting.
     * @return A float representing the current setting of the warp engine.
     */
    float warpDriveSpeedScanner() const;

public slots:

    /**
     * Sets the gravity setting to on or off.
     * @param gravity The new value of the gravity setting.
     */
    void setGravity(bool gravity);

    /**
     * Sets where to divert power, and index that is interpreted by the application.
     * @param powerDivert The new value of the powerDivert.
     */
    void setPowerDivert(int powerDivert);

    /**
     * Sets the scanner to be on or off.
     * @param uranuscanner The new value of the scanner.
     */
    void setUranuscanner(bool uranuscanner);

    /**
     * Sets the current warp speed.
     * @param warpDriveSpeedScanner The current speed ranging from 0-1
     */
    void setWarpDriveSpeedScanner(float warpDriveSpeedScanner);

signals:

    /**
     * @brief Signal emitted when the value where to divert power to changes.
     * @param powerDivert The new value of the powerDivert.
     */
    void powerDivertChanged(int powerDivert);

    /**
     * @brief Signal emitted when the gravity setting changes.
     * @param gravity The new value of the gravity setting.
     */
    void gravityChanged(bool gravity);

    /**
     * @brief Signal emitted when the scanner is turned on and off.
     * @param uranuscanner The new value of the scanner setting.
     */
    void uranuscannerChanged(bool uranuscanner);

    /**
     * @brief Signal emitted when the speed is altered.
     * @param warpDriveSpeedScanner The new speed.
     */
    void warpDriveSpeedScannerChanged(float warpDriveSpeedScanner);

private:
    /**
     * Default values for properties
     */
    static const bool mDefaultGravity;
    static const int mDefaultPowerDivert;
    static const bool mDefaultUranusscanner;
    static const float mDefaultWarpDriveSpeedScanner;

    /**
     * The keys where the properties are stored in the QSettings object.
     */
    static const QString STARSHIP_GRAVITY_KEY;
    static const QString STARSHIP_POWERDIVERT_KEY;
    static const QString STARSHIP_URANUSSCANNER_KEY;
    static const QString STARSHIP_WARPDRIVESPEEDSCANNER_KEY;

    /**
     * The property variables.
     */
    bool mGravity;
    int mPowerDivert;
    bool mUranuscanner;
    float mWarpDriveSpeedScanner;
};

#endif /* APPSETTINGS_H_ */
