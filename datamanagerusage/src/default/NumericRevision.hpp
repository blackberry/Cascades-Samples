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

#ifndef NUMERICREVISION_HPP
#define NUMERICREVISION_HPP

#include <bb/cascades/datamanager/Revision>
#include <QDebug>

/*!
 * A Revision implementation which uses a 64 bit unsigned integer as the
 * revision.
 */
class NumericRevision: public bb::cascades::datamanager::Revision {
public:
    /*!
     * Constructor.
     *
     * The revision number will be zero.
     *
     */
    NumericRevision();

    /*!
     * Constructor.
     *
     * @param revision The numeric revision number as a 64-bit unsigned integer.
     *
     */
    explicit NumericRevision(quint64 revision);

    /*!
     * Copy constructor.
     *
     * @param other The @c NumericRevision object to copy data from.
     *
     */
    NumericRevision(const NumericRevision& other);

    /*!
     * Destructor.
     *
     */
    virtual ~NumericRevision();

    /*!
     * @see Revision::equals
     *
     */
    bool equals(const bb::cascades::datamanager::Revision &other) const;

    /*!
     * @see Revision::greaterThan
     *
     */
    bool greaterThan(const bb::cascades::datamanager::Revision &other) const;

    /*!
     * @see Revision::update
     *
     */
    bb::cascades::datamanager::Revision* newRevision(
            const bb::cascades::datamanager::Revision &other) const;

    /*!
     * @see Revision::toString
     *
     */
    QString toString() const;

private:
    quint64 m_revision;
};

#endif /* NUMERICREVISION_HPP */
