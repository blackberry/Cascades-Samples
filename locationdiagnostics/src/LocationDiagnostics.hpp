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

#ifndef LOCATIONDIAGNOSTICS_HPP
#define LOCATIONDIAGNOSTICS_HPP

#include <QtCore/QObject>
#include <bb/cascades/controls/page.h>
#include <bb/cascades/TitleBar>

class LocationSession;

using namespace bb::cascades;


//! [0]
class LocationDiagnostics : public QObject
{
    Q_OBJECT

    // The properties to configure the geo location sources
    Q_PROPERTY(QString positionMethod READ positionMethod WRITE setPositionMethod NOTIFY positionMethodChanged)
    Q_PROPERTY(QString assistanceMode READ assistanceMode WRITE setAssistanceMode NOTIFY assistanceModeChanged)

    Q_PROPERTY(QString pdeUrl READ pdeUrl WRITE setPdeUrl NOTIFY pdeUrlChanged)
    Q_PROPERTY(QString slpUrl READ slpUrl WRITE setSlpUrl NOTIFY slpUrlChanged)
    Q_PROPERTY(QString appId READ appId WRITE setAppId NOTIFY appIdChanged)
    Q_PROPERTY(QString password READ password WRITE setPassword NOTIFY passwordChanged)

    Q_PROPERTY(int frequency READ frequency WRITE setFrequency NOTIFY frequencyChanged)
    Q_PROPERTY(bool useSound READ useSound WRITE setUseSound NOTIFY useSoundChanged)
    Q_PROPERTY(bool backgroundMode READ backgroundMode WRITE setBackgroundMode NOTIFY backgroundModeChanged)

public:
    LocationDiagnostics(QObject *parent = 0);

    /**
     * This method creates a new location session.
     */
    Q_INVOKABLE LocationSession* createLocationSession(bool trackingMode, QObject *var);


Q_SIGNALS:
    // The change notification signals of the properties
    void positionMethodChanged();
    void assistanceModeChanged();
    void pdeUrlChanged();
    void slpUrlChanged();
    void appIdChanged();
    void passwordChanged();
    void frequencyChanged();
    void useSoundChanged();
    void backgroundModeChanged();

private Q_SLOTS:
	void onManualExit();

private:
    // The accessor methods of the properties
    QString positionMethod() const;
    void setPositionMethod(const QString &method);
    QString assistanceMode() const;
    void setAssistanceMode(const QString &mode);
    QString pdeUrl() const;
    void setPdeUrl(const QString &url);
    QString slpUrl() const;
    void setSlpUrl(const QString &url);
    QString appId() const;
    void setAppId(const QString &appId);
    QString password() const;
    void setPassword(const QString &password);
    int frequency() const;
    void setFrequency(int frequency);
    bool useSound() const;
    void setUseSound(bool sound);
    bool backgroundMode() const;
    void setBackgroundMode(bool mode);

    // The property values
    QString m_positionMethod;
    QString m_assistanceMode;
    QString m_pdeUrl;
    QString m_slpUrl;
    QString m_appId;
    QString m_password;
    int m_frequency;
    bool m_useSound;
    bool m_backgroundMode;

};
//! [0]

#endif
