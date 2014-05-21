/* Copyright (c) 2012, 2013, 2014 BlackBerry Limited.
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
#include "speedgauge.h"

#include <bb/cascades/Container>
#include <bb/cascades/ImageView>
#include <bb/cascades/LayoutUpdateHandler>
#include <bb/cascades/DockLayout>
#include <QDateTime>

#define WORD_LENGTH 6.0f
#define ZERO_DIAL_ANGLE -120.0f

using namespace bb::cascades;

SpeedGauge::SpeedGauge(Container *parent) :
        CustomControl(parent), mMaxSpeedAngle(ZERO_DIAL_ANGLE), mAverageSpeed(0)
{
    bool connectResult;
    Q_UNUSED(connectResult);

    Container *content = new Container();

    // The content Container will be set to lay out children using a dock layout (to center everything on screen).
    content->setLayout(new DockLayout());

    // Background image in an ImageView
    ImageView *bgImage = ImageView::create("asset:///images/dial_bg.png");

    // Shine Image in an ImageView
    ImageView *dialShineImage = ImageView::create("asset:///images/dial_shine.png");
    dialShineImage->setHorizontalAlignment(HorizontalAlignment::Center);
    dialShineImage->setVerticalAlignment(VerticalAlignment::Center);

    // Create the needles; both needles are centered in the dial and should rotate around the base.
    mSpeedNeedle = setUpNeedle("asset:///images/needle.png");
    mMaxNeedle = setUpNeedle("asset:///images/red_needle.png");

    // Add the controls to the Container.
    content->add(bgImage);
    content->add(mMaxNeedle);
    content->add(mSpeedNeedle);
    content->add(dialShineImage);

    // Create a timer that will reduce the speed when the user is not typing.
    mSpeedUpdateTimer = new QTimer(this);
    mSpeedUpdateTimer->setInterval(200);
    connectResult = connect(mSpeedUpdateTimer, SIGNAL(timeout()), this, SLOT(calculateSpeedTimeOut()));
    Q_ASSERT(connectResult);

    setRoot(content);
}

ImageView *SpeedGauge::setUpNeedle(const QString pImageUrl)
{
    // Create the needle ImageView, set size and align it to the center of the dial.
    ImageView *needleImage = ImageView::create(pImageUrl);
    needleImage->setHorizontalAlignment(HorizontalAlignment::Center);
    needleImage->setVerticalAlignment(VerticalAlignment::Center);

    // We need the size of the image in order to set the pivot point for
    // rotation correctly, a LayoutUpdateHandler will report the image size, so
    // we can set the pivot points to the base of the image.
    LayoutUpdateHandler::create(needleImage).onLayoutFrameChanged(this,
            SLOT(needleLayoutFrameUpdated(QRectF)));

    return needleImage;
}

long SpeedGauge::getTimeMs()
{
    // Return current system time (used to calculate the speed).
    QDateTime time = QDateTime::currentDateTime();
    return time.toMSecsSinceEpoch();
}

void SpeedGauge::calculateSpeed(int nbrOfChars)
{
    long currentTime = getTimeMs();

    if (mStartTime == 0) {
        // If the start time has not been set, initialize it.
        mStartTime = getTimeMs();
    }

    // Calculate the number of words by dividing number of characters by an average word length.
    float nbrOfWords = (float) (nbrOfChars / WORD_LENGTH);
    mNbrOfChars = nbrOfChars;

    // We do not start displaying the speed until one full word has been entered.
    if (nbrOfWords >= 1.0) {

        // Time since start in minutes.
        float time = (currentTime - mStartTime) / (1000.0f * 60.0f);

        // Speed in words / minute.
        float speed = nbrOfWords / time;

        // The speed is translated into an angle (0 speed at -120 degrees and 60 at 120).
        float speedAngle = speed * (-ZERO_DIAL_ANGLE) * 2 / 60.0f + ZERO_DIAL_ANGLE;

        // Rotate the speed needle (using implicit animation).
        mSpeedNeedle->setRotationZ(speedAngle);

        // Calculate the average speed.
        qreal average = (60.0f * (mSpeedNeedle->rotationZ() - ZERO_DIAL_ANGLE)
                / (-ZERO_DIAL_ANGLE * 2.0f));
        mAverageSpeed = qRound(average);

        emit averageSpeedChanged(mAverageSpeed);


        if (speedAngle >= mMaxSpeedAngle) {
            // If the speed is the highest so far, update the rotation angle of the max needle.
            mMaxSpeedAngle = speedAngle;
            mMaxNeedle->setRotationZ(speedAngle);
        }

        if (!mSpeedUpdateTimer->isActive()) {
            // A timer is started, it will make consecutive calls to this function
            // if the user is not entering any text or is entering the wrong text.
            mSpeedUpdateTimer->start();
        }
    }
}

int SpeedGauge::averageSpeed()
{
    return mAverageSpeed;
}

void SpeedGauge::stop()
{
    mSpeedUpdateTimer->stop();
}

void SpeedGauge::calculateSpeedTimeOut()
{
    // Call the speed calculation with the current number of characters entered.
    calculateSpeed(mNbrOfChars);
}

void SpeedGauge::needleLayoutFrameUpdated(QRectF layoutRect)
{
    LayoutUpdateHandler *layoutHandler = qobject_cast<LayoutUpdateHandler*>(sender());
    ImageView *needleImage = qobject_cast<ImageView*>(layoutHandler->parent());

    if(needleImage) {
        // The centerPoint variable is the point where we want the rotation to be centered
        // on the Y-axis (height/vertical). Normally, this is in the exact middle (width/2 and height/2)
        // but since we are simulated an analog dial, we want it slightly above bottom of the image.
        float centerPoint = layoutRect.height() / 2 - layoutRect.width() / 2;

        needleImage->setPivotY(centerPoint);
        needleImage->setTranslationY(-centerPoint);

        needleImage->setRotationZ(ZERO_DIAL_ANGLE);
    }
}
