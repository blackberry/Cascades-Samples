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
#include "stockcurverecipe.h"
#include "stockcurvelist.h"

#include <bb/cascades/ArrayDataModel>
#include <bb/cascades/Color>
#include <bb/cascades/Container>
#include <bb/cascades/DockLayout>
#include <bb/cascades/FadeTransition>
#include <bb/cascades/ImageView>
#include <bb/cascades/Label>
#include <bb/cascades/LayoutUpdateHandler>
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
    // Get the UIConfig object in order to use resolution independent sizes.
    UIConfig *ui = this->ui();

    // The recipe Container where the UI components will be kept
    Container *recipeContainer = new Container();

    StackLayout *recipeLayout = new StackLayout();
    recipeLayout->setOrientation(LayoutOrientation::LeftToRight);
    recipeContainer->setLayout(recipeLayout);

    // A Container in which the animated object is shown
    Container *animationContainer = Container::create()
            .top(ui->du(2));
    animationContainer->setLayoutProperties(StackLayoutProperties::create().spaceQuota(20.0));
    animationContainer->setVerticalAlignment(VerticalAlignment::Fill);

    // Create a label that will present which easing curve is being used.
    mStockCurveDescription = new Label(animationContainer);
    mStockCurveDescription->setHorizontalAlignment(HorizontalAlignment::Center);
    mStockCurveDescription->setText("Linear");

    mStockCurveDescription->textStyle()->setBase(SystemDefaults::TextStyles::smallText());
    mStockCurveDescription->textStyle()->setTextAlign(TextAlign::Center);

    // Set up an image with animations.
    createAnimatedEgg(animationContainer);

    // Add the Containers to the recipe Container.
    recipeContainer->add(animationContainer);

    // The selection of easing curve is done in a panel to the right. The
    // panel has three lists one for each ease type ("out"/"in"/"inout").
    setUpStockCurveSelectionPanel(recipeContainer);

    setRoot(recipeContainer);

    LayoutUpdateHandler::create(recipeContainer).onLayoutFrameChanged(this,
            SLOT(screenLayoutFrameUpdated(QRectF)));

}

void StockCurveRecipe::createAnimatedEgg(Container *parent)
{
    bool connectResult;
    Q_UNUSED(connectResult);

    // The object that is going to be animated using different StockCurves as ease
    mEgg = new ImageView(parent);
    mEgg->setImageSource(QUrl("asset:///images/stockcurve/egg.png"));
    mEgg->setParent(parent);
    mEgg->setHorizontalAlignment(HorizontalAlignment::Center);
    mEgg->setProperty("breakEgg", QVariant(true));

    // To illustrate the effect of different StockCurves, we will translate the object
    // using all the different StockCurves available (new curves will be selected in a RadioButtonGroup).
    mEaseAnim = TranslateTransition::create(mEgg)
            .toY(780)
            .duration(1500)
            .easingCurve(StockCurve::Linear);

    // To reset the animated object before running the animation with the ease curve
    // we set up a sequential animation which hides the object at the end position,
    // moves it to the start point, shows it again, and finally runs the animation with
    // the ease curve that has been selected.
    mAnim = SequentialAnimation::create(mEgg)
            .add(FadeTransition::create(mEgg).to(0))
            .add(TranslateTransition::create(mEgg).toY(0))
            .add(FadeTransition::create(mEgg).to(1.0f))
            .add(mEaseAnim).parent(this);

    // Connect to the animation signals, in order to change the image
    // on an onEnded signal to a broken egg if the animation stops abruptly.
    connectResult = connect(mAnim, SIGNAL(started()), this, SLOT(resetEggImage()));
    Q_ASSERT(connectResult);

    connectResult = connect(mAnim, SIGNAL(ended()), this, SLOT(breakEgg()));
    Q_ASSERT(connectResult);
}

void StockCurveRecipe::setUpStockCurveSelectionPanel(Container *parent)
{
    bool connectResult;
    Q_UNUSED(connectResult);

    ArrayDataModel *model1 = new ArrayDataModel();
    model1->append(mapForNameAndCurve("Linear", StockCurve::Linear));
    model1->append(mapForNameAndCurve("SineOut", StockCurve::SineOut));
    model1->append(mapForNameAndCurve("CubicOut", StockCurve::CubicOut));
    model1->append(mapForNameAndCurve("QuarticOut", StockCurve::QuarticOut));
    model1->append(mapForNameAndCurve("QuarticOut", StockCurve::QuarticOut));
    model1->append(mapForNameAndCurve("QuinticOut", StockCurve::QuinticOut));
    model1->append(mapForNameAndCurve("CircularOut", StockCurve::CircularOut));
    model1->append(mapForNameAndCurve("BackOut", StockCurve::BackOut));
    model1->append(mapForNameAndCurve("ElasticOut", StockCurve::ElasticOut));
    model1->append(mapForNameAndCurve("DoubleElasticOut", StockCurve::DoubleElasticOut));
    model1->append(mapForNameAndCurve("BounceOut", StockCurve::BounceOut));
    model1->append(mapForNameAndCurve("DoubleBounceOut", StockCurve::DoubleBounceOut));

    StockcurveList *list1 = new StockcurveList();
    list1->setTitle("Out");
    list1->setDataModel(model1);

    ArrayDataModel *model2 = new ArrayDataModel();
    model2->append(mapForNameAndCurve("Linear", StockCurve::Linear));
    model2->append(mapForNameAndCurve("SineIn", StockCurve::SineIn));
    model2->append(mapForNameAndCurve("CubicIn", StockCurve::CubicIn));
    model2->append(mapForNameAndCurve("QuarticIn", StockCurve::QuarticIn));
    model2->append(mapForNameAndCurve("QuarticIn", StockCurve::QuarticIn));
    model2->append(mapForNameAndCurve("QuinticIn", StockCurve::QuinticIn));
    model2->append(mapForNameAndCurve("CircularIn", StockCurve::CircularIn));
    model2->append(mapForNameAndCurve("BackIn", StockCurve::BackIn));

    StockcurveList *list2 = new StockcurveList();
    list2->setTitle("In");
    list2->setDataModel(model2);

    ArrayDataModel *model3 = new ArrayDataModel();
    model3->append(mapForNameAndCurve("Linear", StockCurve::Linear));
    model3->append(mapForNameAndCurve("SineInOut", StockCurve::SineInOut));
    model3->append(mapForNameAndCurve("CubicInOut", StockCurve::CubicInOut));
    model3->append(mapForNameAndCurve("QuarticInOut", StockCurve::QuarticInOut));
    model3->append(mapForNameAndCurve("QuarticInOut", StockCurve::QuarticInOut));
    model3->append(mapForNameAndCurve("QuinticInOut", StockCurve::QuinticInOut));
    model3->append(mapForNameAndCurve("CircularInOut", StockCurve::CircularInOut));
    model3->append(mapForNameAndCurve("BackInOut", StockCurve::BackInOut));

    StockcurveList *list3 = new StockcurveList();
    list3->setTitle("InOut");
    list3->setDataModel(model3);

    // Connect the trigger signals to set up and play the animation.
    connectResult = connect(list1, SIGNAL(triggered(QVariantList)), SLOT(itemTriggered(QVariantList)));
    Q_ASSERT(connectResult);
    connectResult = connect(list2, SIGNAL(triggered(QVariantList)), SLOT(itemTriggered(QVariantList)));
    Q_ASSERT(connectResult);
    connectResult = connect(list3, SIGNAL(triggered(QVariantList)), SLOT(itemTriggered(QVariantList)));
    Q_ASSERT(connectResult);

    // Connect the triggered list to clear the other list from selection.
    connectResult = connect(list1, SIGNAL(triggered(QVariantList)), list2, SLOT(clearSelection()));
    Q_ASSERT(connectResult);
    connectResult = connect(list1, SIGNAL(triggered(QVariantList)), list3, SLOT(clearSelection()));
    Q_ASSERT(connectResult);
    connectResult = connect(list2, SIGNAL(triggered(QVariantList)), list1, SLOT(clearSelection()));
    Q_ASSERT(connectResult);
    connectResult = connect(list2, SIGNAL(triggered(QVariantList)), list3, SLOT(clearSelection()));
    Q_ASSERT(connectResult);
    connectResult = connect(list3, SIGNAL(triggered(QVariantList)), list1, SLOT(clearSelection()));
    Q_ASSERT(connectResult);
    connectResult = connect(list3, SIGNAL(triggered(QVariantList)), list2, SLOT(clearSelection()));
    Q_ASSERT(connectResult);

    parent->add(list1);
    parent->add(list2);
    parent->add(list3);
}

QVariantMap StockCurveRecipe::mapForNameAndCurve(const QString name, StockCurve curve) {
    QVariantMap itemMap;
    itemMap["name"] = name;
    itemMap["curve"] = QVariant::fromValue<StockCurve>(curve);
    return itemMap;
}

void StockCurveRecipe::itemTriggered(QVariantList indexPath)
{
    StockcurveList *senderControl = qobject_cast<StockcurveList*>(sender());
    if(senderControl) {
        ArrayDataModel *dataModel = qobject_cast<ArrayDataModel*>(senderControl->dataModel());
        if(dataModel) {
            QVariantMap itemMap = dataModel->data(indexPath).toMap();
            playAnim(itemMap["name"].toString(), itemMap["curve"].value<StockCurve>());
        }
    }
}

void StockCurveRecipe::playAnim(const QString name, StockCurve curve)
{
    // Set the StockCurve name stored in the "name" property.
    mStockCurveDescription->setText(name);

    // If the Easing curve selected results in an abrupt stop,
    // we break the egg when the animation is over.
    if (name.indexOf("InOut") == -1
            && (name.indexOf("In") >= 0 || name.indexOf("Elastic") >= 0
                    || name.indexOf("Bounce") >= 0 || name.indexOf("Linear") >= 0)) {
        mEgg->setProperty("breakEgg", QVariant(true));
    } else {
        mEgg->setProperty("breakEgg", QVariant(false));
    }

    // Get the selected easing curve, update the animation, and play it
    // (if the animation is already running, we first stop it to avoid conflicts).
    mEaseAnim->setEasingCurve(curve);
    mAnim->stop();
    mAnim->play();
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

void StockCurveRecipe::screenLayoutFrameUpdated(QRectF layoutRect)
{
    mEaseAnim->setToY(layoutRect.height() - 200);

    // Run the seqeuntial animation once we know the animation distance,
    // this happens on creation since the layout frame is for the main Container.
    mAnim->play();
}
