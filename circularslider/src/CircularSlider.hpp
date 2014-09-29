/* Copyright (c) 2012, 2013  BlackBerry Limited.
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

#ifndef CIRCULARSLIDER_HPP
#define CIRCULARSLIDER_HPP

#include <bb/cascades/CustomControl>
#include <bb/cascades/Image>
#include <bb/cascades/ImplicitAnimationController>
#include <bb/cascades/TrackpadHandler>

#include <QPair>
#include <QVector>

namespace bb {
namespace cascades {
class Container;
class ImageView;
class TouchEvent;
}
}

class CircularSlider: public bb::cascades::CustomControl
{
    Q_OBJECT

    Q_PROPERTY(float value READ value WRITE setValue NOTIFY valueChanged FINAL)

public:
    CircularSlider(bb::cascades::Container *parent = 0);

    float value() const;
    void setValue(float value);

Q_SIGNALS:
    void valueChanged(float value);

private Q_SLOTS:
    void onSliderHandleTouched(bb::cascades::TouchEvent *touchEvent);
    void onWidthChanged(float width);
    void onHeightChanged(float height);
    void onTrackpad(bb::cascades::TrackpadEvent *trackpadEvent);
    void onDevShortcutTriggered(void);

private:
    void onSizeChanged();
    void processRawCoordinates(float inX, float inY);

    bb::cascades::Container *m_rootContainer;
    bb::cascades::ImageView *m_trackImage;
    bb::cascades::TrackpadHandler *m_trackpad;

    float m_width;
    float m_height;
    float m_revAngle;

    float m_centerX;
    float m_centerY;
    float m_radiusCircle;

    bb::cascades::Image m_handleOn;
    bb::cascades::Image m_handleOff;
    bb::cascades::ImageView *m_handle;
    bb::cascades::Container *m_handleContainer;
    bb::cascades::ImplicitAnimationController m_handleImplicitAnimationController;

    QVector<QPair<float, float> > m_pointsOnCircumference;
    float m_angle;
    float m_value;
};

#endif
