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

#include "ConfigurationService.hpp"

ConfigurationService::ConfigurationService()
{
}

ConfigurationService::~ConfigurationService()
{

}

void ConfigurationService::save(const Configuration &config)
{
    m_configDAO.save(config);
}

void ConfigurationService::remove()
{
    m_configDAO.remove();
}

Configuration& ConfigurationService::configuration()
{
    return m_configDAO.configuration();
}

bool ConfigurationService::hasConfiguration() const
{
    return m_configDAO.hasConfiguration();
}
