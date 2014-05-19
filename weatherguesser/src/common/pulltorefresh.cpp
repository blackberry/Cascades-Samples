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
#include "pulltorefresh.h"
#include <bb/cascades/ActivityIndicator>
#include <bb/cascades/Color>
#include <bb/cascades/Container>
#include <bb/cascades/DockLayout>
#include <bb/cascades/FadeTransition>
#include <bb/cascades/ImageView>
#include <bb/cascades/Label>
#include <bb/cascades/LayoutUpdateHandler>
#include <bb/cascades/ListView>
#include <bb/cascades/ListScrollStateHandler>
#include <bb/cascades/StackLayout>
#include <bb/cascades/StockCurve>
#include <bb/cascades/SystemDefaults>
#include <bb/cascades/StackLayoutProperties>
#include <bb/cascades/TouchType>

using namespace bb::cascades;

PullToRefresh::PullToRefresh(bb::cascades::Container* parent) :
        CustomControl(parent), mControl(0), mListView(0), mRefreshActive(false), mToBeRefreshed(false)
{
    // Adding the visual components to the custom control, an activity indicator
    // shown when the control is in its refreshActive state, a label with a status text,
    // and an arrow image used to reflect if further pulling is needed in order to trigger the refresh.
    mActivityIndicator = ActivityIndicator::create().vertical(
            VerticalAlignment::Center).horizontal(HorizontalAlignment::Center);

    mImage = ImageView::create("asset:///images/pulltorefresh/arrow.png").vertical(
            VerticalAlignment::Center).horizontal(HorizontalAlignment::Center);

    Container * innerContainer = Container::create().layout(DockLayout::create())
            .horizontal(HorizontalAlignment::Center).add(mImage).add(mActivityIndicator);

    mLabel = Label::create(tr("Pull."));
    mLabel->setMinWidth(150); // A minimum width is set to avoid repositioning of the label.

    Container *instContainer = Container::create().layout(StackLayout::create()
                .orientation(LayoutOrientation::LeftToRight)).vertical(VerticalAlignment::Center)
                .horizontal(HorizontalAlignment::Center).add(innerContainer).add(mLabel);

    Container *contentContainer = Container::create().layout(DockLayout::create()).
            add(instContainer).vertical(VerticalAlignment::Fill).layoutProperties(StackLayoutProperties::create().spaceQuota(1));

    // The root Container is set to be a horizontal StackLayout in order to use space quota to fill the screen.
    mRootContainer = Container::create().layout(StackLayout::create()
                        .orientation(LayoutOrientation::LeftToRight)).add(contentContainer);

    setRoot(mRootContainer);

    // In order to track how much of the Control is visible on screen we
    // create a layout update handler and connect to the signal for layout
    // frame changes.
    LayoutUpdateHandler *layoutUpdateHandler = LayoutUpdateHandler::create(this);
    bool connectResult = connect(layoutUpdateHandler, SIGNAL(layoutFrameChanged(const QRectF &)),
            SLOT(onLayoutFrameChanged(const QRectF &)));
    Q_ASSERT(connectResult);
    Q_UNUSED(connectResult);
}

PullToRefresh::~PullToRefresh()
{
}

void PullToRefresh::onLayoutFrameChanged(const QRectF& layoutFrame)
{
    // If the Activity indicator is not running it means that the user
    // hasn't yet triggered a refresh (not pulled down and released).
    if (!mActivityIndicator->isRunning()) {
        // Make sure the content is showing.
        mActivityIndicator->setOpacity(1.0);
        mLabel->setOpacity(1.0);
        mImage->setOpacity(1.0);

        if (layoutFrame.y() > 0) {
            // A y position larger then 0 indicates that the entire Control
            // is on screen, that is the user has pulled far enough so that
            // if released the refresh signal should be emitted.
            mImage->setRotationZ(180);
            mLabel->setText(tr("Release."));
            mToBeRefreshed = true;

        } else if (layoutFrame.y() < 0) {

            // While the position of the Control is less then zero part
            // of the Control is off the screen and the user can still decide
            // to not trigger a refresh of the list.
            mImage->setRotationZ(0);
            mLabel->setText(tr("Pull."));
            mToBeRefreshed = false;
        }
    }
}

void PullToRefresh::handleScrollingChanged(bool scrolling)
{
    // If the activity indicator opacity has been set to 0 and the list has stopped
    // scrolling this mean that leading visual is outside the screen and its
    // ok to refresh listitems.
    if( mActivityIndicator->opacity() == 0 && !scrolling) {
        resetStates();
    }
}

void PullToRefresh::onListViewTouch(bb::cascades::TouchEvent* event)
{
    if (event->touchType() == TouchType::Up && mToBeRefreshed) {

        // If the scrollable control that this custom control monitors is released,
        // while mToBeRefreshed is true (the entire Control is showing) update the UI
        // to show a loading state and emit a signal that its time to refresh (get more data).
        mActivityIndicator->setRunning(true);
        mImage->setOpacity(0);
        mLabel->setText(tr("Updating..."));
        mRefreshActive = true;
        mToBeRefreshed = false;
        emit refreshActiveChanged(mRefreshActive);
    }
}

bb::cascades::Control* PullToRefresh::control()
{
    return mControl;
}

void PullToRefresh::setControl(bb::cascades::Control *control)
{
    // A control has been set connect to the touch signal in order
    // to see when the user release the control.
    mControl = control;
    bool connectResult = connect(mControl, SIGNAL(touch(bb::cascades::TouchEvent*)),
            SLOT(onListViewTouch(bb::cascades::TouchEvent*)));
    Q_ASSERT(connectResult);
    Q_UNUSED(connectResult);

    // If the Control is a list start listening to scroll changes.
    mListView = qobject_cast<ListView *>(mControl);
    if(mListView) {
        ListScrollStateHandler::create(mListView)
            .onScrollingChanged(this, SLOT(handleScrollingChanged(bool)));
    }

    emit controlChanged(mControl);
}

void PullToRefresh::refreshDone()
{
    if (mListView) {
        // Scroll to the top of the list and thereby hiding the leading visual,
        // the reset signal is emitted once the scrolling is done in order to trigger
        // reload on a list that is not moving. There are other use cases where after
        // the refresh items are added on top of the current position. In that case
        // hide the visuals by using for example a FadeTransition and connect the resetstates
        // function to the animation onEnded signal.
        mListView->scrollToPosition(ScrollPosition::Beginning, ScrollAnimation::Smooth);
        mActivityIndicator->setOpacity(0);
        mLabel->setOpacity(0);
    }
}

void PullToRefresh::resetStates()
{
    // Reset all the states of the control.
    mActivityIndicator->setRunning(false);
    mRefreshActive = false;
    mToBeRefreshed = false;

    emit refreshActiveChanged(mRefreshActive);
}

bool PullToRefresh::refreshActive()
{
    return mRefreshActive;
}

