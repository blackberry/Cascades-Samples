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

#ifndef DATAMODELLOGGER_H_
#define DATAMODELLOGGER_H_

#include <bb/cascades/datamanager/DataModelDecorator>

/**
 * A data model that wraps another data model and logs all of its interactions.
 * The intended purpose of this class is to enable highly-verbose logging for
 * debugging only.
 *
 * It is not recommended to enable this class in production code.
 */
//! [0]
class DataModelLoggerDecorator: public bb::cascades::datamanager::DataModelDecorator {
    Q_OBJECT

    // Property for toggling data logging.
    Q_PROPERTY(bool dataDetails READ logData WRITE setLogData)

    // Property for toggling the start of each function logging.
    Q_PROPERTY(bool funcBegin READ logBegin WRITE setLogBegin)

    // Property for toggling itemsChanged callback events logging.
    Q_PROPERTY(bool allCallbacks READ logAllCallbacks WRITE setLogAllCallbacks)

public:
    explicit DataModelLoggerDecorator(QObject *parent = 0);
    virtual ~DataModelLoggerDecorator();

    /**
     * Call the method childCount of the DataModel it wraps
     *
     * @see DataModel
     */
    virtual int childCount(const QVariantList &indexPath);

    /*
     * Call the method hasChildren of the DataModel it wraps
     *
     * @see DataModel
     */
    virtual bool hasChildren(const QVariantList &indexPath);

    /**
     * Call the method itemType of the DataModel it wraps
     *
     * @see DataModel
     */
    virtual QString itemType(const QVariantList &indexPath);

    /*
     * Call the method data of the DataModel it wraps
     *
     * @see DataModel
     */
    virtual QVariant data(const QVariantList &indexPath);

    /**
     * Tell the DataModel to load if it has an invokable load() function.
     */
    Q_INVOKABLE void load();

    /**
     * Whether to log the details of each data item that is returned.
     * return If true, log all details.
     */
    bool logData();

    /**
     * Setter for the logData variable.
     */
    void setLogData(bool enable);

    /**
     * Whether to log the start of each instrumented function.
     * return If true, log the start of functions.
     */
    bool logBegin();

    /**
     * Setter for the logBegin variable.
     */
    void setLogBegin(bool enable);

    /**
     * Whether to log all itemsChanged callback events, or just the interesting ones,
     * e.g. add/remove.
     * return If true, log all itemsChanged callback events.
     */
    bool logAllCallbacks();

    /**
     * Setter for the logAllCallbacks variable.
     */
    void setLogAllCallbacks(bool enable);

public Q_SLOTS:
    // This method emits the onItemAdded signal with the added index path.
    virtual void onItemAdded(QVariantList indexPath);

    // This method emits the onItemUpdated signal with the updated index path.
    virtual void onItemUpdated(QVariantList indexPath);

    // This method emits the onItemRemoved signal with the removed index path.
    virtual void onItemRemoved(QVariantList indexPath);

    // This method emits the onItemsChanged signal with the type of change and IndexMapper to map currently cached items.
    virtual void onItemsChanged(
            bb::cascades::DataModelChangeType::Type eChangeType =
                    bb::cascades::DataModelChangeType::Init,
            QSharedPointer<bb::cascades::DataModel::IndexMapper> indexMapper =
                    QSharedPointer<bb::cascades::DataModel::IndexMapper>(0));

Q_SIGNALS:
    // This signal is emited when model is loaded.
    void loaded();

private:
    // Timer used for logging execution time.
    QElapsedTimer m_timer;

    bool m_logBegin;
    bool m_logData;
    bool m_logAllCallbacks;

    // This method pretty prints the start of method with given list argument and starts the execution timer.
    void logStart(const QString &method, const QVariantList &list);

    // This method pretty prints the end of the started method with the methods result and elapsed execution time.
    void logEnd(const QString &method, const QVariantList &list,
            const QString &result);Q_DISABLE_COPY(DataModelLoggerDecorator)
    ;
};
//! [0]
#endif /* DATAMODELLOGGER_H_ */
