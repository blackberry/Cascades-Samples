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
#ifndef WEBMAPS_HPP
#define WEBMAPS_HPP

#include <QObject>
#include <QMap>
#include <QVariant>

//! [0]
class WebMaps : public QObject
{
    Q_OBJECT
    Q_ENUMS(Provider)

    // The map provider that is currently used
    Q_PROPERTY(Provider currentProvider READ currentProvider WRITE setCurrentProvider NOTIFY currentProviderChanged)

    // The html page content for the current map provider
    Q_PROPERTY(QString pageContent READ pageContent NOTIFY currentProviderChanged)

    // The provider specific view mode title
    Q_PROPERTY(QString viewModeTitle READ viewModeTitle NOTIFY viewModeChanged)

    // The provider specific view mode identifier
    Q_PROPERTY(QString viewMode READ viewMode NOTIFY viewModeChanged)

public:
    // Available map providers
    enum Provider {
        GoogleMaps,
        BingMaps,
        OpenLayers
    };

    WebMaps(QObject *parent = 0);

public slots:
    void nextViewMode();

signals:
    // The change notification signals of the properties
    void currentProviderChanged();
    void viewModeChanged();

private:
    // The accessor methods of the properties
    Provider currentProvider() const;
    void setCurrentProvider(Provider provider);
    QString pageContent() const;
    QString viewModeTitle() const;
    QString viewMode() const;

    Provider m_currentProvider;

    QMap<Provider, QVariantMap> m_providerData;
    int m_currentViewModeIndex;
};
//! [0]

#endif
