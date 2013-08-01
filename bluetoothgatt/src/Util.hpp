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

#ifndef UTIL_HPP
#define UTIL_HPP

typedef struct {
    int uuid;
    const char *name;
} uuidnames_t;

namespace Util {
const char *parse_characteristic_uuid(const char *uuid);
const char *parse_uuid(const char *uuid, const uuidnames_t uuidNames[]);
const char *parse_service_uuid(const char *uuid);
const char *parse_descriptor_uuid(const char *uuid);
}

#endif
