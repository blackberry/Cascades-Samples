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
#include "bucketmodel.h"

#include <bb/data/JsonDataAccess>
#include <bb/cascades/InvokeQuery>

using namespace bb::cascades;

BucketModel::BucketModel(QObject *parent): mBucketIsFull(false), mInvocation(0)
{
    setParent(parent);
}

BucketModel::~BucketModel()
{
}

bool BucketModel::saveData()
{
    JsonDataAccess jda;
    jda.save(mBucketData, mJsonDataPath);

    if (jda.hasError()) {
        bb::data::DataAccessError error = jda.error();
        qDebug() << "JSON loading error: " << error.errorType() << ": " << error.errorMessage();
        return false;
    }

    return true;
}

bool BucketModel::jsonToDataFolder()
{
    // Since we need read-write access to the database, the JSON file has
    // to be moved to a folder where we have that access (application's data folder).
    // First, we check if the file already exists (previously copied)
    QStringList pathSplit = mJsonAssetsPath.split("/");
    QString fileName = pathSplit.last();
    QString dataFolder = QDir::homePath();

    // The path to the file in the data folder
    mJsonDataPath = dataFolder + "/" + fileName;
    QFile newFile(mJsonDataPath);

    if (!newFile.exists()) {
        // If the file is not already in the data folder, we copy it from the
        // assets folder (read only) to the data folder (read and write).
        // Note that on a debug build you will be able to write to a database
        // in the assets folder but that is not possible on a signed application.
        QString appFolder(QDir::homePath());
        appFolder.chop(4);
        QString originalFileName = appFolder + mJsonAssetsPath;
        QFile originalFile(originalFileName);

        if (originalFile.exists()) {
            return originalFile.copy(mJsonDataPath);
        } else {
            qDebug() << "Failed to copy file data base file does not exists.";
            return false;
        }
    }

    return true;
}

void BucketModel::setJsonAssetPath(const QString jsonAssetPath)
{
    if (mJsonAssetsPath.compare(jsonAssetPath) != 0) {
        JsonDataAccess jda;

        // Set and emit the JSON asset path.
        mJsonAssetsPath = jsonAssetPath;
        emit jsonAssetPathChanged(jsonAssetPath);

        if (jsonToDataFolder()) {

            // If the file was either already in the data folder or it was just copied there,
            // it is loaded into the mBucketData QVaraiantlist.
            mBucketData = jda.load(mJsonDataPath).value<QVariantList>();

            if (jda.hasError()) {
                bb::data::DataAccessError error = jda.error();
                qDebug() << "JSON loading error: " << error.errorType() << ": "
                        << error.errorMessage();
                return;
            }

            // After loading the data, check if the bucket list exceeds 100 items.
            checkBucketIsFull();
        }
    }
}

void BucketModel::setFilter(const QString filter)
{
    if (mFilter.compare(filter) != 0) {
        QVariantList filteredBucketData;

        // Remove all the old data.
        clear();

        // Populate a list with the items that has the corresponding status.
        foreach(QVariant v, mBucketData){
        if(v.toMap().value("status") == filter) {
            filteredBucketData << v;
            append(v.toMap());
        }
    }

    // Update the filter property and emit the signal.
        mFilter = filter;
        emit filterChanged(filter);
    }
}

QString BucketModel::filter()
{
    return mFilter;
}

QString BucketModel::jsonAssetPath()
{
    return mJsonAssetsPath;
}

void BucketModel::updateItemStatusAtIndex(QVariantList indexPath, const QString newStatus)
{
    QVariant modelItem = data(indexPath);

    // Two indices are needed: the index of the item in the data list and
    // the index of the item in the current model.
    int itemDataIndex = mBucketData.indexOf(modelItem);
    int itemIndex = indexPath.last().toInt();

    // Update the item in the list of data.
    QVariantMap itemMap = mBucketData.at(itemDataIndex).toMap();
    itemMap["status"] = newStatus;
    mBucketData.replace(itemDataIndex, itemMap);

    // Since the item status was changed, it is removed from the model and
    // consequently it is removed from the current list shown by the app.
    removeAt(itemIndex);
}

void BucketModel::setStatus(const QVariantList selectionList, const QString newStatus)
{
    // If the selectionList parameter is a list of index paths update all the items
    if (selectionList.at(0).canConvert<QVariantList>()) {
        for (int i = selectionList.count() - 1; i >= 0; i--) {
            // Get the item at the index path of position i in the selection list
            QVariantList indexPath = selectionList.at(i).toList();
            updateItemStatusAtIndex(indexPath, newStatus);
        }
    } else {
        updateItemStatusAtIndex(selectionList, newStatus);
    }
    saveData();
}

void BucketModel::deleteItemAtIndex(QVariantList indexPath)
{
    QVariant modelItem = data(indexPath);

    // Two indices are needed: the index of the item in the data list and
    // the index of the item in the current model.
    int itemDataIndex = mBucketData.indexOf(modelItem);
    int itemIndex = indexPath.last().toInt();

    // Remove the item from the data list and from the current data model items.
    mBucketData.removeAt(itemDataIndex);
    removeAt(itemIndex);
}

void BucketModel::deleteBucketItems(const QVariantList selectionList)
{
    // If the selectionList parameter is a list of index paths update all the items
    if (selectionList.at(0).canConvert<QVariantList>()) {
        for (int i = selectionList.count() - 1; i >= 0; i--) {

            // Get the item at the index path of position i in the selection list.
            QVariantList indexPath = selectionList.at(i).toList();
            deleteItemAtIndex(indexPath);
        }
    } else {
        deleteItemAtIndex(selectionList);
    }

    saveData();
    checkBucketIsFull();
}

void BucketModel::addBucketItem(const QString itemTitle)
{
    QVariantMap itemMap;
    itemMap["title"] = QVariant(itemTitle);
    itemMap["status"] = QVariant("todo");

    if (indexOf(itemMap) == -1) {
        if (mFilter.compare("todo") == 0) {
            // New items are added to the todo list. If the filter is set to todo,
            // the current list is shown and the new item is added at the top of the list model.
            insert(0, itemMap);
        }

        // Add the new item to the data list.
        mBucketData.insert(0, itemMap);
        saveData();
    }

    // A new item has been added check to see if the max limit has been reached.
    checkBucketIsFull();
}

void BucketModel::editBucketItem(const QVariant item, const QString newItemTitle)
{
    // Get the indices of the item in the model and in the data list.
    QVariantMap itemMap = item.toMap();
    int itemDataIndex = mBucketData.indexOf(itemMap);
    int itemIndex = indexOf(itemMap);

    // Update the title.
    itemMap["title"] = newItemTitle;

    // And replace the item in both the model and the data list.
    mBucketData.replace(itemDataIndex, itemMap);
    replace(itemIndex, itemMap);

    saveData();
}

bool BucketModel::bucketIsFull()
{
    return mBucketIsFull;
}

void BucketModel::checkBucketIsFull()
{
    // Check if the bucket is full or not. If the status changes, update the property.
    if (mBucketData.count() < mMaxItems) {

        // Only emit the signal if the property actually changed.
        if (mBucketIsFull) {
            mBucketIsFull = false;
            emit bucketIsFullChanged(mBucketIsFull);
        }

    } else if (!mBucketIsFull) {
        if (!mBucketIsFull) {
            mBucketIsFull = true;
            emit bucketIsFullChanged(mBucketIsFull);
        }
    }
}

void BucketModel::shareBucketItem(const QString itemTitle)
{

    //Create a file to share over BBM.
    QFile bucketFile("data/bucketItemToShare.buk");
    if (bucketFile.exists()) {
        bucketFile.remove();
    }

    bucketFile.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(&bucketFile);
    out << itemTitle;
    bucketFile.close();

    //Share the file over BBM using the Invocation Framework.
    QString path = QDir::current().absoluteFilePath("data/bucketItemToShare.buk");

    mInvocation = Invocation::create(
            InvokeQuery::create().parent(this).uri(QUrl::fromLocalFile(path)).invokeTargetId(
                    "sys.bbm.sharehandler"));

    QObject::connect(mInvocation, SIGNAL(armed()), SLOT(onArmed()));

    QObject::connect(mInvocation, SIGNAL(finished()), mInvocation, SLOT(deleteLater()));

}

void BucketModel::onArmed()
{
    // Once the system has armed the invocation trigger it to launch BBM sharing.
    mInvocation->trigger("bb.action.SHARE");
}

void BucketModel::deleteLater()
{
    delete (mInvocation);
}

