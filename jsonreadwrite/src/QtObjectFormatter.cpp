/****************************************************************************
 **
 ** Portions Copyright (C) 2012, 2013  BlackBerry Limited.
 ** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
 ** All rights reserved.
 ** Contact: BlackBerry Ltd. (http://www.blackberry.com/company/contact/)
 ** Contact: Nokia Corporation (qt-info@nokia.com)
 **
 ** This file is part of the examples of the BB10 Platform and is derived
 ** from a similar file that is part of the Qt Toolkit.
 **
 ** You may use this file under the terms of the BSD license as follows:
 **
 ** "Redistribution and use in source and binary forms, with or without
 ** modification, are permitted provided that the following conditions are
 ** met:
 **   * Redistributions of source code must retain the above copyright
 **     notice, this list of conditions and the following disclaimer.
 **   * Redistributions in binary form must reproduce the above copyright
 **     notice, this list of conditions and the following disclaimer in
 **     the documentation and/or other materials provided with the
 **     distribution.
 **   * Neither the name of BlackBerry, nor the name of Nokia
 **     Corporation and its Subsidiary(-ies), nor the names of its
 **     contributors may be used to endorse or promote products
 **     derived from this software without specific prior written
 **     permission.
 **
 ** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 ** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 ** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 ** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 ** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 ** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 ** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 ** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 ** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 ** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 ** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
 **
 ****************************************************************************/

#include "QtObjectFormatter.hpp"

#include <iostream>
#include <stdarg.h>

#include <QtCore/QDebug>
#include <QtCore/QMetaType>

Q_DECLARE_METATYPE(QList<QVariantMap>)

QtObjectFormatter::QtObjectFormatter()
{
}

void QtObjectFormatter::traverse(const QVariant &value, QString &out) const
{
    switch (value.type()) {
        case QVariant::Map:
            {
                const QVariantMap object = value.value<QVariantMap>();
                QMapIterator<QString, QVariant> it(object);
                while(it.hasNext()) {
                    it.next();
                    out += it.key();
                    out += ": ";
                    traverse(it.value(), out);
                    out += "\n";
                }
            }
            break;
        case QVariant::List:
            {
                const QVariantList list = value.value<QVariantList>();
                QListIterator<QVariant> it(list);
                int index = 0;
                while(it.hasNext()) {
                    QVariant value = it.next();
                    out += "item[";
                    out += QString::number(index++);
                    out += "]:\n";
                    traverse(value, out);
                }
                out += "\n";
            }
            break;
        case QVariant::String:
            {
                out += "\"";
                out += value.toString();
                out += "\"";
            }
            break;
        case QVariant::Bool:
            {
                out += value.toString();
                out += " (Bool)";
            }
            break;
        case QVariant::Int:
            {
                out += value.toString();
                out += " (Int)";
            }
            break;
        case QVariant::Double:
            {
                out += value.toString();
                out += " (Double)";
            }
            break;
        case QVariant::LongLong:
            {
                out += value.toString();
                out += " (LongLong)";
            }
            break;
        default:
            if (value.canConvert< QList<QVariantMap> >())
            {
                const QList<QVariantMap> list = value.value< QList<QVariantMap> >();
                QListIterator<QVariantMap> it(list);
                int index = 0;
                while(it.hasNext()) {
                    QVariant value = it.next();
                    out += "item[";
                    out += QString::number(index++);
                    out += "]:\n";
                    traverse(value, out);
                }
                out += "\n";
                break;
            }
            qWarning() << "Unsupported property type: " << value.typeName();
            break;
    }
}

QString QtObjectFormatter::asString(const QVariant &value) const
{
    QString out;
    out.reserve(200);
    traverse(value, out);
    return out;
}
