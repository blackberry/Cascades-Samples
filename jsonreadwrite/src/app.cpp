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

#include "app.hpp"
#include "QtObjectFormatter.hpp"

#include <bb/data/JsonDataAccess>

#include <bb/cascades/Application>
#include <bb/cascades/Page>
#include <bb/cascades/QmlDocument>

using namespace bb::cascades;
using namespace bb::data;

static QString assetPath(const QString& assetName)
{
    return QDir::currentPath() + "/app/native/assets/" + assetName;
}

App::App()
    : mRhsDefaultTitle(tr("Output display area"))
    , mRhsTitle(mRhsDefaultTitle)
    , mResult("results")
    , mState(Init)
{
    // Load the UI description from main.qml
    QmlDocument *qml = QmlDocument::create("asset:///main.qml").parent(this);

    // Make the App object available to the UI as context property
    qml->setContextProperty("_app", this);

    // Create the application scene
    AbstractPane *appPage = qml->createRootObject<AbstractPane>();
    setResultAndState("  ", Init);
    Application::instance()->setScene(appPage);
}

QString App::jsonData() const
{
    return mJsonData;
}

void App::setJsonData(const QString& data)
{
    if (mJsonData == data)
        return;

    mJsonData = data;
    emit jsonDataChanged(data);
}

QString App::rhsTitle() const
{
    return mRhsTitle;
}

void App::setRhsTitle(const QString& title)
{
    if (mRhsTitle == title)
        return;

    mRhsTitle = title;
    emit rhsTitleChanged(title);
}

QString App::rhsText() const
{
    return mRhsText;
}

void App::setRhsText(const QString& text)
{
    if (mRhsText == text)
        return;

    mRhsText = text;
    emit rhsTextChanged(text);
}

QString App::result() const
{
    return mResult;
}

void App::setResult(const QString& result)
{
    if (mResult == result)
        return;

    mResult = result;
    emit resultChanged(result);
}

App::State App::state() const
{
    return mState;
}

void App::setState(const State state)
{
    if (mState == state)
        return;

    mState = state;
    emit stateChanged(state);
}

void App::loadOriginalJson()
{
    const QString result = tr("Loading ... ");
    setResult(result);
    setRhsTitleAndText(mRhsDefaultTitle, "");
    setQtData(QVariant());

    QFile jsonFile(assetPath("snippet.json"));
    loadJsonFileRaw(result, jsonFile);
}

//! [1]
void App::convertJsonToQt()
{
    const QString result = tr("Converting ... ");
    setResult(result);
    setRhsTitleAndText(mRhsDefaultTitle, "");
    setQtData (QVariant());

    JsonDataAccess jda;
    QVariant qtData = jda.loadFromBuffer(mJsonData);
    if (jda.hasError()) {
        const DataAccessError err = jda.error();
        const QString errorMsg = tr("Error converting JSON data: %1").arg(err.errorMessage());
        setResultAndState(errorMsg, JsonLoaded);
    } else {
        setQtData(qtData);
        const QtObjectFormatter fmt;
        setRhsTitleAndText(tr("Qt Data from JSON"), fmt.asString(qtData));
        setResultAndState(result + tr("Success"), QtDisplayed);
    }
}
//! [1]

//! [2]
void App::convertQtToJson()
{
    const QString result = tr("Converting ... ");
    setResult(result);
    setRhsTitleAndText(mRhsDefaultTitle, "");

    QString jsonBuffer;
    JsonDataAccess jda;
    jda.saveToBuffer(mQtData, &jsonBuffer);
    if (jda.hasError()) {
        const DataAccessError err = jda.error();
        const QString errorMsg = tr("Error converting Qt data to JSON: %1").arg(err.errorMessage());
        setResultAndState(errorMsg, QtDisplayed);
    } else {
        setRhsTitleAndText(tr("JSON Data from Qt"), jsonBuffer);
        setResultAndState(result + tr("Success"), ReadyToWrite);
    }
}
//! [2]

void App::updateJsonDataFromQml(const QString &data)
{
    mJsonData = data;
}

//! [3]
void App::writeToJsonFileAndReload()
{
    const QString result = tr("Writing file ... ");
    setResult(result);

    const QDir home = QDir::home();
    QTemporaryFile file(home.absoluteFilePath("XXXXXX.json"));
    if (file.open()) {
        JsonDataAccess jda(&file);
        jda.save(mQtData, &file);
        if (jda.hasError()) {
            const DataAccessError err = jda.error();
            const QString errorMsg = tr("Error writing data to JSON file: %1").arg(err.errorMessage());
            setResultAndState(errorMsg, ReadyToWrite);
        } else {
            file.close();
            loadJsonFileRaw(tr("Reloading new JSON file ... "), file);
        }
    }
}
//! [3]

//! [0]
void App::loadJsonFileRaw(const QString& resultMsg, QFile& jsonFile)
{
    if (!jsonFile.open(QFile::ReadOnly)) {
        const QString msg = tr("Failed to open JSON file: %1").arg(jsonFile.fileName());
        setJsonData (QString());
        setResultAndState(msg, Init);
        return;
    }

    const QString doc = QString::fromUtf8(jsonFile.readAll());
    setJsonData(doc);
    setResultAndState(resultMsg + tr("Success"), JsonLoaded);
}
//! [0]

void App::setQtData(const QVariant& data)
{
    mQtData = data;
}

void App::setResultAndState(const QString& result, State newState)
{
    setResult(result);
    setState(newState);
}

void App::setRhsTitleAndText(const QString& title, const QString& text)
{
    setRhsTitle(title);
    setRhsText(text);
}
