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


#ifndef _BUCKETLISTAPP_H_
#define _BUCKETLISTAPP_H_

#include <bb/cascades/Application>

/**
 * BucketListApp description
 *
 * One hundred things to do before you kick the bucket. A relatively basic
 * list based application, which illustrates the usage of the multi select
 * handler. Moreover the application loads and saves data from a JSON file.
 *
 * You will learn:
 * - How to use the multiSelectHandler in order to select several list entries at once.
 * - How to one adds ActinoSets and contextActions to a list items.
 * - How to load and save data from a JSON file.
 *
 */
class BucketListApp: public QObject
{
    Q_OBJECT

public:
    BucketListApp();
};

#endif // ifndef _BUCKETLISTAPP_H_
