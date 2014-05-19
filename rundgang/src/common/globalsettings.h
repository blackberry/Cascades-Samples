/* Copyright (c) 2013 BlackBerry Limited.
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
#ifndef GLOBALSETTINGS_H_
#define GLOBALSETTINGS_H_

#include <QObject>
#include <bb/cascades/VisualStyle>

namespace bb
{
    namespace system
    {
        class InvokeManager;
    }
}

/**
 * GlobalSettings Description
 *
 * This class handles application wide settings that persist between runs.
 * It also has a function for invoking the Device Settings application.
 */
class GlobalSettings: public QObject
{
    Q_OBJECT

    /**
     * The autoEmailSignature controls if a signature should be added to each email.
     */
    Q_PROPERTY(bool autoEmailSignature READ autoEmailSignature WRITE setAutoEmailSignature NOTIFY autoEmailSignatureChanged FINAL)

    /**
     * The directTextEmail decides if a text data feedback should be sent immediately
     * when the action is triggered or if another way of first adding data should be used.
     * If true it is possible to save the actual text enter in the history as well.
     */
    Q_PROPERTY(bool directTextEmail READ directTextEmail WRITE setDirectTextEmail NOTIFY directTextEmailChanged FINAL)

    /**
     * The email signature that is added to each email if the autoEmailSignature is set to true.
     */
    Q_PROPERTY(QString emailSignature READ emailSignature WRITE setEmailSignature NOTIFY emailSignatureChanged FINAL)

    /**
     * The application takes photo as one of the feedback data options, this property
     * is used to check if the photo should be scaled down before it is sent.
     */
    Q_PROPERTY(bool loresPhoto READ loresPhoto WRITE setLoresPhoto NOTIFY loresPhotoChanged FINAL)

    /**
     * The application theme, defaults to the standard of the device that the app is running on
     * But the user can change it from the settings menu.
     */
    Q_PROPERTY(bb::cascades::VisualStyle::Type visualStyle READ visualStyle WRITE setVisualStyle NOTIFY visualStyleChanged FINAL)

public:
    GlobalSettings(QObject *parent = 0);
    virtual ~GlobalSettings();

    /**
     * Sets the autoEmailSignature property.
     *
     * @param autoEmailSignature If true an email signature should be added to emails that is set up by the app.
     */
    Q_SLOT void setAutoEmailSignature(bool autoEmailSignature);

    /**
     * Returns the auto email signature.
     *
     * The autoEmailSignature controls if a signature should be added to each email.
     *
     * @return True if a signature should be added otherwise false.
     */
    bool autoEmailSignature();

    /**
     * Sets the directEmail, that decides if an email card is to be triggered
     * directly or not.
     *
     * @param directEmail If True an email card should be triggered directly
     */
    Q_SLOT void setDirectTextEmail(bool directTextEmail);

    /**
     * Return the directTextEmail that decides if a text data feedback should be sent immediately
     * when the action is triggered or if another way of first adding data should be used.
     *
     * @return True if the email card should be launched directly otherwise false.
     */
    bool directTextEmail();

    /**
     * Sets the email signature.
     *
     * @param emailSignature A QString with the email signature.
     */
    Q_SLOT void setEmailSignature(const QString emailSignature);

    /**
     * Returns the email signature.
     *
     * The text that is added to each email if autoEmailSignature is true.
     *
     * @return The string that holds the signature
     */
    QString emailSignature();

    /**
     * Sets whether using the Camera should result in a low resolution
     * Image being stored or not.
     *
     * @param loresPhoto If true the image should be saved in low resolution.
     */
    Q_SLOT void setLoresPhoto(bool loresPhoto);

    /**
     * Indicates if a photo taken by a Camera in the app should be stored in
     * low resolution or not.
     *
     * @return True if images is to be stored in low resolution.
     */
    bool loresPhoto();

    /**
     * Sets the visual style of the application a.k.a. theme, can be bright or dark.
     *
     * @param visualStyle The style that should be used by the entire applicaiton.
     */
    Q_SLOT void setVisualStyle(bb::cascades::VisualStyle::Type visualStyle);

    /**
     * The visual style of the application.
     *
     * @return Currently set visual style (bright or dark), defaults to the device default.
     */
    bb::cascades::VisualStyle::Type visualStyle();

    /**
     * Invokable function that open the device settings application with the
     * view given by the URI
     *
     *  @param uri The view that will be opened in the Settings app.
     */
    Q_INVOKABLE void invokeSystemSettings(const QString uri);
signals:

    /**
     * Emitted when autoEmailSignature change
     *
     * @param The new value of autoEmailSignature
     */
    void autoEmailSignatureChanged( bool autoEmailSignature );

    /**
     * Emitted when directTextEmail change.
     *
     * @param The new value of autoEmailSignature
     */
    void directTextEmailChanged(bool directTextEmail);

    /**
     * Emitted when the emailSignature change.
     *
     * @param The new email signature string.
     */
    void emailSignatureChanged(QString emailSignature);

    /**
     * Emitted when the loresPhoto change.
     *
     * @param The new value of loresPhoto.
     */
    void loresPhotoChanged(bool loresPhoto);

    /**
     * Emitted when the visualStyle property changes.
     *
     * @param The new value of the visualStyle.
     */
    void visualStyleChanged(bb::cascades::VisualStyle::Type visualStyle);



private:
    // Constant strings.
    static const QString APP_NAME;
    static const QString APP_ORGANIZATION;
    static const QString LORESHOTO_KEY;
    static const QString AUTOEMAILSIGNATURE_KEY;
    static const QString DIRECTEMAIL_KEY;
    static const QString EMAILSIGNATURE_KEY;
    static const QString VISUALSTYLE_KEY;

    // Boolean property variables
    bool mAutoEmailSignature;
    bool mDirectEmail;
    bool mLoresPhoto;

    // The email signature string variable representing the emailSignature property.
    QString mEmailSignature;

    // The visual style of the application.
    bb::cascades::VisualStyle::Type mVisualStyle;

    // An invocation manager used to invoke application cards.
    bb::system::InvokeManager *mInvokeManager;
};

#endif /* GLOBALSETTINGS_H_ */
