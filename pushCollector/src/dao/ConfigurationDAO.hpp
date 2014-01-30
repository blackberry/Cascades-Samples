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

#ifndef CONFIGURATIONDAO_HPP
#define CONFIGURATIONDAO_HPP

#define QSETTINGS_CONFIG_GROUP "configuration"
#define PROVIDER_APP_ID_KEY "providerAppId"
#define PPG_URL_KEY "ppgUrl"
#define PUSH_INITIATOR_URL_KEY "pushInitiatorUrl"
#define USING_PUBLIC_PPG_KEY "usingPublicPpg"
#define LAUNCH_APP_ON_PUSH_KEY "launchAppOnPush"

#define DEFAULT_PROVIDER_APP_ID ""
#define DEFAULT_PPG_URL ""
#define DEFAULT_PUSH_INITIATOR_URL ""
#define DEFAULT_USING_PUBLIC_PPG true
#define DEFAULT_LAUNCH_APP_ON_PUSH true

#include "../vo/Configuration.hpp"

/*!
 * DAO related to the configuration settings of the application.
 * Note: In our application, we expose the configuration settings to the user.
 * However, in an actual push application, you would likely have these settings
 * hard coded under the covers.  We expose them in our application for testing /
 * debugging purposes.
 */
class ConfigurationDAO
{
public:
    ConfigurationDAO();
    ~ConfigurationDAO();

    void save(const Configuration &config);

    void remove();

    Configuration& configuration();

    bool hasConfiguration() const;

private:
    Configuration m_config;
};

#endif
