/* Copyright (c) 2012, 2013  BlackBerry Limited.
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

#include "vegetablesdatamodel.hpp"

/**
 * The data of the VegetablesDataModel have the following form:
 *
 * - Green
 *   + Cucumber
 *   + Peas
 *   + Salad
 * - Red
 *   + Tomato
 *   + Red Radish
 *   + Carrot
 * - Yellow
 *   + Corn
 *   + Paprika
 */
//! [0]
VegetablesDataModel::VegetablesDataModel(QObject *parent)
    : bb::cascades::DataModel(parent)
{
}
//! [0]

//! [1]
int VegetablesDataModel::childCount(const QVariantList& indexPath)
{
    /**
     * In this very simple data model, the first two headers has 3 children
     * and the third one has 2.
     */
    const int level = indexPath.size();

    if (level == 0) { // The number of top-level items is requested
        return 3; // headers "Green", "Red" and "Yellow"
    }

    if (level == 1) { // The number of child items for a header is requested
        const int header = indexPath[0].toInt();

        if (header == 0 || header == 1) // "Green" or "Red"
            return 3;
        else // "Yellow"
            return 2;
    }

    // The number of child items for 2nd level items is requested -> always 0
    return 0;
}
//! [1]

//! [2]
bool VegetablesDataModel::hasChildren(const QVariantList& indexPath)
{
    // Performance is not an issue with this data model.
    // So just let childCount tell us if we have children.
    return childCount(indexPath) > 0;
}
//! [2]

/*
 * Return data as a string QVariant for any requested indexPath.
 * We could add defensive code to ensure that the data is not
 * out of range, but ListView honours the results of hasChildren
 * and childCount.
 */
//! [3]
QVariant VegetablesDataModel::data(const QVariantList& indexPath)
{
    QString value;

    if (indexPath.size() == 1) { // Header requested
        switch (indexPath[0].toInt()) {
            case 0:
                value = tr("Green");
                break;
            case 1:
                value = tr("Red");
                break;
            case 2:
                value = tr("Yellow");
                break;
        }
    }

    if (indexPath.size() == 2) { // 2nd-level item requested
        const int header = indexPath[0].toInt();
        const int childItem = indexPath[1].toInt();

        switch (header) {
            case 0: // "Green"
                switch (childItem) {
                    case 0:
                        value = tr("Cucumber");
                        break;
                    case 1:
                        value = tr("Peas");
                        break;
                    case 2:
                        value = tr("Salad");
                        break;
                }
                break;
            case 1: // "Red"
                switch (childItem) {
                    case 0:
                        value = tr("Tomato");
                        break;
                    case 1:
                        value = tr("Red Radish");
                        break;
                    case 2:
                        value = tr("Carrot");
                        break;
                }
                break;
            case 2: // "Yellow"
                switch (childItem) {
                    case 0:
                        value = tr("Corn");
                        break;
                    case 1:
                        value = tr("Paprika");
                        break;
                }
                break;
        }
    }

    qDebug() << "Data for " << indexPath << " is " << value;

    return QVariant(value);
}
//! [3]

//! [4]
QString VegetablesDataModel::itemType(const QVariantList& indexPath)
{
    switch (indexPath.size()) {
        case 0:
            return QString();
            break;
        case 1:
            return QLatin1String("header");
            break;
        default:
            return QLatin1String("item");
            break;
    }
}
//! [4]
