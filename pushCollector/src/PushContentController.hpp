/*!
* Copyright (c) 2012 Research In Motion Limited.
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

#ifndef PUSHCONTENTCONTROLLER_HPP
#define PUSHCONTENTCONTROLLER_HPP

#include <QObject>
#include <QVariant>

class Push;

/**
 * @short A controller class for the push content page
 *
 * This class is used as controller for the PushContentPage.
 * It provides the push date/time, content type and content
 * of the currently selected Push object through properties to
 * the UI.
 */
class PushContentController : public QObject
{
    Q_OBJECT

    // The content information of a Push object
    Q_PROPERTY(QString pushDateTime READ pushDateTime NOTIFY pushContentChanged)
    Q_PROPERTY(QString contentType READ contentType NOTIFY pushContentChanged)
    Q_PROPERTY(QString textContent READ textContent NOTIFY pushContentChanged)
    Q_PROPERTY(QVariant imageContent READ imageContent NOTIFY pushContentChanged)

public:
    PushContentController(QObject *parent = 0);

    // This method is called whenever the user selects a push from the listview in the UI.
    void setCurrentPush(const Push &push);

Q_SIGNALS:
    // The change notification signal of the properties
    void pushContentChanged();

private:
    // The accessor methods of the properties
    QString pushDateTime() const;
    QString contentType() const;
    QString textContent() const;
    QVariant imageContent() const;

    // The property values
    QString m_pushDateTime;
    QString m_contentType;
    QString m_textContent;
    QVariant m_imageContent;
};

#endif
