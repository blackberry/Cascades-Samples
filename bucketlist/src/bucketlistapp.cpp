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
#include "bucketlistapp.h"
#include "bucketdata/bucketmodel.h"
#include "bucketdata/bucketsettings.h"
#include "bucketbbm/bucketbbmmanager.h"

#include <bb/cascades/AbstractPane>
#include <bb/cascades/LocaleHandler>
#include <bb/cascades/QmlDocument>

#include <bb/system/InvokeManager>
#include <bb/system/InvokeRequest>

using namespace bb::cascades;
using namespace bucketbbm;

BucketListApp::BucketListApp(QObject *parent) : QObject(parent)
{
    // Initiate the Invocation Manager, so that we can react on incoming bucket items
    // (see bar-descriptor.xml for how to register as an invokable app)
    mInvokeManager = new bb::system::InvokeManager(this);

    // Connect to the invoke managers invoked signal.
    bool connectResult = connect(mInvokeManager, SIGNAL(invoked(const bb::system::InvokeRequest&)),
                                            SLOT(handleInvoke(const bb::system::InvokeRequest&)));
    Q_ASSERT(connectResult);

    // Prepare localization.Connect to the LocaleHandlers systemLanguaged change signal, this will
    // tell the application when it is time to load a new set of language strings.
    mTranslator = new QTranslator(this);
    mLocaleHandler = new LocaleHandler(this);
    connectResult = connect(mLocaleHandler, SIGNAL(systemLanguageChanged()), SLOT(onSystemLanguageChanged()));
    Q_ASSERT(connectResult);
    Q_UNUSED(connectResult);

    onSystemLanguageChanged();

    // Initialize the bucketItem title property
    mBucketItemTitle = "";

    // Set the application organization and name, which is used by QSettings
    // when saving values to the persistent store.
    QCoreApplication::setOrganizationName("Example");
    QCoreApplication::setApplicationName("Bucket List Settings");

    // The model for populating the bucket list is registered, so that it and all its
    // properties can be accessed directly from QML. This is done before creating the
    // QmlDocument below so that it is available when the corresponding QML component
    // is needed (see main.qml).
    qmlRegisterType<BucketModel>("com.bucketlist.bucketdata", 1, 0, "BucketModel");

    // The application settings object used to store the BBM connection state
    qmlRegisterType<BucketSettings>("com.bucketlist.bucketdata", 1, 0, "BucketSettings");

    // The BBM manager that can connect the application to BBM and update the BBM status message
    qmlRegisterType<BucketBBMManager>("com.bucketlist.bucketbbm", 1, 0, "BucketBBMManager");

    // Create a QMLDocument and load it, using build patterns.
    QmlDocument *qml = QmlDocument::create("asset:///main.qml").parent(this);

    // Make this object available to the UI as context property
    qml->setContextProperty("_app", this);

    if (!qml->hasErrors()) {

        AbstractPane *appPage = qml->createRootObject<AbstractPane>();

        if (appPage) {
            // Set the main scene to the application Page.
            Application::instance()->setScene(appPage);
        }
    }
}

void BucketListApp::handleInvoke(const bb::system::InvokeRequest& invoke)
{
    //Grab the .buk file we were invoked with.
    QUrl uri = invoke.uri();

    QFile file(uri.toLocalFile());

    mBucketItemTitle = "";

    if (file.open(QIODevice::ReadOnly)) {
        QTextStream in(&file);
        while (!in.atEnd()) {
            mBucketItemTitle += in.readLine();
        }
        file.close();
    }

    emit incomingBucketItem();

}

QString BucketListApp::bucketItemTitle() const
{
    return mBucketItemTitle;
}

void BucketListApp::onSystemLanguageChanged()
{
    // Remove the old translator to make room for the new translation.
    QCoreApplication::instance()->removeTranslator(mTranslator);

    // Initiate, load and install the application translation files.
    QString localeString = QLocale().name();
    QString fileName = QString("bucketlist_%1").arg(localeString);
    if (mTranslator->load(fileName, "app/native/qm")) {
        QCoreApplication::instance()->installTranslator(mTranslator);
    }
}
