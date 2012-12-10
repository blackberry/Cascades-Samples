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

#include "CircularSlider.hpp"

#include <bb/cascades/AbsoluteLayout>
#include <bb/cascades/Container>
#include <bb/cascades/DockLayout>
#include <bb/cascades/ImageView>
#include <bb/cascades/TouchEvent>

#include "math.h"

using namespace bb::cascades;

CircularSlider::CircularSlider(Container *parent)
    : CustomControl(parent)
    , m_revAngle(0)
    , m_angle(0)
    , m_value(0)
{
    // Create a root container with an AbsoluteLayout.
    m_rootContainer = new Container();
    m_rootContainer->setLayout(new AbsoluteLayout());

    // Create the slider track image.
    m_trackImage = ImageView::create().image(QUrl("asset:///images/slider_track.png"));

    // Create the handle container and two images, one for
    // active state and one for inactive.
    m_handleContainer = Container::create().layout(new DockLayout());

    // Disable implicit animations for the handle container so that
    // the handle doesn't jump when it's being dragged.
    m_handleImplicitAnimationController = ImplicitAnimationController::create(m_handleContainer).enabled(false);

    // Load the handle images
    m_handleOn = Image(QUrl("asset:///images/handle_pressed.png"));
    m_handleOff = Image(QUrl("asset:///images/handle_inactive.png"));

    // Create the image view for the handle using the image for
    // the inactive handle
    m_handle = ImageView::create().image(m_handleOff)
                                  .horizontal(HorizontalAlignment::Right)
                                  .vertical(VerticalAlignment::Center);

    // Add the handle image to the to handle container
    // and add everything to the root container
    m_handleContainer->add(m_handle);
    m_rootContainer->add(m_trackImage);
    m_rootContainer->add(m_handleContainer);

    // Set the root of the custom control.
    setRoot(m_rootContainer);

    // Connect the signals of CustomControl to your custom slots to react to size changes
    connect(this, SIGNAL(preferredHeightChanged(float)), this, SLOT(onHeightChanged(float)));
    connect(this, SIGNAL(preferredWidthChanged(float)), this, SLOT(onWidthChanged(float)));

    // Set the initial size.
    m_width = 600;
    m_height = 600;
    setPreferredSize(m_width, m_height);

    // Connect to the signal of Container to handle touch events
    connect(m_rootContainer, SIGNAL(touch(bb::cascades::TouchEvent*)),
            this, SLOT(onSliderHandleTouched(bb::cascades::TouchEvent*)));
}

// Set the new width of the custom control and initiate the resizing
void CircularSlider::onWidthChanged(float width)
{
    m_width = width;
    onSizeChanged();
}

// Set the new height of the custom control and initiate the resizing
void CircularSlider::onHeightChanged(float height)
{
    m_height = height;
    onSizeChanged();
}

void CircularSlider::onSizeChanged()
{
    // Define the center of the circle.
    m_centerX = m_width / 2;
    m_centerY = m_height / 2;
    m_radiusCircle = m_width - m_centerX;

    // Set the root container to the new size.
    m_rootContainer->setPreferredSize(m_width, m_height);

    // Set the track image to be slightly smaller than the root.
    m_trackImage->setPreferredSize(m_width * 0.85, m_height * 0.85);

    // Set the handle image and container to be much smaller.
    m_handle->setPreferredSize(0.2 * m_width, 0.2 * m_height);
    m_handleContainer->setPreferredSize(m_width, 0.2 * m_height);

    // Transform the handle container along its y axis to move it
    // into the correct position.
    m_handleContainer->setTranslationY((m_height - 0.2 * m_height) / 2);

    // Transform the position of the track image to the correct
    // position.
    m_trackImage->setTranslation((m_width - 0.85 * m_width) / 2,
                                 (m_height - 0.85 * m_height) / 2);

    // Clear the circumference points for the circle
    // and reinitialize them to reflect the new size.
    m_pointsOnCircumference.clear();

    for (int angle = 0; angle < 360; angle++) {
        const float x = m_centerX + (m_radiusCircle) * cos(angle * M_PI / 180);
        const float y = m_centerY + (m_radiusCircle) * sin(angle * M_PI / 180);
        m_pointsOnCircumference.append(qMakePair(x, y));
    }
}

void CircularSlider::processRawCoordinates(float touchX, float touchY)
{
    // Determine the distance from the center to the touch point.
    const float distanceFromCenterToTouchPoint = sqrt(
                        (touchX - m_centerX) * (touchX - m_centerX) +
                        (touchY - m_centerY) * (touchY - m_centerY));

    // Determine whether the touch point is outside the center of
    // the circle and in the valid touch area.
    if (distanceFromCenterToTouchPoint >= (0.3 * m_radiusCircle) &&
        distanceFromCenterToTouchPoint <= m_radiusCircle) {
        // The minimum distance from the touch.
        float minDistanceFromTouch = INT_MAX;

        // Measure the distance from the touch to the circumference
        // for each point on the circle and store the X and Y
        // coordinates for the shortest distance.
        for (float i = 0; i < m_pointsOnCircumference.size(); i++) {
            const float x = m_pointsOnCircumference[i].first;
            const float y = m_pointsOnCircumference[i].second;

            const float distanceFromTouch = sqrt((x - touchX) * (x - touchX) +
                                                 (y - touchY) * (y - touchY));

            if (distanceFromTouch < minDistanceFromTouch) {
                minDistanceFromTouch = distanceFromTouch;
                // The angle to rotate the handle container once moved
                m_angle = i;
            }
        }

        // Rotate the handle container along its Z-axis.
        if (m_angle != m_revAngle) {
            m_handleContainer->setRotationZ(m_angle);

            // Our slider has a new value, and we want our QML to know
            m_value = m_angle;
            emit valueChanged(m_value);

            m_revAngle = m_angle;
        }
    }
}

// Get the value of the slider.
float CircularSlider::value() const
{
    return m_value;
}

// Set the value of the slider.
void CircularSlider::setValue(float value)
{
    if (m_value == value)
        return;

    m_revAngle = m_angle = m_value = value;
    m_handleContainer->setRotationZ(m_angle);

    emit valueChanged(m_value);
}

void CircularSlider::onSliderHandleTouched(TouchEvent* touchEvent)
{
    // Change to the active handle image if isDown()
    if (touchEvent->isDown()) {
        m_handle->setImage(m_handleOn);
    // Change to the inactive handle image if isUp()
    } else if (touchEvent->isUp()) {
        m_handle->setImage(m_handleOff);
    // Change the position of the slider handle if isMove()
    } else if (touchEvent->isMove()) {
        processRawCoordinates(touchEvent->localX(), touchEvent->localY());
    }
}
