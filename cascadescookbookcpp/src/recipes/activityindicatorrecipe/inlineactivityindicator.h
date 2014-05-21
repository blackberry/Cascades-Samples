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

#ifndef _INLINEACTIVITYINDICATOR_H_
#define _INLINEACTIVITYINDICATOR_H_

#include <bb/cascades/CustomControl>

namespace bb
{
    namespace cascades
    {
        class ActivityIndicator;
        class Container;
        class Label;
    }
}

// A namespace is used to make this control easier/safer to use in other contexts.
namespace customcs
{

    /* InlineActivityIndicator Description:
     *
     * An inline activity indicator Control should be used when only
     * part of the screen is updated and interaction is still possible.
     * The Activity indicator is placed at the bottom of the screen
     * above the ActionBar, so that it interfere as little as possible with
     * the actual content. The Control mimic the behavior of the
     * ActivityIndicator but adds some visual components to it.
     *
     */
    class InlineActivityIndicator: public bb::cascades::CustomControl
    {
        Q_OBJECT

        /**
         * Similar to the out of the box activity indicator in Cascades the running property
         * indicates if the indicator is running or not.
         *
         * Setting the property to @c true starts the animation while @c false stops it.
         *
         * Defaults to false.
         */
        Q_PROPERTY(bool running READ isRunning WRITE setRunning RESET resetRunning NOTIFY runningChanged FINAL)
        Q_PROPERTY(QString indicatorText READ indicatorText WRITE setIndicatorText RESET resetIndicatorText NOTIFY indicatorTextChanged FINAL)

    public:
        /**
         * Constructs an inline activity indicator with a parent, if no parent is set
         * ownership has to be handled by the owner, if a parent is given ownership
         * is transferred to the parent.
         */
        InlineActivityIndicator(bb::cascades::Container *parent = 0);

        /*
         * Start animating of the activity indicator.
         *
         * Calls the ActivityIndicator function (see ActivityIndicator::start())
         */
        Q_SLOT void start();

        /*
         * Stop animating the activity indicator.
         *
         * Calls the ActivityIndicator function (see ActivityIndicator::stop())
         */
        Q_SLOT void stop();

        /*
         * Determines if the activity indicator is currently running.
         *
         * Calls the ActivityIndicator function (see ActivityIndicator::isRunning())
         *
         * @return @c true if the activity indicator is running, False otherwise.
         */
        bool isRunning() const;

        /*
         * Sets the running state.
         *
         * Calls the ActivityIndicator function (see ActivityIndicator::setRunning())
         *
         * @param running The new state to set. @c true for running, and @c false to stop it.
         */
        Q_SLOT void setRunning(bool running);

        /*
         * Resets the running state to @c false, which stops the indicator.
         *
         * Calls the ActivityIndicator function (see ActivityIndicator::resetRunning())
         *
         */
        Q_SLOT void resetRunning();

        /**
         * Call this function to get the current description text set on the Control.
         *
         * @return The current description text.
         */
        QString indicatorText();

        /**
         * Sets the desciption text of the InlineActivityIndicator.
         *
         * @param indicatorText The new text to set as description
         */
        Q_SLOT void setIndicatorText(QString indicatorText);

        /*
         * Resets the current description to an empty string.
         */
        Q_SLOT void resetIndicatorText();

    Q_SIGNALS:
        /*
         * @brief Emitted when the activity indicator is started.
         *
         * (see ActivityIndicator::started())
         *
         */
        void started();

        /*
         * Emitted when the activity indicator is stopped.
         *
         * (see ActivityIndicator::stopped())             *
         */
        void stopped();

        /*
         * Emitted when the activity indicator is stopping.
         *
         * (see ActivityIndicator::stopping())             *
         */
        void stopping();

        /*
         * Emitted when the running state has been changed.
         *
         * (see ActivityIndicator::runningChanged())             *
         *
         * @param running The newly set running state
         *
         */
        void runningChanged(bool running);

        /**
         * Emitted when the text description has been changed-
         *
         * @param indicatorText the new text on the Control.
         */
        void indicatorTextChanged(QString indicatorText);
    private:
        Q_SLOT void onRunningChanged(bool running);

        bb::cascades::ActivityIndicator *mActivityIndicator;
        bb::cascades::Label *mIndicatorLabel;
        QString mIndicatorText;

    };
}

#endif // ifndef _INLINEACTIVITYINDICATOR_H_
