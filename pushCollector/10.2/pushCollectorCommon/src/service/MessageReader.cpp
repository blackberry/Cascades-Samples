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

#include "MessageReader.hpp"
#include "../PushUtility.hpp"
#include <QDebug>

namespace pushcollector
{

MessageReader::MessageReader(QObject *parent)
	: QObject(parent)
	, m_watcher(new QFileSystemWatcher(this))
{
	checkConnectResult(QObject::connect(m_watcher, SIGNAL(fileChanged(const QString&)),
	                this, SLOT(onFileChanged(const QString&))));

	// Create the message store if it doesn't exist
	// otherwise adding a file watch to it will fail
	if (!m_messageDAO.hasMessage()){
		m_messageDAO.save(StatusMessage());
		m_messageDAO.remove();
	}

	m_watcher->addPath(m_messageDAO.fileName());
}

MessageReader::~MessageReader()
{

}

StatusMessage& MessageReader::message()
{
    return m_messageDAO.message();
}

void MessageReader::onFileChanged(const QString &path)
{
	Q_UNUSED(path);

	if (!message().status().isEmpty()){
		emit messageReceived(message());
	}
}

bool MessageReader::hasMessage() const
{
    return m_messageDAO.hasMessage();
}

void MessageReader::remove()
{
	m_messageDAO.remove();
}

} // namespace pushcollector



