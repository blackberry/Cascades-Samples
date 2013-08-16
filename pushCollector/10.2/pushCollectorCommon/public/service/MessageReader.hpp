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

#ifndef MESSAGEREADER_HPP
#define MESSAGEREADER_HPP

#include "../dao/MessageDAO.hpp"
#include <QFileSystemWatcher>

namespace pushcollector
{

/*!
 * Reads messages from the persistent store
 */
class PUSHCOLLECTOR_EXPORT MessageReader : public QObject
{
	 Q_OBJECT

public:
	MessageReader(QObject *parent = 0);
    ~MessageReader();

    /*!
     * Returns the message in the persistent store
     */
    StatusMessage& message();

    /*!
     * Returns whether there is a message already in the persistent store
     */
    bool hasMessage() const;

    /*!
     * Removes the message in the persistent store
     */
    void remove();

public Q_SLOTS:
	void onFileChanged(const QString &path);

Q_SIGNALS:
	void messageReceived(const StatusMessage &statusMsg);

private:
    MessageDAO m_messageDAO;
    QFileSystemWatcher *m_watcher;
};

} // namespace pushcollector


#endif /* MESSAGEREADER_HPP */
