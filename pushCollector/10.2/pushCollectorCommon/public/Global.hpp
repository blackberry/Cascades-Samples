/*!
* Copyright (c) 2012, 2013 BlackBerry Limited.
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

#ifndef PUSHCOLLECTOR_GLOBAL_HPP
#define PUSHCOLLECTOR_GLOBAL_HPP

#include <QtGlobal>

#if defined(PUSHCOLLECTOR_STATIC_LIB)
#   define BB_NETWORK_EXPORT
#else
#   if defined(PUSHCOLLECTOR_BUILD_LIB)
#       define PUSHCOLLECTOR_EXPORT    Q_DECL_EXPORT
#   else
#       define PUSHCOLLECTOR_EXPORT    Q_DECL_IMPORT
#   endif
#endif


#endif /* PUSHCOLLECTOR_GLOBAL_HPP */
