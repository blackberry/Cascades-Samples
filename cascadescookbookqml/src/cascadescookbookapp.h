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
#ifndef _CASCADESCOOKBOOKAPPQML_H_
#define _CASCADESCOOKBOOKAPPQML_H_

#include <bb/cascades/Application>

using namespace bb::cascades;

/**
 * CascadesCookbookApp Description:
 *
 * A kitchen sink style demo with all available UI controls and settings for those controls.
 *
 */
class CascadesCookbookApp: public QObject
{
Q_OBJECT

public:
	
	/**
	 * Constructor; sets up the recipe.
	 */
    CascadesCookbookApp();

	/**
	 * Class destructor; nothing to delete, so not used.
	 */
    ~CascadesCookbookApp();

private:
    /**
     * Function used to create and set the SceneCover of the application,
     * the cover is shown when the app is running in minimized mode.
     */
    void addApplicationCover();
};

#endif // ifndef _CASCADESCOOKBOOKAPPQML_H_
