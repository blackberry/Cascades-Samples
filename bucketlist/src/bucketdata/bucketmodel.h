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
#ifndef _BUCKETMODEL_H
#define _BUCKETMODEL_H

#include <bb/cascades/QListDataModel>
#include <bb/data/JsonDataAccess>
#include <QtNetwork/QNetworkReply>
#include <bb/cascades/Invocation>

using namespace bb::data;

// The bucket model is based on the QListDataModel template, which in turn
// implements the abstract DataModel class.
typedef bb::cascades::QListDataModel<QVariantMap> BucketListModel;

/* BucketModel Description:
 *
 * Model class for the Bucket List application, the data model
 * reads and write from a JSON file that keeps all item data
 * for the list.
 */
class BucketModel: public BucketListModel
{
Q_OBJECT

/**
 * The filter determines which data should populate the model, it can be one of: "todo",
 * "finished" or "chickened". If the filter is not one those three is states, the data model
 * will be empty.
 */
Q_PROPERTY(QString filter READ filter WRITE setFilter NOTIFY filterChanged)

/**
 * This property is the path to the JSON file which initially populates the model.
 * This file is moved to the application's data folder in order to have read and
 * write access to it.
 */
Q_PROPERTY(QString jsonAssetPath READ jsonAssetPath WRITE setJsonAssetPath NOTIFY jsonAssetPathChanged)

/**
 * A read-only boolean parameter, which is true as long as there is less than a hundred
 * items in the list. Once it is above that the user should not be able to add
 * more items, the app is all about the one hundred things you want to do before you
 * kick the bucket.
 */
Q_PROPERTY(bool bucketIsFull READ bucketIsFull NOTIFY bucketIsFullChanged)

public:
    /**
     * Constructor that sets up the recipe
     * @param parent The parent Container, if not specified, 0 is used
     */
    BucketModel(QObject *parent = 0);
    ~BucketModel();

    /**
     * This function returns the current filter set on the model, there are three
     * valid filters "todo", "finished" and "chickened".
     *
     * @return A QString with the current filter
     */
    QString filter();

    /**
     * This function sets the current filter that is used to populate the model with data.
     * It can be one of: "todo", "finished" or "chickened". If filter is
     * not one of those three is states, the data model will be empty.
     *
     * @param filter The new filter that should be used when loading data.
     */
    void setFilter(const QString filter);

    /**
     * This function returns the relative file path to the JSON file in the assets folder. Note
     * that this path will only be used the first time the application launches.
     * The file will then be moved to the application's data folder, so that it is
     * possible to write to the file.
     *
     * @return The assets path to the JSON file for the bucket list app.
     */
    QString jsonAssetPath();

    /**
     * Sets the relative path to the JSON file containing the initial bucket list
     * data.
     *
     * @param jsonAssetPath The new relative path to the JSON file residing in the assets folder.
     */
    void setJsonAssetPath(const QString jsonAssetPath);

    /**
     * This function returns the filled status of the bucket list data. The application is set
     * so that the user only will be allowed to have one hundred items in total.
     * When the bucket is full, false is returned.
     *
     * @return True if there is room for more items, otherwise false.
     */
    bool bucketIsFull();

signals:

    /**
     * This signal is emitted when the filter changes.
     *
     * @param filter The new filter string.
     */
    void filterChanged(QString filter);

    /**
     * This signal is emitted when the property holding the path to JSON file in the assets folder is changed.
     *
     * @param jsonAssetPath The new path to the JSON file in the asset path.
     */
    void jsonAssetPathChanged(QString jsonAssetPath);

    /**
     * This signal is emitted when the bucket filled status is changed.
     *
     * @param bucketIsFull True if the item limit of one hundred items is reached, otherwise false.
     */
    void bucketIsFullChanged(bool bucketIsFull);

public slots:
    /**
     * This Slot function changes the status of a list of items.
     *
     * @param selectionList The list of indexPaths to the item that should be changed.
     * @param newStatus The new status that all items should have.
     */
    void setStatus(const QVariantList selectionList, const QString newStatus);

    /**
     * This Slot function deletes the items at the index paths given in the parameter list.
     *
     * @param selectionList A list of indices to the items that should be removed.
     */
    void deleteBucketItems(const QVariantList selectionList);

    /**
     * Slot function that adds a new item to the JSON file, the status will be set to "todo".
     *
     * @param itemTitle The string with the text describing the bucket title.
     */
    void addBucketItem(const QString itemTitle);

    /**
     * This Slot function edits the content of an item.
     *
     * @param item The item that is to be edited.
     * @param newItemTitle The new title text of the item.
     */
    void editBucketItem(const QVariant item, const QString newItemTitle);

    /**
     * Slot function that shares an item over BBM..
     *
     * @param itemTitle The string with the text describing the bucket title.
     */

    void shareBucketItem(const QString itemTitle);

    /**
     * Slot function that is triggered with the invocation item is ready to be fired.
     *
     */
    void onArmed();

    /**
     * Slot function that is triggered when it's safe to clean up the invocation item.
     *
     */
    void deleteLater();
private:
    /**
     * In order to write to a file in a signed application the file has
     * to reside in the app's data folder. This function copies the bundled
     * JSON file to that folder.
     *
     * @return True either if the file has already been copied or if the file is successfully copied, otherwise false.
     */
    bool jsonToDataFolder();

    /**
     * This helper function is for saving the data in mBucketData to the JSON file.
     *
     * @return If save was successful returns true.
     */
    bool saveData();

    /**
     * This helper function is for evaluating if the bucket is full. Alters the bucketIsFull property
     * if the state needs changing and emits the corresponding signal.
     */
    void checkBucketIsFull();

    /**
     * This helper function updates the status of the item at indexPath;
     *
     * @param indexPath item indexPath
     * @param newStatus the updated status string
     */
    void updateItemStatusAtIndex(QVariantList indexPath, const QString newStatus);

    /**
     * This helper function deletes the item at indexPath.
     *
     * @param indexPath item indexPath
     */
    void deleteItemAtIndex(QVariantList indexPath);

    // Property variables
    QString mFilter;
    QString mJsonAssetsPath;
    QString mJsonDataPath;
    bool mBucketIsFull;

    // A list containing all data read from the JSON file
    QVariantList mBucketData;

    // Constant set to the maximum number of bucket items (a hundred
    // things to do before you kick the bucket)
    static const int mMaxItems = 100;

    //Invocation variables
    bb::cascades::Invocation* mInvocation;
};

#endif // ifndef _BUCKETMODEL_H
