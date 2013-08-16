/*!
* Copyright (c) 2012, 2013 Research In Motion Limited.
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

#ifndef MESSAGEWRITER_HPP
#define MESSAGEWRITER_HPP

#include "../dao/MessageDAO.hpp"

namespace pushcollector
{

/*!
 * Writes messages to the persistent store
 */
class PUSHCOLLECTOR_EXPORT MessageWriter
{
public:
	MessageWriter();
    ~MessageWriter();

    /*!
     * Saves the message in the persistent store
     */
    void write(const StatusMessage &statusMsg);

private:
    MessageDAO m_messageDAO;
};

} // namespace pushcollector



#endif /* MESSAGEWRITER_HPP */
