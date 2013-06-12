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
#include "InlineActivityIndicator.h"
#include <uivalues.h>

#include <bb/cascades/ActivityIndicator>
#include <bb/cascades/ColorTheme>
#include <bb/cascades/Container>
#include <bb/cascades/DockLayout>
#include <bb/cascades/Label>
#include <bb/cascades/StackLayout>
#include <bb/cascades/SystemDefaults>
#include <bb/cascades/TextStyle>
#include <bb/cascades/Theme>
#include <bb/cascades/ThemeSupport>

using namespace bb::cascades;

namespace customcs
{
    InlineActivityIndicator::InlineActivityIndicator(bb::cascades::Container *parent) :
            CustomControl(parent)
    {
        // Initialize the descriptive text set on the Control
        mIndicatorText = "";

        // Get the UiValues instance for handling different resolutions.
        UiValues *uiValues = UiValues::instance();

        // The content Container of the Control, which will contain all the
        // components building up the control.
        Container *content = new Container();
        content->setPreferredHeight(
                uiValues->intValue(UiValues::UI_INLINEACTIVITYINDICATOR_HEIGHT));
        content->setPreferredWidth(uiValues->intValue(UiValues::SCREEN_WIDTH));
        content->setLeftPadding(uiValues->intValue(UiValues::UI_INLINEACTIVITYINDICATOR_PADDING));
        content->setRightPadding(uiValues->intValue(UiValues::UI_INLINEACTIVITYINDICATOR_PADDING));
        content->setLayout(DockLayout::create());

        // Set the background color based on the current application theme
        // Note that in some cases this might give a color that is not suitable
        // for example if the context that this control is placed does not follow
        // the application theme, if so one would have to expose some way of setting
        // the individual colors. Here we assume that the indicator should follor the theme.
        ThemeSupport *themeSupport = Application::instance()->themeSupport();
        Theme *theme = themeSupport->theme();
        ColorTheme *colorTheme = theme->colorTheme();
        if (colorTheme->style() == VisualStyle::Bright) {
            content->setBackground(Color::fromARGB(0xe6f8f8f8));
        } else {
            content->setBackground(Color::fromARGB(0xff000000));
        }

        // The standard ActitviyIndicator, it will be placed to the right.
        mActivityIndicator = new ActivityIndicator();
        mActivityIndicator->setVerticalAlignment(VerticalAlignment::Center);
        mActivityIndicator->setHorizontalAlignment(HorizontalAlignment::Right);

        // Connect the ActivityIndicator signals to the signals of this class.
        connect(mActivityIndicator, SIGNAL(started()), SIGNAL(started()));
        connect(mActivityIndicator, SIGNAL(stopped()), SIGNAL(stopped()));
        connect(mActivityIndicator, SIGNAL(stopping()), SIGNAL(stopping()));

        // In is running we hide and show the entire Custom control so its connected to slot.
        connect(mActivityIndicator, SIGNAL(runningChanged(bool)), SLOT(onRunningChanged(bool)));

        // A label used to display a text for the user so that it knows what is going on.
        mIndicatorLabel = new Label();
        mIndicatorLabel->setText(mIndicatorText);
        mIndicatorLabel->textStyle()->setBase(SystemDefaults::TextStyles::subtitleText());
        mIndicatorLabel->setVerticalAlignment(VerticalAlignment::Center);
        mIndicatorLabel->setHorizontalAlignment(HorizontalAlignment::Left);

        // Add the controls to the content Container and set it as root.
        content->add(mIndicatorLabel);
        content->add(mActivityIndicator);

        // The indicator starts in stopped mode (hidden).
        setVisible(false);

        // Set the main content Container as the root Control of the CustomControl.
        setRoot(content);
    }

    void InlineActivityIndicator::start()
    {
        // Just mimic the behavior of the standard ActivityIndicator
        mActivityIndicator->start();
    }

    void InlineActivityIndicator::stop()
    {
        // Just mimic the behavior of the standard ActivityIndicator
        mActivityIndicator->stop();
    }

    bool InlineActivityIndicator::isRunning() const
    {
        // Just mimic the behavior of the standard ActivityIndicator
        return mActivityIndicator->isRunning();
    }

    void InlineActivityIndicator::setRunning(bool running)
    {
        // Just mimic the behavior of the standard ActivityIndicator
        mActivityIndicator->setRunning(running);
    }

    void InlineActivityIndicator::resetRunning()
    {
        // Just mimic the behavior of the standard ActivityIndicator
        mActivityIndicator->resetRunning();
    }

    QString InlineActivityIndicator::indicatorText()
    {
        return mIndicatorText;
    }

    void InlineActivityIndicator::setIndicatorText(QString indicatorText)
    {
        // Update the indicator text and emit the changed signal if it has changed.
        if (mIndicatorText != indicatorText) {
            mIndicatorText = indicatorText;
            emit indicatorTextChanged(mIndicatorText);
            mIndicatorLabel->setText(mIndicatorText);
        }
    }

    void InlineActivityIndicator::resetIndicatorText()
    {
        // Reset the indicator text only if it is not already the default empty string.
        if (mIndicatorText != "") {
            mIndicatorText = "";
            emit indicatorTextChanged(mIndicatorText);
            mIndicatorLabel->setText(mIndicatorText);
        }
    }

    void InlineActivityIndicator::onRunningChanged(bool running)
    {
        // Hide and show the Control when running is turned off and on.
        if (running) {
            setVisible(true);
        } else {
            setVisible(false);
        }
        emit runningChanged(running);
    }

}
