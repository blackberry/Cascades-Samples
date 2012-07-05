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

#include "SqlModel.hpp"

#include  <bb/data/SqlDataAccess>

using namespace bb::data;

SqlModel* SqlModel::fromSQLAsset(const QString &fileName)
{
    SqlDataAccess dbda("app/native/assets/" + fileName);

    // Load the result and extract the list of rows.
    const QVariantList sqlRoot = dbda.execute("SELECT a.firstname, a.surname, b.title, b.price, b.notes FROM author a, book b WHERE a.id = b.authorid").value<QVariantList>();

    // Use the result as data source for a model.
    SqlModel *model = new SqlModel();
    model->append(sqlRoot);
    return model;
}
