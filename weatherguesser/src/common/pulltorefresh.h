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
#ifndef PULLTOREFRESH_H_
#define PULLTOREFRESH_H_
#include <bb/cascades/CustomControl>

namespace bb
{
    namespace cascades
    {
        class ActivityIndicator;
        class Label;
        class ListView;
        class ImageView;
    }
}

/**
 * PullToRefresh Description.
 *
 * A Custom Control meant to be used to monitor user activity at the
 * edge of a scrollable control. If added as a leading visual to a list
 * it will monitor how much of the control is visible on a screen and
 * if the user decides to show it in its entirety a signal will be
 * emitted once the list is released. This will indicate that the user
 * would like to add more data at the top of the list.
 */
class PullToRefresh: public bb::cascades::CustomControl
{
    Q_OBJECT

    /**
     * The scrollable Control, in our current example its a ListView and the
     * pull to refresh control is added a leading visual to the list.
     */
    Q_PROPERTY(bb::cascades::Control* control READ control WRITE setControl NOTIFY controlChanged FINAL)

    /**
     * Property that tells which state of pulling the control is in. Once the
     * user has pulled far enough and released the control enters a loading state and
     * this property will be true.
     */
    Q_PROPERTY(bool refreshActive READ refreshActive NOTIFY refreshActiveChanged FINAL)

public:
    /**
     * Constructor of the CustomControl.
     *
     * @param parent The parent object of the Control.
     */
    PullToRefresh(bb::cascades::Container* parent = 0);
    virtual ~PullToRefresh();

    /**
     * This function should be called when the pull to refresh control should
     * be hidden, for example once the update that was triggered has finished
     * and a new data is being added to the list.
     */
    Q_INVOKABLE void refreshDone();

    /**
     * Returns the current state of refresh.
     *
     * @return True of the Control is in the loading state otherwise false.
     */
    Q_INVOKABLE bool refreshActive();

    /**
     * The Control that the pull to refresh control is attached to.
     */
    bb::cascades::Control* control();

    /**
     * Set a specific scrollable control that this Control should
     * trigger updates on. In most cases this will be a ListView but
     * its possible to use it in other circumstances as well.
     *
     * @param Control The scrollable Control to monitor.
     */
    void setControl(bb::cascades::Control* control);

signals:
    /**
     * Signal emitted when the refresh state changes for the Control.
     *
     * @param active True if the Control has entered the lading state.
     */
    void refreshActiveChanged(bool active);

    /**
     * Signal emitted when the Control that the pull to refresh Control
     * is connected to changes.
     *
     * @param control The Control that this control is monitoring.
     */
    void controlChanged(bb::cascades::Control *control);

private slots:
    /**
     * This function is connected to the layoutFrameChanged signal
     * of the controls LayoutUpdateHandler. It will tell us when the
     * Controls position changes in the UI. This used to decide if the
     * user has pulled far enough to trigger the refresh signal.
     *
     * @param A rectangle representing the size and position of the Control.
     */
    void onLayoutFrameChanged(const QRectF &layoutFrame);

    /**
     * A touch slot function used to listen for when user
     * touch interaction with the control which this CustomControl
     * is attached to. Appropriate action is taken in this function based
     * upon the current state of the pulling.
     *
     * @param event A touch event.
     */
    void onListViewTouch(bb::cascades::TouchEvent *event);

    /**
     * Function that resets all states to the default values.
     */
    void resetStates();

    /**
     * Function connected to the ListViews scrollingChanged signal.
     * This is used to find the moment when this control has gone off screen
     * after the refreshDone() function has been called.
     *
     * @param scrolling Boolean that is true if the list starts to scroll otherwise false.
     */
    void handleScrollingChanged(bool scrolling);

private:

    // UI components.
    bb::cascades::Label *mLabel;
    bb::cascades::ImageView *mImage;
    bb::cascades::ActivityIndicator *mActivityIndicator;
    bb::cascades::Control *mControl;
    bb::cascades::Container *mRootContainer;
    bb::cascades::ListView *mListView;

    // State variables.
    bool mRefreshActive;
    bool mToBeRefreshed;
};

#endif /* PULLTOREFRESH_H_ */
