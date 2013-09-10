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

#ifndef EXPANDABLEDATAMODELDECORATOR_HPP_
#define EXPANDABLEDATAMODELDECORATOR_HPP_

#include <bb/cascades/datamanager/DataModelDecorator>

/**
 * This class creates an expandable header type of model. It allows the user to show collapsed
 * headers as an overview, and expand the headers items for more granular detail.
 */
//! [0]
class ExpandableDataModelDecorator: public bb::cascades::datamanager::DataModelDecorator {
    Q_OBJECT

public:
    ExpandableDataModelDecorator(QObject* parent = 0);
    virtual ~ExpandableDataModelDecorator();

    /*
     * Inject "expanded" data value to true or false depending
     * if the data index equals the expanded index.
     * Call the method data of the DataModel it wraps.
     *
     * @see DataModel
     */

    Q_INVOKABLE virtual QVariant data(const QVariantList &indexPath);

    /**
     * This method returns none zero children count only when the
     * selected index equals the expand index.
     * Call the method childCount of the DataModel it wraps.
     *
     * @see DataModel
     */
    virtual int childCount(const QVariantList& indexPath);

    /*
     * This method confirms child count only at time when selected
     * index equals that of the expand index.
     * Call the method hasChildren of the DataModel it wraps.
     *
     * @see DataModel
     */
    virtual bool hasChildren(const QVariantList& indexPath);

public Q_SLOTS:

    // This method toggles expanding or collapsing of header items.
    // It sets the expanded index and emits itemsChanged signal once the
    // index has been set to expand or collapse.
    void expandHeader(const QVariantList& indexPath, bool expand);

private:
    // Sets the index to collapse or expand.
    int m_expandedIndex;

    // This is a helper method to verify index path correctness, and whether the
    // index is expandable or not(already expanded).
    bool isExpandable(const QVariantList& indexPath) const;
};
//! [0]
#endif /* EXPANDABLEDATAMODELDECORATOR_HPP_ */
