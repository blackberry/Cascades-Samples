/* Copyright (c) 2013 BlackBerry Limited.
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
#ifndef LOADMODELDECORATOR_HPP_
#define LOADMODELDECORATOR_HPP_

#include <bb/cascades/datamanager/DataModelDecorator>

/**
 * LoadModelDecorator Description
 *
 * A data model decorator can be used to reinterpret the data of in a model
 * before it is presented in a ListView. Here we use it to add a loading item
 * at the end of the list.
 */
class LoadModelDecorator: public bb::cascades::datamanager::DataModelDecorator
{
    Q_OBJECT

    /**
     * Property controlling if the model is in a loading state.
     */
    Q_PROPERTY(bool loadingItems READ loadingItems WRITE setLoadingItems NOTIFY loadingItemsChanged FINAL)

    /**
     * Property controlling if the last item in the model should be a loading item.
     */
    Q_PROPERTY(bool showDecorator READ showDecorator WRITE setShowDecorator NOTIFY showDecoratorChanged FINAL)

public:
    /**
     * Constructor.
     *
     * @param parent The parent owner or 0. Optional and will default to 0 if not specified.
     */
    LoadModelDecorator(QObject *parent = 0);

    /**
     * Destructor.
     */
    virtual ~LoadModelDecorator();

    /**
     * Call the method data of the DataModel being decorated. Sets
     * up a load item for the last item in the list if showDecorator is
     * true, otherwise returns the data of the DataModel.
     *
     * @param The path to the data item to get child count for.
     */
    Q_INVOKABLE virtual QVariant data(const QVariantList &indexPath);

    /**
     * Returns the child count of the model + 1 if showDecorator is true.
     * If not decorated the childcount of the DataModel is returned.
     *
     * @param The path to the data item to get child count for.
     * @return The number of children. The return value for invalid index paths is undefined.
     */
    Q_INVOKABLE virtual int childCount(const QVariantList& indexPath);

    /**
     * Function that return the type of item in the list. In our case the
     * first and last item is special.
     *
     * @param indexPath The path to the item in the model.
     */
    Q_INVOKABLE virtual QString itemType(const QVariantList &indexPath);

    /**
     * The state of the decorated model to being in a loading state.
     * Which means it's "waiting" for more items to be added.
     *
     * @return Boolean which is True if loading items otherwise false.
     */
    bool loadingItems() const;

    /**
     * Sets the loading state of the decorated model to either true or false.
     * If true the model is waiting for item changes.
     *
     * @param Boolean which is True if loading items otherwise false.
     */
    Q_SLOT void setLoadingItems(bool value);

    /**
     * Indicates whether the model should be decorated or not.
     *
     * @return True if the model is decorated with an extra item at the end otherwise false.
     */
    bool showDecorator() const;

    /**
     * In some cases it is useful to be able to turn of the special decoration of the
     * last item. In our case if there is no more data we would like the
     * last item to be not be shown. This function enables that functionality.
     *
     * @param value Boolen that should be true if an extra item should be
     *              added as the last item in the DataModel
     */
    Q_SLOT void setShowDecorator(bool value);

    /**
     * This function temporarily hides the decorator item, so
     * that list will scroll it outside of screen.
     */
    Q_INVOKABLE void temporaryHideDecorator();

signals:
    /**
     * Emitted when the loadingItems property change.
     *
     * @param True if the DataModel is in a loading state.
     */
    void loadingItemsChanged(bool value);

    /**
     *  Emitted when the showDecorator property change.
     *
     *  @param True if the DataModel is decorated with an extra item at the end otherwise false.
     */
    void showDecoratorChanged(bool value);

private slots:
    /**
     * Helper function connected to a timer that is used by the temporaryHideDecorator function.
     */
    void showAfterDelay();
private:

    // State variables.
    int mChildCount;
    bool mLoadingItems;
    bool mShowDecorator;
};

#endif /* LOADMODELDECORATOR_HPP_ */
