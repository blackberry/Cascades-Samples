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

#ifndef APP_HPP
#define APP_HPP

#include <QtCore/QObject>

/**
 * The main business logic object in the application.
 *
 * This object normally provides signals, slots and properties
 * that are accessed by the main page of the application's UI.
 */
//! [0]
class App : public QObject
{
    Q_OBJECT

    // Add your Q_PROPERTY declarations here.

public:
    App(QObject *parent = 0);

public Q_SLOTS:
    void show();

    // Add your slot declarations here.

Q_SIGNALS:
    // Add your signal declarations here.
};
//! [0]

#endif
