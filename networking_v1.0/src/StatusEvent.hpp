#ifndef STATUSEVENT_HPP_
#define STATUSEVENT_HPP_

/* Copyright (c) 2013 BlackBerry Limited
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

#include <QObject>
#include <QString>
#include <bb/AbstractBpsEventHandler>
#include <bps/netstatus.h>

/* This class forms part of a sample app that demonstrates how to use the
 * Qt networking APIs. This class helps to report on the status of the
 * network connection, and the interface that it's using to make the
 * connection.
 *
 * This class uses BPS APIs to report whether the network connection was
 * made, and the interface used to make the network connection, which
 * can include Wifi, Cellular, BlueTooth, USB, BB, and others. When the
 * status of the network connection or the interface used to make that
 * connection changes, the networkStatusUpdated signal is emitted.
 */
class StatusEvent: public QObject, public bb::AbstractBpsEventHandler
{
	Q_OBJECT

public:
	StatusEvent();
	virtual ~StatusEvent();
	virtual void event(bps_event_t *event);

signals:
	void networkStatusUpdated(bool, QString);

private:
	netstatus_info_t* info;
};

#endif /* STATUSEVENT_HPP_ */
