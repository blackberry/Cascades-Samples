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

#include "NumericRevision.hpp"

#include <bb/cascades/datamanager/Revision>

NumericRevision::NumericRevision() :
        bb::cascades::datamanager::Revision(), m_revision(0L) {
}

NumericRevision::NumericRevision(quint64 revision) :
        bb::cascades::datamanager::Revision(), m_revision(revision) {
}

NumericRevision::NumericRevision(const NumericRevision& other) :
        bb::cascades::datamanager::Revision() {
    m_revision = other.m_revision;
}

NumericRevision::~NumericRevision() {
}

bool NumericRevision::equals(const Revision &other) const {
    const NumericRevision *otherRev =
            dynamic_cast<const NumericRevision *>(&other);
    if (otherRev == 0) {
        return false;
    }
    return m_revision == otherRev->m_revision;
}

bool NumericRevision::greaterThan(const Revision &other) const {
    const NumericRevision *otherRev =
            dynamic_cast<const NumericRevision *>(&other);
    if (otherRev == 0) {
        return false;
    }
    return m_revision > otherRev->m_revision;
}

bb::cascades::datamanager::Revision* NumericRevision::newRevision(
        const Revision &other) const {
    quint64 newRevision = m_revision;
    const NumericRevision *otherRev =
            dynamic_cast<const NumericRevision *>(&other);
    if (otherRev != 0) {
        if (m_revision < otherRev->m_revision) {
            newRevision = otherRev->m_revision;
        }
    }
    return new NumericRevision(newRevision);
}

QString NumericRevision::toString() const {
    QString result;
    QTextStream s(&result);
    s << "NumericRevision(";
    s << m_revision;
    s << ")";
    return result;
}

