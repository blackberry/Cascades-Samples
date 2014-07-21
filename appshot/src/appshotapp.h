/* Copyright (c) 2013, 2014 BlackBerry Limited.
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
#ifndef ApplicationUI_HPP_
#define ApplicationUI_HPP_

#include <QObject>

namespace bb
{
    namespace cascades
    {
        class Application;
    }
}

/*!
 * AppShotApp Description:
 *
 * Sometimes you might want to be able to take a programmatic
 * screen shot of you Cascades application. It could for
 * example be that the application creates a screen that is
 * suited for sharing on social network or something like that.
 * This sample shows how this can be achieved.
 *
 * You will learn how to:
 * - Take a snap shot of the current screen and open it in the pictures card
 */

class AppShotApp: public QObject
{
    Q_OBJECT

    /**
     * The show instruction property, that determines if an overlay with instructions is shown.
     * Only true the first time the application launches. The value is stored in QSettings to
     * persist between application launches.
     */
    Q_PROPERTY(bool showInstruction READ showInstruction WRITE setShowInstruction NOTIFY showInstructionChanged)

public:
    AppShotApp(QObject *parent = 0);
    virtual ~AppShotApp()
    {
    }

    Q_INVOKABLE void showPhotoInCard(const QString fileName);

    /**
     * Sets the show instruction property.
     *
     * @param showInstruction A boolean value for controlling if an instruction
     * 		  overlay should be shown on application launch.
     */
    Q_SLOT void setShowInstruction(bool showInstruction);

    /**
     * Returns the value of the showInstruction property, only true the first
     * time the app launches.
     *
     * @return The current value of the showInstruction property
     */
    bool showInstruction();

    /**
     * Signal that is emitted when the value of the showInstruction property
     * changes.
     */
    Q_SIGNAL void showInstructionChanged(bool showInstruction);

private:
    // Member variable for storing the showInstruction property.
    bool mShowInstruction;

	/**
	 * The key where the showInstruction property is stored in the QSettings object.
	 */
    static const QString SHOW_INSTRUCTIONS_KEY;
};

#endif /* ApplicationUI_HPP_ */
