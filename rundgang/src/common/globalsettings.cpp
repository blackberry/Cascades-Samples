/* Copyright (c) 2013 Research In Motion Limited.
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
#include "globalsettings.h"
#include <QCoreApplication>
#include <QSettings>
#include <QDebug>

#include <bb/system/InvokeManager>
#include <bb/system/InvokeTargetReply>

const QString GlobalSettings::APP_NAME("Rundgang");
const QString GlobalSettings::APP_ORGANIZATION("Example");

const QString GlobalSettings::AUTOEMAILSIGNATURE_KEY("autoEmailSignature");
const QString GlobalSettings::DIRECTEMAIL_KEY("directEmail");
const QString GlobalSettings::EMAILSIGNATURE_KEY("emailSignature");
const QString GlobalSettings::LORESHOTO_KEY("lofiPhoto");

using namespace bb::system;

GlobalSettings::GlobalSettings(QObject* parent) :
        QObject(parent)
{
    // Set up the application organization and name (used by QSettings
    // when saving values to the persistent store).
    QCoreApplication::setOrganizationName(APP_ORGANIZATION);
    QCoreApplication::setApplicationName(APP_NAME);
    QSettings settings;

    // Initiate the properties to either default values if it is the first time
    // the application launch or the persisted values stored in QSettings.
    if (settings.value(AUTOEMAILSIGNATURE_KEY).isNull()) {
        setAutoEmailSignature(true);
    } else {
        setAutoEmailSignature(settings.value(AUTOEMAILSIGNATURE_KEY).toBool());
    }

    if (settings.value(DIRECTEMAIL_KEY).isNull()) {
        setDirectTextEmail(true);
    } else {
        setDirectTextEmail(settings.value(DIRECTEMAIL_KEY).toBool());
    }

    if (settings.value(EMAILSIGNATURE_KEY).isNull()) {
        setEmailSignature(tr("Sent from rundgŒng."));
    } else {
        setEmailSignature(settings.value(EMAILSIGNATURE_KEY).toString());
    }

    if (settings.value(LORESHOTO_KEY).isNull()) {
        setLoresPhoto(false);
    } else {
        setLoresPhoto(settings.value(LORESHOTO_KEY).toBool());
    }

    // Create the InvokeManager used to open the device settings application.
    mInvokeManager = new InvokeManager(this);
}

GlobalSettings::~GlobalSettings()
{
}

void GlobalSettings::setLoresPhoto(bool loresPhoto)
{
    if (loresPhoto != mLoresPhoto) {
        mLoresPhoto = loresPhoto;
        emit loresPhotoChanged(mLoresPhoto);

        // Store the value in QSettings to persist it between app runs.
        QSettings().setValue(LORESHOTO_KEY, QVariant(mLoresPhoto));
    }
}

bool GlobalSettings::loresPhoto()
{
    return mLoresPhoto;
}

void GlobalSettings::setDirectTextEmail(bool directEmail)
{
    if ( directEmail != mDirectEmail) {
        mDirectEmail = directEmail;
        emit directTextEmailChanged(mDirectEmail);

        // Store the value in QSettings to persist it between app runs.
        QSettings().setValue(DIRECTEMAIL_KEY, QVariant(mDirectEmail));
    }
}

bool GlobalSettings::directTextEmail()
{
    return mDirectEmail;
}

void GlobalSettings::setAutoEmailSignature(bool autoEmailSignature)
{
    if ( autoEmailSignature != mAutoEmailSignature) {
        mAutoEmailSignature = autoEmailSignature;
        emit autoEmailSignatureChanged(mAutoEmailSignature);

        // Store the value in QSettings to persist it between app runs.
        QSettings().setValue(AUTOEMAILSIGNATURE_KEY, QVariant(autoEmailSignature));
    }
}

bool GlobalSettings::autoEmailSignature()
{
    return mAutoEmailSignature;
}

void GlobalSettings::setEmailSignature(const QString emailSignature)
{
    if ( emailSignature != mEmailSignature) {
        mEmailSignature = emailSignature;
        emit emailSignatureChanged(mEmailSignature);

        // Store the value in QSettings to persist it between app runs.
        QSettings().setValue(EMAILSIGNATURE_KEY, QVariant(mEmailSignature));
    }
}

QString GlobalSettings::emailSignature()
{
    return mEmailSignature;
}

void GlobalSettings::invokeSystemSettings(const QString uri)
{
    InvokeRequest request;

    // Set up the Invocation request for an the email composer using
    // the incoming data.
    QString invokeTargetId = "sys.settings.target";
    QString invokeActionId = "bb.action.OPEN";
    QString mimeType = "settings/view";

    request.setAction(invokeActionId);
    request.setTarget(invokeTargetId);
    request.setMimeType(mimeType);
    request.setUri(uri);

    // Finally make the invocation which will result in the an email being set up.
    mInvokeManager->invoke(request);
}
