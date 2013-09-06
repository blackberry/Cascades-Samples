/*
 * Copyright (c) 2013 BlackBerry Limited.
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

#ifndef SELECTIONDATAMODELDECORATOR_HPP_
#define SELECTIONDATAMODELDECORATOR_HPP_

#include <bb/cascades/datamanager/DataModelDecorator>

/**
 * The SelectionDataModelDecorator class stores information about the selected
 * list items and injects a new data field to mark the selected item.
 */
//! [0]
class SelectionDataModelDecorator: public bb::cascades::datamanager::DataModelDecorator {
    Q_OBJECT

public:
    SelectionDataModelDecorator(QObject *parent = 0);
    virtual ~SelectionDataModelDecorator();

    /*!
     * Call the method data of the DataModel being decorated.
     *
     * @see DataModel
     */
    Q_INVOKABLE virtual QVariant data(const QVariantList &indexPath);

    // This method clears the selection data set.
    Q_INVOKABLE void clearSelections();

    /*!
     * Call the method itemType of the DataModel being decorated.
     *
     * @see DataModel
     */
    virtual QString itemType(const QVariantList &indexPath);

public Q_SLOTS:
    // This method adds or removes the item id from the selection set.
    void changeSelection(QVariantList indexPath, bool selected);

private:

    // The set that tracks all the selected item id's
    QSet<int> m_selectedSet;

    // The internal hightlight color
    QString m_highlight;
};
//! [0]
#endif /* SELECTIONDATAMODELDECORATOR_HPP_ */
