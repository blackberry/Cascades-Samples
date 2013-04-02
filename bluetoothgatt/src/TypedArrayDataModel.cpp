/*
 * TypedArrayDataModel.cpp
 *
 *  Created on: Dec 11, 2012
 *      Author: robwilliams
 */

#include "TypedArrayDataModel.hpp"
#include <QDebug>

TypedArrayDataModel::TypedArrayDataModel() {
    // TODO Auto-generated constructor stub

}

TypedArrayDataModel::~TypedArrayDataModel() {
    // TODO Auto-generated destructor stub
}

QString TypedArrayDataModel::itemType(const QVariantList & path) {
    return "item";
}

