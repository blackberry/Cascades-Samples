/* Copyright (c) 2012, 2013, 2014 BlackBerry Limited.
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
#include "recipeitemfactory.h"
#include "recipeitem.h"

using namespace bb::cascades;

RecipeItemFactory::RecipeItemFactory()
{
}

VisualNode * RecipeItemFactory::createItem(ListView* list, const QString &type)
{
    // We only have one item type so we do not need to check the type variable
    Q_UNUSED(type);
    Q_UNUSED(list);

    RecipeItem *recipeItem = new RecipeItem();
    return recipeItem;
}

void RecipeItemFactory::updateItem(ListView* list, bb::cascades::VisualNode *listItem,
        const QString &type, const QVariantList &indexPath, const QVariant &data)
{
    Q_UNUSED(list);
    Q_UNUSED(indexPath);
    Q_UNUSED(type);

    // Update the control with correct data
    QVariantMap map = data.value<QVariantMap>();
    RecipeItem *recipeItem = static_cast<RecipeItem *>(listItem);
    recipeItem->updateItem(map["title"].toString(), map["image"].toString());
}
