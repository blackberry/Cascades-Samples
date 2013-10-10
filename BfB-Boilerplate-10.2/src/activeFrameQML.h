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
#ifndef ACTIVEFRAMEQML_H_
#define ACTIVEFRAMEQML_H_

#include <QObject>
#include <bb/cascades/SceneCover>
#include <bb/cascades/QmlDocument>

using namespace ::bb::cascades;

class ActiveFrameQML: public SceneCover {
Q_OBJECT

public:
	ActiveFrameQML(QObject *parent = 0);
	void setContextProperty(QString propertyName, QObject *object);
	Q_SIGNAL void updateChanged();

public slots:
	void update();
	void foregrounded();
	void backgrounded();

private:
	bb::cascades::QmlDocument *m_qml;
	bool isActiveFrame;
};

#endif /* ACTIVEFRAMEQML_H_ */
