/*
 * TypedArrayDataModel.hpp
 *
 *  Created on: Dec 11, 2012
 *      Author: robwilliams
 */

#ifndef TYPEDARRAYDATAMODEL_HPP_
#define TYPEDARRAYDATAMODEL_HPP_

#include <bb/cascades/databinding/arraydatamodel.h>

#include <QVariantList>

class TypedArrayDataModel : public bb::cascades::ArrayDataModel {
public:
    TypedArrayDataModel();
    virtual ~TypedArrayDataModel();

    Q_INVOKABLE virtual QString itemType(const QVariantList & path);
};

#endif /* TYPEDARRAYDATAMODEL_HPP_ */
