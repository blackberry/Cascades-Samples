/* Copyright (c) 2012 Research In Motion Limited.
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


#ifndef SPEEDWRITERAPP_H
#define SPEEDWRITERAPP_H

#include <bb/cascades/Application>
#include "wordchecker.h"
#include "speedgauge.h"

using namespace bb::cascades;

namespace bb
{
    namespace cascades
    {
        class TextArea;
    }
}

/**
 * SpeedWriterApp
 * 
 * This sample app focuses on how to handle text input. We show you how to 
 * attach an event listener to an input field and how to process the events.
 * Furthermore it is shown how you can set up a CustomControl and add it to the
 * UI.The app itself is a game, where the user is supposed to type text as quickly
 * as possible.
 */
class SpeedWriterApp: public QObject
{
Q_OBJECT

public:
    SpeedWriterApp();
    ~SpeedWriterApp();

private slots:
    void onTextChanging(const QString &newText);

private:

    // Word checker, monitor the input progress.
    WordChecker *mWordChecker;

    /**
     * Performs line feed, the input field and overlay text are cleared.
     * The text area holding the speed text is animated to show a new line.
     */
    void lineFeed();

    /**
     * Function that ends the game, called when the entire text
     * has been correctly typed.
     */
    void endGame();

    // Number of correct input lines.
    int mNbrOfLines;

    // Game state variable.
    bool mGameEnded;

    TextArea *mDisplayTextArea;
    TextArea *mOverlayTextArea;
    TextArea *mTextInput;

    // Our custom control
    SpeedGauge *mSpeedGauge;
};

#endif // ifndef SPEEDWRITERAPP_H

