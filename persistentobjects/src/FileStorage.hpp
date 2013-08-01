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

#ifndef FILESTORAGE_HPP
#define FILESTORAGE_HPP

#include "Storage.hpp"

using namespace bb::cascades;

class FileStorage: public Storage
{
public:
    FileStorage();
    virtual ~FileStorage();
    virtual bool clear();
    virtual int load(int& lastID, GroupDataModel *model);
    virtual bool save(int lastID, GroupDataModel *model);

private:
    static const QString m_personsFilePath;
    bool serializeDataModel(int lastID, GroupDataModel* model, QDataStream* stream);
    int deserializeIntoDataModel(QDataStream *stream, GroupDataModel *model, int& lastID);
    bool loadPerson(QDataStream* stream, GroupDataModel *model);
    bool loadLastCustomerID(QDataStream* stream, int& id);
};

#endif
