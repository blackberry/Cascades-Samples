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
#include "speedwriterapp.h"
#include "speedgauge.h"

#include <bb/cascades/Container>
#include <bb/cascades/Page>
#include <bb/cascades/QmlDocument>
#include <bb/cascades/TextArea>
#include <math.h>

#include <sstream>

using namespace bb::cascades;

SpeedWriterApp::SpeedWriterApp()
{
    // qmlRegisterType registers our custom control with the QML parsing engine
    qmlRegisterType<SpeedGauge>("MyControl", 1, 0, "SpeedGauge");

    // Here we create a QMLDocument and load it, we are using build patterns.
    QmlDocument *qml = QmlDocument::create().load("speedwriter.qml");

    if (!qml->hasErrors()) {

        // The application Page is created from QML.
        Page *appPage = qml->createRootNode<Page>();

        if (appPage) {
            // Create the application scene.
            Application::setScene(appPage);

            // Find the four components that we need to have a handle to using objectNames defined in QML.
            mTextInput = appPage->findChild<TextArea*>("textInput");
            mDisplayTextArea = appPage->findChild<TextArea*>("textDisplay");
            mOverlayTextArea = appPage->findChild<TextArea*>("overlayTextDisplay");

            mSpeedGauge = appPage->findChild<SpeedGauge*>("speedGauge");
            mSpeedGauge->setPreferredSize(395, 397);

            // Connect to the textChanging signal (to update text).
            connect(mTextInput, SIGNAL(textChanging(const QString &)), this,
                    SLOT(onTextChanging(const QString &)));

            if (mDisplayTextArea != NULL) {
                mDisplayTextArea->setText(SPEED_TEXT);
            }

            // A word checker is used to validate if the text entry is correct.
            mWordChecker = new WordChecker(this);

            // Initialize game state variables.
            mGameEnded = false;
        }
    }
}

SpeedWriterApp::~SpeedWriterApp()
{
}

void SpeedWriterApp::onTextChanging(const QString &newText)
{
    // If the game is still ongoing and text in the input are is not empty, check the enter text.
    if (!mGameEnded && newText.compare("") != 0) {
        WordResult result = mWordChecker->checkWord(newText);

        switch (result) {
            case WordResult_Correct: {
                // If all is OK, set the overlay text to display progress.
                mOverlayTextArea->setText(mWordChecker->enteredLines() + newText);
            }
                break;
            case WordResult_NewLine: {
                // New line means, line feed should be executed.
                lineFeed();
            }
                break;
            case WordResult_End: {
                // Game complete, finish up.
                endGame();
            }
                break;
            default:
                break;
        }

        if (result == WordResult_Correct || result == WordResult_NewLine) {
            // As long as the entry is correct the speed is updated. Remove one to account for
            // the check cursor already being moved and account for new line characters '\n'.
            mSpeedGauge->calculateSpeed(mWordChecker->nbrOfCharacters());
        }
    }
}

void SpeedWriterApp::lineFeed()
{
    // Clear the input field.
    mTextInput->setText("");
    mTextInput->setHintText("");

    // Animate the text areas upwards to display the next line.
    mDisplayTextArea->setTranslationY((float) (-mWordChecker->line() * 60));
}

void SpeedWriterApp::endGame()
{
    // Stop measuring the progress, the returned value is the mean speed.
    qreal averageSpeed = (qreal)mSpeedGauge->stopMeasuring();

    // Convert the average speed result to a QString for presentation in the overlay.
    QString averageString;
    averageString.setNum((int)qRound(averageSpeed));

    //Set up an end game text for displaying the result in the overlay area.
    mOverlayTextArea->setText(QLatin1String("Your speed was ") +averageString + QLatin1String(" words/min.\nWell done!") );
     
    // Position the resulting text in the middle of the window and clear the other texts.
    mOverlayTextArea->setTranslationY((float) (0.0));
    mDisplayTextArea->setText((const char*) "");
    mTextInput->setText("");
    mTextInput->setEnabled(false);

    // End game for real.
    mGameEnded = true;
}
