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
#ifndef _STOCKCURVELIST_H_
#define _STOCKCURVELIST_H_

#include <bb/cascades/Color>
#include <bb/cascades/CustomControl>

using namespace bb::cascades;

namespace bb
{
    namespace cascades
    {
        class Container;
        class DataModel;
        class Label;
        class ListView;
    }
}

/**
 * StockcurveList Description:
 *
 * A CustomControl that extends the CheckBox to be an entire item
 * with a title label and a colored box.
 */
class StockcurveList: public bb::cascades::CustomControl
{
    Q_OBJECT
    Q_PROPERTY( QString title READ title WRITE setTitle NOTIFY titleChanged FINAL)
    Q_PROPERTY(bb::cascades::DataModel *dataModel READ dataModel WRITE setDataModel NOTIFY dataModelChanged FINAL)

public:
    /**
     * Constructor that sets up the CustomControl for the selection checkbox
     * @param parent The parent Container, if not specified, 0 is used.
     **/
    StockcurveList(Container * parent = 0);

    /**
     * Accessor and Mutator methods for mTitle
     **/
    void setTitle(const QString &title);
    QString title() const;


    /**
     * Assigns a DataModel to the ListView in the Control.
     *
     * @param dataModel The DataModel to assign to the Control ListView.
     */
    Q_SLOT void setDataModel(bb::cascades::DataModel *dataModel);

    /**
     * Gets the DataModel assigned to Controls ListView.
     *
     * @return The DataModel assigned to the Controls ListView.
     */
    DataModel *dataModel() const;

    /**
     * Clears all selections made in the Controls ListView.
     */
    Q_SLOT void clearSelection();
signals:
    /**
     * Emitted when the title changes
     *
     * @param title The new title
     */
    void titleChanged(const QString title);

    /**
     * Emitted when the DataModel associated with the ListView in the Conrol has changed.
     *
     * @param dataModel The new DataModel.
     */
    void dataModelChanged(bb::cascades::DataModel *dataModel);

    /**
     * Emitted when an item in the list has been tapped.
     *
     * @param indexPath The index path to the triggered item.
     */
    void triggered(QVariantList indexPath);

private:
    /**
     * This function is called when an item in the Controls
     * ListView is tapped, it will forward the signal so that
     * Objects listening for the trigger signal above can react on it.
     *
     * @param indexPath The index path to the triggered item.
     */
    Q_SLOT void itemTriggered(QVariantList indexPath);

    QString mTitle;
    Label *mTitleLabel;
    ListView *mListView;
    DataModel *mDataModel;
};

#endif // ifndef _STOCKCURVELIST_H_
