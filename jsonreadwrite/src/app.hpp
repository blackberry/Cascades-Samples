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

#ifndef JSON_READWRITE_HPP
#define JSON_READWRITE_HPP

#include <QtCore/QFile>
#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QVariant>

class App: public QObject
{
    Q_OBJECT
    Q_ENUMS(State)
    Q_PROPERTY(QString jsonData READ jsonData WRITE setJsonData NOTIFY jsonDataChanged)
    Q_PROPERTY(QString rhsTitle READ rhsTitle WRITE setRhsTitle NOTIFY rhsTitleChanged)
    Q_PROPERTY(QString rhsText READ rhsText WRITE setRhsText NOTIFY rhsTextChanged)
    Q_PROPERTY(QString result READ result WRITE setResult NOTIFY resultChanged)
    Q_PROPERTY(State state READ state WRITE setState NOTIFY stateChanged)

public:
    enum State {
      Init,
      JsonLoaded,
      QtDisplayed,
      ReadyToWrite
    };

    App();

    QString jsonData() const;
    void setJsonData(const QString& data);

    QString rhsTitle() const;
    void setRhsTitle(const QString& title);

    QString rhsText() const;
    void setRhsText(const QString& text);

    QString result() const;
    void setResult(const QString& result);

    State state() const;
    void setState(const State state);

public Q_SLOTS:
    void loadOriginalJson();
    void convertJsonToQt();
    void convertQtToJson();
    void updateJsonDataFromQml(const QString &data);
    void writeToJsonFileAndReload();

Q_SIGNALS:
    void jsonDataChanged(const QString& data);
    void rhsTitleChanged(const QString& title);
    void rhsTextChanged(const QString& text);
    void resultChanged(const QString& result);
    void stateChanged(State state);

private:
    void loadJsonFileRaw(const QString& resultMsg, QFile& jsonFile);
    void setQtData(const QVariant& data);
    void setResultAndState(const QString& resultMsg, State newState);
    void setRhsTitleAndText(const QString& title, const QString& text);

    QString mJsonData;
    QString const mRhsDefaultTitle;
    QString mRhsTitle;
    QString mRhsText;
    QString mResult;
    State mState;
    QVariant mQtData;
};

#endif
