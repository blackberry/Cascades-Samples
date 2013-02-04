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
#include "stockcurverecipe.h"

#include <bb/cascades/Color>
#include <bb/cascades/Container>
#include <bb/cascades/DockLayout>
#include <bb/cascades/FadeTransition>
#include <bb/cascades/ImageView>
#include <bb/cascades/Label>
#include <bb/cascades/RadioGroup>
#include <bb/cascades/ScrollView>
#include <bb/cascades/ScrollViewProperties>
#include <bb/cascades/SequentialAnimation>
#include <bb/cascades/StackLayout>
#include <bb/cascades/StackLayoutProperties>
#include <bb/cascades/StockCurve>
#include <bb/cascades/SystemDefaults>
#include <bb/cascades/TextStyle>
#include <bb/cascades/TranslateTransition>

using namespace bb::cascades;

StockCurveRecipe::StockCurveRecipe(Container *parent) :
        CustomControl(parent)
{
    // Get the application UI values instance for accessing styling data
    mUiValues = UiValues::instance();

    // The recipe Container where the UI components will be kept
    Container *recipeContainer = new Container();

    StackLayout *recipeLayout = new StackLayout();
    recipeLayout->setOrientation(LayoutOrientation::LeftToRight);
    recipeContainer->setLayout(recipeLayout);

    // A Container in which the animated object is shown
    Container *animationContainer = Container::create().top(mUiValues->intValue(UiValues::UI_PADDING_STANDARD));
    animationContainer->setLayoutProperties(StackLayoutProperties::create().spaceQuota(35.0));
    animationContainer->setVerticalAlignment(VerticalAlignment::Fill);

    // Create a label that will present which easing curve is being used.
    mStockCurveDescription = new Label(animationContainer);
    mStockCurveDescription->setHorizontalAlignment(HorizontalAlignment::Center);
    mStockCurveDescription->setText("Linear");

    mStockCurveDescription->textStyle()->setBase(SystemDefaults::TextStyles::smallText());
    mStockCurveDescription->textStyle()->setTextAlign(TextAlign::Center);

    // Set up an image with animations.
    createAnimatedEgg(animationContainer);

    // The selection of easing curve is done in a panel to the right. The
    // panel has three radio groups one for each ease type ("out"/"in"/"inout").
    ScrollView *selectionView = setUpStockCurveSelectionPanel();

    // Add the Containers to the recipe Container.
    recipeContainer->add(animationContainer);
    recipeContainer->add(selectionView);

    setRoot(recipeContainer);
}

void StockCurveRecipe::createAnimatedEgg(Container *parent)
{
    // The object that is going to be animated using different StockCurves as ease
    mEgg = new ImageView(parent);
    mEgg->setImageSource(QUrl("asset:///images/stockcurve/egg.png"));
    mEgg->setParent(parent);
    mEgg->setHorizontalAlignment(HorizontalAlignment::Center);
    mEgg->setProperty("breakEgg", QVariant(true));

    // To illustrate the effect of different StockCurves, we will translate the object
    // using all the different StockCurves available (new curves will be selected in a RadioButtonGroup).
    mEaseAnim = TranslateTransition::create(mEgg)
            .toY(mUiValues->intValue(UiValues::UI_STOCKCURVERECIPE_EGGDISTANCE))
            .duration(mUiValues->intValue(UiValues::UI_STOCKCURVERECIPE_EGGTIME))
            .easingCurve(StockCurve::Linear);

    // To reset the animated object before running the animation with the ease curve
    // we set up a sequential animation which hides the object at the end position,
    // moves it to the start point, shows it again, and finally runs the animation with
    // the ease curve that has been selected.
    mAnim =
            SequentialAnimation::create(mEgg).add(FadeTransition::create(mEgg).to(0)).add(
                    TranslateTransition::create(mEgg).toY(0)).add(
                    FadeTransition::create(mEgg).to(1.0f)).add(mEaseAnim).parent(this);

    // Connect to the animation signals, in order to change the image
    // on an onEnded signal to a broken egg if the animation stops abruptly.
    connect(mAnim, SIGNAL(started()), this, SLOT(resetEggImage()));
    connect(mAnim, SIGNAL(ended()), this, SLOT(breakEgg()));

    // Run the animation as the recipe is created.
    mAnim->play();
}

ScrollView *StockCurveRecipe::setUpStockCurveSelectionPanel()
{
    Container *stockCurves = new Container();

    StackLayout *stockCurvesLayout = new StackLayout();
    stockCurvesLayout->setOrientation(LayoutOrientation::LeftToRight);

    stockCurves->setLayout(stockCurvesLayout);

    // Create RadioGroup for ease "out" animation curves.
    QList<StockCurve> curvesOut;
    curvesOut << StockCurve::Linear << StockCurve::SineOut << StockCurve::CubicOut
            << StockCurve::QuarticOut << StockCurve::QuinticOut << StockCurve::CircularOut
            << StockCurve::BackOut << StockCurve::ElasticOut << StockCurve::DoubleElasticOut
            << StockCurve::BounceOut << StockCurve::DoubleBounceOut;

    QList<QString> namesOut;
    namesOut << "Linear" << "SineOut" << "CubicOut" << "QuarticOut" << "QuinticOut" << "CircularOut"
            << "BackOut" << "ElasticOut" << "DoubleElasticOut" << "BounceOut" << "DoubleBounceOut";

    // Create a RadioGroup containing the curves specified in the lists above.
    stockCurves->add(
            setUpRadioGroup(&mRadioGroupOut, "Out", namesOut, curvesOut,
                    SLOT(outSelectedOptionChanged(int))));

    // Create RadioGroup for ease "in" animation curves.
    QList<StockCurve> curvesIn;
    curvesIn << StockCurve::Linear << StockCurve::SineIn << StockCurve::CubicIn
            << StockCurve::QuarticIn << StockCurve::QuinticIn << StockCurve::CircularIn
            << StockCurve::BackIn;

    QList<QString> namesIn;
    namesIn << "Linear" << "SineIn" << "CubicIn" << "QuarticIn" << "QuinticIn" << "CircularIn"
            << "BackIn";

    // Create a RadioGroup containing the curves specified in the lists above.
    stockCurves->add(
            setUpRadioGroup(&mRadioGroupIn, "In", namesIn, curvesIn,
                    SLOT(inSelectedOptionChanged(int))));

    // Create RadioGroup for ease "inout" animation curves.
    QList<StockCurve> curvesInOut;
    curvesInOut << StockCurve::Linear << StockCurve::SineInOut << StockCurve::CubicInOut
            << StockCurve::QuarticInOut << StockCurve::QuinticInOut << StockCurve::CircularInOut
            << StockCurve::BackInOut;

    QList<QString> namesInOut;
    namesInOut << "Linear" << "SineInOut" << "CubicInOut" << "QuarticInOut" << "QuinticInOut"
            << "CircularInOut" << "BackInOut";

    stockCurves->add(
            setUpRadioGroup(&mRadioGroupInOut, "InOut", namesInOut, curvesInOut,
                    SLOT(inOutSelectedOptionChanged(int))));

    // Set the initial index to be "Linear" (see the out list above).
    mRadioGroupOut->setSelectedIndex(0);

    // This Part of the UI does not fit on one screen, a ScrollView is
    // set up so that the user can scroll vertically the selections.
    ScrollView* scrollView = ScrollView::create().scrollMode(ScrollMode::Vertical);
    scrollView->setContent(stockCurves);
    scrollView->setLayoutProperties(StackLayoutProperties::create().spaceQuota(65));
    scrollView->setVerticalAlignment(VerticalAlignment::Fill);

    return scrollView;
}

Container *StockCurveRecipe::setUpRadioGroup(RadioGroup **radioGroup, QString const &title,
        QList<QString> names, QList<StockCurve> curves, const char *slot)
{
    Container *radioGroupContainer = Container::create().top(mUiValues->intValue(UiValues::UI_PADDING_STANDARD));

    Label* radioGroupLabel = Label::create(title);
    radioGroupLabel->textStyle()->setBase(SystemDefaults::TextStyles::smallText());
    radioGroupLabel->textStyle()->setTextAlign(TextAlign::Center);

    radioGroupLabel->setHorizontalAlignment(HorizontalAlignment::Fill);

    *radioGroup = new RadioGroup();

    for (int i = 0; i < names.size(); ++i) {
        // Create the option and set its image asset based on the name.
        Option* option = Option::create();
        option->setImageSource(QUrl("asset:///images/stockcurve/icon_" + names.at(i) + ".png"));

        // Store the name and the StockCurve as properties. They are stored as properties
        // so we can use them to trigger a new animation when an option is selected.
        option->setProperty("name", QVariant(names.at(i)));
        option->setProperty("curve", QVariant::fromValue<StockCurve>(curves.at(i)));
        (*radioGroup)->add(option);
    }

    connect(*radioGroup, SIGNAL(selectedIndexChanged(int)), this, slot);

    radioGroupContainer->add(radioGroupLabel);
    radioGroupContainer->add(*radioGroup);

    return radioGroupContainer;
}

void StockCurveRecipe::playAnimForOption(int selected, RadioGroup* radioGroup)
{
    QString description = radioGroup->at(selected)->property("name").toString();

    // Set the StockCurve name stored in the "name" property.
    mStockCurveDescription->setText(description);

    // If the Easing curve selected results in an abrupt stop,
    // we break the egg when the animation is over.
    if (description.indexOf("InOut") == -1
            && (description.indexOf("In") >= 0 || description.indexOf("Elastic") >= 0
                    || description.indexOf("Bounce") >= 0 || description.indexOf("Linear") >= 0)) {
        mEgg->setProperty("breakEgg", QVariant(true));
    } else {
        mEgg->setProperty("breakEgg", QVariant(false));
    }

    // Get the selected easing curve, update the animation, and play it
    // (if the animation is already running, we first stop it to avoid conflicts).
    mEaseAnim->setEasingCurve(radioGroup->at(selected)->property("curve").value<StockCurve>());
    mAnim->stop();
    mAnim->play();
}

void StockCurveRecipe::outSelectedOptionChanged(int selected)
{
    if (selected >= 0) {

        // Reset the other radio groups (make sure there is no selected option in the groups).
        mRadioGroupIn->resetSelectedIndex();
        mRadioGroupInOut->resetSelectedIndex();

        // Set up the new easingCurve and run the animation.
        playAnimForOption(selected, mRadioGroupOut);
    }
}

void StockCurveRecipe::inSelectedOptionChanged(int selected)
{
    if (selected >= 0) {

        // Reset the other radio groups (make sure there is no selected option in the groups).
        mRadioGroupOut->resetSelectedIndex();
        mRadioGroupInOut->resetSelectedIndex();

        // Set up the new easingCurve and run the animation.
        playAnimForOption(selected, mRadioGroupIn);
    }
}

void StockCurveRecipe::inOutSelectedOptionChanged(int selected)
{
    if (selected >= 0) {

        // Reset the other radio groups (make sure there is no selected option in the groups).
        mRadioGroupOut->resetSelectedIndex();
        mRadioGroupIn->resetSelectedIndex();

        // Set up the new easingCurve and run the animation.
        playAnimForOption(selected, mRadioGroupInOut);
    }
}

void StockCurveRecipe::resetEggImage()
{
    mEgg->setImageSource(QUrl("asset:///images/stockcurve/egg.png"));
}

void StockCurveRecipe::breakEgg()
{
    // If the animation is set to break the egg in the onEnded signal, show the broken egg.
    if (mEgg->property("breakEgg").toBool()) {
        mEgg->setImageSource(QUrl("asset:///images/stockcurve/broken_egg.png"));
    }
}
