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
#include "lightningcrossfadeapp.h"

#include <bb/cascades/Color>
#include <bb/cascades/Container>
#include <bb/cascades/DockLayout>
#include <bb/cascades/DockLayoutProperties>
#include <bb/cascades/ImageView>
#include <bb/cascades/Page>
#include <bb/cascades/Slider>
#include <bb/cascades/StackLayout>
#include <bb/cascades/StackLayoutProperties>

using namespace bb::cascades;

LightningCrossfadeApp::LightningCrossfadeApp()
{
    // The UI is set up using a couple of different Containers set to handle
    // layouts of their children in different ways (stack and dock layouts).
    // First off we have the main content Container, where all other components will be added.
    Container *contentContainer = new Container();
    contentContainer->setLayout(StackLayout::create().top(20.0f));
    contentContainer->setBackground(Color::fromARGB(0xfff8f8f8));

    // Inside the content Container we have two component Containers, the first one contain images.
    Container *imageContainer = setUpImageContainer();
    imageContainer->setLayoutProperties(
            StackLayoutProperties::create().horizontal(HorizontalAlignment::Center));

    // Then set up the Slider Container and align it to the center of the content Container as well.
    Container *sliderContainer = setUpSliderContainer(imageContainer);
    sliderContainer->setLayoutProperties(
            StackLayoutProperties::create().horizontal(HorizontalAlignment::Center));

    // Add the two containers to the content container.
    contentContainer->add(imageContainer);
    contentContainer->add(sliderContainer);

    // We create a page with the control as content.
    Page *page = new Page();
    page->setContent(contentContainer);

    // Create the application scene and we are done.
    Application::setScene(page);
}

Container *LightningCrossfadeApp::setUpImageContainer()
{
    // In this function a Container containing two images are set up.
    // A dock layout is used to align the images.
    Container* nightAndDayContainer = new Container();
    nightAndDayContainer->setLayout(new DockLayout());

    // This the big image that was taking during the night
    // it's at the same position as the day one, but further from the viewer.
    ImageView* night = ImageView::create("asset:///images/night.jpg");

    // Center it using dock layout info.
    night->setLayoutProperties(DockLayoutProperties::create()
                                    .horizontal(HorizontalAlignment::Center)
                                    .vertical(VerticalAlignment::Center));


    // Since this image is on top of the night one, we can show the
    // night image with changing the opacity value of this image.
    ImageView* day = ImageView::create("asset:///images/day.jpg").opacity(0);

    // Center it using dock layout info.
    day->setLayoutProperties(DockLayoutProperties::create()
                                .horizontal(HorizontalAlignment::Center)
                                .vertical(VerticalAlignment::Center));

    // The day image is later attached to the slider to be able to find the
    // object an object name is assigned.
    day->setObjectName("dayImage");


    // Now add all the images to the Container and the alignment will be
    // automatically done by the layout system.
    nightAndDayContainer->add(night);
    nightAndDayContainer->add(day);

    return nightAndDayContainer;
}

Container *LightningCrossfadeApp::setUpSliderContainer(Container *imageContainer)
{
    // The Slider Container layouting set's the controls to sort themselves from
    // left to right rather then top to bottom (which is the default for a StackLayout).
    Container* sliderContainer = new Container();
    sliderContainer->setLayout(StackLayout::create()
                                    .direction(LayoutDirection::LeftToRight)
                                    .left(20.0f).right(20.0f).top(25.0f).bottom(25.0f));

    // This is the Slider you see at the bottom of the page when it get's a onValueChanging
    // event it changes the image with id night's opacity to that value, margins for space.
    Slider* opacitySlider = Slider::create()
                                .leftMargin(20.0f).rightMargin(20.0f);

    // Center the slider in the stack layout, a positive space quota makes the slider count less
    // than the moon and sun icon images when layouting on screens of different widths.
    opacitySlider->setLayoutProperties(StackLayoutProperties::create()
                                            .horizontal(HorizontalAlignment::Fill)
                                            .spaceQuota(1.0f));

    // A moon and sun icon image is used to illustrate that time of day, both
    // aligned to center in vertical direction to line up with the slider.
    ImageView* moon = ImageView::create("asset:///images/moon.png");
    moon->setLayoutProperties(StackLayoutProperties::create().vertical(VerticalAlignment::Center));
    ImageView* sun = ImageView::create("asset:///images/sun.png");
    sun->setLayoutProperties(StackLayoutProperties::create().vertical(VerticalAlignment::Center));

    // The night image will be attached to a slider so we need to get the corresponding
    // object from the image Container.
    ImageView *nightImage = imageContainer->findChild<ImageView*>("dayImage");

    // Connect the Slider value directly to the opacity property of the day image view.
    connect(opacitySlider, SIGNAL(valueChanging(float)), nightImage, SLOT(setOpacity(float)));

    // Then add the components (remember that they will be stacked from left to right).
    sliderContainer->add(moon);
    sliderContainer->add(opacitySlider);
    sliderContainer->add(sun);

    return sliderContainer;
}
