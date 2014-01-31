/*!
* Copyright (c) 2012, 2013  BlackBerry Limited.
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

#include "ConfigurationDAO.hpp"

#include <QSettings>
#include <QStringList>

ConfigurationDAO::ConfigurationDAO()
{

}

ConfigurationDAO::~ConfigurationDAO()
{

}

void ConfigurationDAO::save(const Configuration &config)
{
    QSettings settings;
    settings.beginGroup(QSETTINGS_CONFIG_GROUP);
    settings.setValue(PPG_URL_KEY, config.ppgUrl());
    settings.setValue(PROVIDER_APP_ID_KEY, config.providerApplicationId());
    settings.setValue(PUSH_INITIATOR_URL_KEY, config.pushInitiatorUrl());
    settings.setValue(USING_PUBLIC_PPG_KEY, config.usingPublicPushProxyGateway());
    settings.setValue(LAUNCH_APP_ON_PUSH_KEY, config.launchApplicationOnPush());
    settings.endGroup();
}

void ConfigurationDAO::remove()
{
    QSettings settings;
    settings.remove(QSETTINGS_CONFIG_GROUP);
}

Configuration& ConfigurationDAO::configuration()
{
    QSettings settings;

    settings.beginGroup(QSETTINGS_CONFIG_GROUP);
    m_config.setPpgUrl(settings.value(PPG_URL_KEY, DEFAULT_PPG_URL).toString());
    m_config.setProviderApplicationId(settings.value(PROVIDER_APP_ID_KEY, DEFAULT_PROVIDER_APP_ID).toString());
    m_config.setPushInitiatorUrl(settings.value(PUSH_INITIATOR_URL_KEY, DEFAULT_PUSH_INITIATOR_URL).toString());
    m_config.setUsingPublicPushProxyGateway(settings.value(USING_PUBLIC_PPG_KEY, DEFAULT_USING_PUBLIC_PPG).toBool());
    m_config.setLaunchApplicationOnPush(settings.value(LAUNCH_APP_ON_PUSH_KEY, DEFAULT_LAUNCH_APP_ON_PUSH).toBool());
    settings.endGroup();

    return m_config;
}

bool ConfigurationDAO::hasConfiguration() const
{
    int keysCount = 0;

    QSettings settings;
    settings.beginGroup(QSETTINGS_CONFIG_GROUP);
    keysCount = settings.allKeys().count();
    settings.endGroup();

    return keysCount > 0;
}
