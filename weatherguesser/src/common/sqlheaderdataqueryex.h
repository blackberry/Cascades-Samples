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
#ifndef SQLHEADERDATAQUERYEX_H_
#define SQLHEADERDATAQUERYEX_H_

#include <QObject>
#include <bb/cascades/datamanager/SqlHeaderDataQuery>

using namespace bb::cascades::datamanager;

/**
 * SqlHeaderDataQuery Description
 *
 * The SqlHeaderDataQuery does not have an emitDataChanged function
 * so we extend the class by adding the function.
 */
class SqlHeaderDataQueryEx: public SqlHeaderDataQuery
{
Q_OBJECT

public:
    SqlHeaderDataQueryEx(QObject *parent = 0);
    SqlHeaderDataQueryEx(const QString &query, const QString &headerQuery, QObject *parent = 0);
    virtual ~SqlHeaderDataQueryEx();
public:
    /**
     * Emit the signal for data changes if using the default NumericRevision.
     * Can be called from QML to notify the containing model of a source revision change.
     *
     * @return revision the numeric revision of the latest source data.
     */
    Q_SLOT void emitDataChanged (int revision);
};

#endif /* SQLHEADERDATAQUERYEX_H_ */
