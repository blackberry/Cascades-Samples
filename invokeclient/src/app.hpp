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

#ifndef APP_HPP
#define APP_HPP

#include <bb/cascades/GroupDataModel>
#include <bb/system/CardDoneMessage>
#include <bb/system/InvokeManager>
#include <bb/system/SystemDialog.hpp>
#include <bb/cascades/Invocation>
#include <bb/cascades/InvokeQuery>
#include <QObject>

/*!
 * @brief Application GUI object
 */
//! [0]
class App: public QObject {
    Q_OBJECT

    // The properties to configure an invocation request
    Q_PROPERTY(int targetType READ targetType WRITE setTargetType NOTIFY targetTypeChanged)
    Q_PROPERTY(QString action READ action WRITE setAction NOTIFY actionChanged)
    Q_PROPERTY(QString mimeType READ mimeType WRITE setMimeType NOTIFY mimeTypeChanged)
    Q_PROPERTY(QString uri READ uri WRITE setUri NOTIFY uriChanged)
    Q_PROPERTY(QString data READ data WRITE setData NOTIFY dataChanged)
    Q_PROPERTY(QString target READ target WRITE setTarget NOTIFY targetChanged)

    // The model property that lists the invocation targets query results
    Q_PROPERTY(bb::cascades::GroupDataModel* model READ model CONSTANT)

    // The current error message
    Q_PROPERTY(QString errorMessage READ errorMessage NOTIFY errorMessageChanged)

    // The current status message
    Q_PROPERTY(QString statusMessage READ statusMessage NOTIFY statusMessageChanged)

public:
    App(QObject *parent = 0);

public Q_SLOTS:
    // This method is called to invoke another application with the current configuration
    void invoke();

    // This method is called to query for all applications that can be invoked with the current configuration
    void query();

    // This method invokes the menu service with the Invoke Request. Only works with platform actions such as SET, SHARE, OPEN etc.
    void platformInvoke();

    // This method is called to invoke a specific application with the given @p target id
    void invokeTarget(const QString &target);

    // This method clears the current error message
    void clearError();

    // This method shows an error dialog with he current error message
    void showErrorDialog();

Q_SIGNALS:
    // The change notification signals of the properties
    void targetTypeChanged();
    void actionChanged();
    void mimeTypeChanged();
    void uriChanged();
    void dataChanged();
    void targetChanged();
    void errorMessageChanged();
    void statusMessageChanged();

    // This signal is emitted if the query() call was successful
    void queryFinished();

    // This signal is emitted to trigger a close of the query result sheet
    void closeQueryResults();

private Q_SLOTS:
    // This slot handles the result of an invocation
    void processInvokeReply();

    // This slot handles the result of a target query
    void processQueryReply();

    // This slot updates the status message if the user has started to peek an invoked card
    void peekStarted(bb::system::CardPeek::Type);

    // This slot updates the status message if the user has finished to peek an invoked card
    void peekEnded();

    // This slot updates the status message when the invocation of a card is done
    void childCardDone(const bb::system::CardDoneMessage&);

    // This slot triggers the platform invocation via m_invocation
    void onArmed();

private:
    // The accessor methods of the properties
    int targetType() const;
    void setTargetType(int targetType);
    QString action() const;
    void setAction(const QString &action);
    QString mimeType() const;
    void setMimeType(const QString &mimeType);
    QString uri() const;
    void setUri(const QString &uri);
    QString data() const;
    void setData(const QString &data);
    QString target() const;
    void setTarget(const QString &target);
    bb::cascades::GroupDataModel* model() const;
    QString errorMessage() const;
    QString statusMessage() const;

    // The property values
    int m_targetType;
    QString m_action;
    QString m_mimeType;
    QString m_uri;
    QString m_data;
    QString m_target;
    bb::cascades::GroupDataModel* m_model;
    QString m_errorMessage;
    QString m_statusMessage;

    // The error dialog
    bb::system::SystemDialog* m_dialog;

    // The central object to manage invocations
    bb::system::InvokeManager* m_invokeManager;

    // The Invocation object for platform ivnocations
    bb::cascades::Invocation* m_invocation;
};
//! [0]

#endif
