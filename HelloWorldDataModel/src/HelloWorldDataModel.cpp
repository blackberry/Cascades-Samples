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

/*
 * HelloWorldDataModel.cpp
 */

#include "HelloWorldDataModel.h"

HelloWorldDataModel::HelloWorldDataModel()
{
}

HelloWorldDataModel::~HelloWorldDataModel()
{
}

int HelloWorldDataModel::childCount(const QVariantList& indexPath)
{
    // In this very simple data model,
    // the first header has 2 children and the second has 3.
    int level = indexPath.size();
    if (level == 0)
    {
        return 2; // headers "hello" and "world"
    }

    if (level == 1)
    {
        return 2 + indexPath[0].toInt();
    }
    return 0;
}

bool HelloWorldDataModel::hasChildren(const QVariantList& indexPath)
{
    // Performance is not an issue with this data model.
    // So just let childCount tell us if we have children.
    return childCount(indexPath) > 0;
}

/*
 * Return data as a string QVariant for any requested indexPath.
 * We could add defensive code to ensure that the data is not
 * out of range, but ListView honours the results of hasChildren
 * and childCount.
 */
QVariant HelloWorldDataModel::data(const QVariantList& indexPath)
{
    // Data is hard-coded
    //  "hello"
    //     "hello_0"
    //     "hello_1"
    //  "world"
    //     "world_0"
    //     "world_1"
    //     "world_2"
    QString value;
    if (indexPath.size() > 0)
    {
        switch (indexPath[0].toInt())
        {
            case 0:  {value = "hello"; break;}
            case 1:  {value = "world"; break;}
            default: {value = "unexpected"; break;}
        }
        if (indexPath.size() > 1)
        {
            value += QString("_%1").arg(indexPath[1].toInt());
        }
    }
    qDebug() << "Data for " << indexPath << " is " << value;
    return QVariant(value);
}
