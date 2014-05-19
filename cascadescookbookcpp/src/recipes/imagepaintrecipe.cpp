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
#include "imagepaintrecipe.h"

#include <bb/cascades/Application>
#include <bb/cascades/Container>
#include <bb/cascades/Color>
#include <bb/cascades/ColorTheme>
#include <bb/cascades/DockLayout>
#include <bb/cascades/ImagePaint>
#include <bb/cascades/Paint>
#include <bb/cascades/VisualStyle>
#include <bb/cascades/Label>
#include <bb/cascades/SystemDefaults>


using namespace bb::cascades;

ImagePaintRecipe::ImagePaintRecipe(Container *parent) :
        CustomControl(parent)
{
    bool connectResult;
    Q_UNUSED(connectResult);

    // Random image source for the Image Paint Definition is set.
    QString file,name = "";
    randomImageSource(file,name);

    // Get the UIConfig object in order to use resolution independent sizes.
    UIConfig *ui = this->ui();

    mRecipeContainer = Container::create().top(ui->du(3)).left(ui->du(3)).right(ui->du(3));
    mRecipeContainer->setHorizontalAlignment(HorizontalAlignment::Fill);
    mRecipeContainer->setVerticalAlignment(VerticalAlignment::Fill);

    // ImagePaint defines which image to paint with and which
    // repeat pattern that should be used (both X and Y direction in this
    // case). The provided image needs to have a width and height that
    // is a power of two, for optimization purposes. Furthermore, the image
    // has to be accompanied by a meta file (with file ending .amd)
    // with the "repeatable" property set to true. Please note that the
    // .png format extension has been dropped on the file name in order for the matching
    // images/imagepaint/Tile_nistri_16x16.amd file to be found.
    ImagePaint paint(QUrl(file), RepeatPattern::XY);
    mRecipeContainer->setBackground(paint);

    Container *LabelCon = new Container();
    LabelCon->setTopPadding(ui->du(5));
    LabelCon->setHorizontalAlignment(HorizontalAlignment::Fill);

    Label *headerLabel = new Label();
    headerLabel->setText((const QString) "Image Paint Definition Source:");
    headerLabel->textStyle()->setBase(SystemDefaults::TextStyles::smallText());
    headerLabel->setHorizontalAlignment(HorizontalAlignment::Center);

    Label *imageSource = new Label();
    imageSource->setText(name);
    imageSource->textStyle()->setBase(SystemDefaults::TextStyles::bigText());
    imageSource->setHorizontalAlignment(HorizontalAlignment::Center);

    LabelCon->add(headerLabel);
    LabelCon->add(imageSource);

    mRecipeContainer->add(LabelCon);

    setRoot(mRecipeContainer);
}

void ImagePaintRecipe::randomImageSource(QString& file, QString& name)
{
    int randomNum;
    randomNum = rand() % 7 + 1;

    switch(randomNum){
           case 1: file = "asset:///images/imagepaint/nistri_16x16.amd"; name ="Nistri";
               break;
           case 2: file = "asset:///images/imagepaint/pyamas_16x16.amd"; name ="Pyamas";
               break;
           case 3: file = "asset:///images/imagepaint/tactile_stripes_16x16.amd"; name ="Tactile";
               break;
           case 4: file = "asset:///images/imagepaint/white_stripes_16x16.amd"; name ="White Stripes";
               break;
           case 5: file = "asset:///images/imagepaint/scribble_light_256x256.amd"; name ="Scribble Light";
               break;
           case 6: file = "asset:///images/imagepaint/light_toast_128x128.amd"; name ="Light Toast";
               break;
           case 7: file = "asset:///images/imagepaint/gplay_256x256.amd"; name ="Gplay";
               break;
       }
}


