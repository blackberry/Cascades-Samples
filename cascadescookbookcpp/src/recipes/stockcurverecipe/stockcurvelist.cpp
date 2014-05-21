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
#include "stockcurvelist.h"
#include "stockcurvelistitemprovider.h"

#include <bb/cascades/Color>
#include <bb/cascades/Container>
#include <bb/cascades/Label>
#include <bb/cascades/ListView>
#include <bb/cascades/SystemDefaults>

using namespace bb::cascades;

StockcurveList::StockcurveList(Container * parent) :
        CustomControl(parent), mDataModel(0)
{
    bool connectResult;
    Q_UNUSED(connectResult);

    // Get the UIConfig object in order to use resolution independent sizes.
    UIConfig *ui = this->ui();

    Container *content = Container::create().background(Color::fromARGB(0xfefefe))
                            .top(ui->du(2)).preferredWidth(ui->du(15));

    mTitleLabel = Label::create().horizontal(HorizontalAlignment::Center)
                            .textStyle(SystemDefaults::TextStyles::subtitleText());
    mTitleLabel->textStyle()->setColor(Color::fromARGB(0xff272727));

    mListView = ListView::create().listItemProvider(new StockListItemProvider());

    connectResult = connect(mListView, SIGNAL(triggered(QVariantList)), SLOT(itemTriggered(QVariantList)));

    content->add(mTitleLabel);
    content->add(mListView);

    Q_ASSERT(connectResult);
    setRoot(content);

    mTitle = "";
}

void StockcurveList::setTitle(const QString &title)
{
    if (mTitle != title) {
        mTitle = title;
        mTitleLabel->setText(mTitle);
        emit titleChanged(mTitle);
        qDebug() << "Title set to " << mTitle;
    }
}

QString StockcurveList::title() const
{
    return mTitle;
}

void StockcurveList::setDataModel(bb::cascades::DataModel* dataModel)
{
    if (mDataModel != dataModel) {
        mDataModel = dataModel;
        mListView->setDataModel(mDataModel);
        emit dataModelChanged(mDataModel);
    }
}

DataModel* StockcurveList::dataModel() const
{
    return mDataModel;
}

void StockcurveList::clearSelection()
{
    mListView->clearSelection();
}

void StockcurveList::itemTriggered(QVariantList indexPath)
{
    // Just reemit the signal for Objects observing this Control
    emit triggered(indexPath);
    mListView->clearSelection();
    mListView->select(indexPath);
}
