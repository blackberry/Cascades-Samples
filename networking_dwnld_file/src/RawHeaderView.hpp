/*
 * Copyright (c) 2011-2014 BlackBerry Limited.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied.
 * See the License for the specific language governing permissions
 * and limitations under the License.
 */

#ifndef RAWHEADERVIEW_HPP_
#define RAWHEADERVIEW_HPP_

#include <QObject>
#include <QtNetwork>

class RawHeaderView : public QObject
{
        Q_OBJECT

        Q_PROPERTY(bool visible READ visible WRITE setVisible NOTIFY visibleChanged)

    public:
        RawHeaderView ();
        virtual ~RawHeaderView (){}

        void setVisible(bool visible);
        bool visible() const;

    public Q_SLOTS:
        void show();

    signals:
        void visibleChanged();

    private:
        bool m_dialogVisible;
};

#endif /* RAWHEADERVIEW_HPP_ */
