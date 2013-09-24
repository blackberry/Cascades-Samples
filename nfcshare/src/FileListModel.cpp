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

#include "FileListModel.hpp"

FileListModel::FileListModel(QObject *parent)
{
    setParent(parent);
}

QList<QUrl> FileListModel::files() const
{
    QList<QUrl> fileList;
    for (int row = 0; row < size(); ++row)
        fileList << QUrl::fromLocalFile(value(row));

    return fileList;
}

void FileListModel::addFile(const QString &filePath)
{
    const int pos = indexOf(filePath);
    if (pos != -1)
        return;

    append(filePath);
    emit changed();
}

void FileListModel::removeFile(const QString &filePath)
{
    const int pos = indexOf(filePath);
    if (pos == -1)
        return;

    removeAt(pos);
    emit changed();
}
