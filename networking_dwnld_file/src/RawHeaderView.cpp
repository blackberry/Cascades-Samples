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

#include "RawHeaderView.hpp"

RawHeaderView::RawHeaderView () : QObject()
{
    this->m_dialogVisible = false;
}

void RawHeaderView::setVisible(bool visible) {
    if (m_dialogVisible == visible)
        return;

    m_dialogVisible = visible;
    emit visibleChanged();
}

bool RawHeaderView::visible() const {
    return m_dialogVisible;
}

void RawHeaderView::show() {
    setVisible(true);
}

