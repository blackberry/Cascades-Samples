/* Copyright (c) 2013 Research In Motion Limited.
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
#include "screensize.h"
#include <bb/cascades/OrientationSupport>

using namespace bb::cascades;

ScreenSize::ScreenSize(QObject* parent) :
        QObject(parent), mDisplayInfo(this)
{
    init();
}

ScreenSize::ScreenSize(int displayId, QObject* parent) :
        QObject(parent), mDisplayInfo(displayId)
{
    init();
}

ScreenSize::~ScreenSize()
{
}

void ScreenSize::init()
{
    mDisplayRes = mRes = mDisplayInfo.pixelSize();
    mDisplayDirection = OrientationSupport::instance()->displayDirection();

    emit resChanged(mRes);
    emit widthChanged(mRes.width());
    emit heightChanged(mRes.height());
    emit displayDirectionChanged(mDisplayDirection);

    QObject::connect(&mDisplayInfo, SIGNAL(pixelSizeChanged(QSize)), this,
            SLOT(onSizeChanged(QSize)));
    QObject::connect(OrientationSupport::instance(),
            SIGNAL(displayDirectionChanged(bb::cascades::DisplayDirection::Type)), this,
            SLOT(onDisplayDirectionChanged(bb::cascades::DisplayDirection::Type)));
}

void ScreenSize::onSizeChanged(QSize res)
{
    mDisplayRes = res;
    update(mDisplayRes, mDisplayDirection);
}

void ScreenSize::onDisplayDirectionChanged(DisplayDirection::Type direction)
{
    update(mDisplayRes, direction);
}

void ScreenSize::update(QSize res, DisplayDirection::Type direction)
{
    if ((direction == 90) || (direction == 270)) {
        res = QSize(res.height(), res.width());
    }
    bool w = (res.width() != mRes.width());
    bool h = (res.height() != mRes.height());
    mRes = res;

    if (w || h) {
        emit resChanged(mRes);
    }
    if (w) {
        emit widthChanged(mRes.width());
    }
    if (h) {
        emit heightChanged(mRes.height());
    }
    if (direction != mDisplayDirection) {
        mDisplayDirection = direction;
        emit displayDirectionChanged(mDisplayDirection);
    }
}
