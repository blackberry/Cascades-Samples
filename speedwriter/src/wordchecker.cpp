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

#include "wordchecker.h"
#include <QDebug>

WordChecker::WordChecker(QObject *parent) :
        QObject(parent), mSpeedTextLength(0), mRemainingTextLength(0), mValid(true)
{
    // Initialize instance variables.
    mNbrOfCharacters = 0;
    mEnteredLines = "";
    mLine = 0;
}

WordChecker::~WordChecker()
{
}

void WordChecker::checkWord(const QString currentLine)
{
    QString compareLine = mSpeedText.section('\n', mLine, mLine);
    QString compareString = compareLine.left(currentLine.length());

    if (currentLine.compare(compareString) == 0) {

        // Update the number of correct characters entered.
        mNbrOfCharacters = mEnteredLines.length() + currentLine.length();
        emit nbrOfCharactersChanged(mNbrOfCharacters);

        // Update the number of characters that is left to enter.
        mRemainingTextLength = mSpeedTextLength - mNbrOfCharacters;

        // Updates the QString holding the text that is not yet typed.
        mRemainingText = mSpeedText.right(mRemainingTextLength);
        emit remainingTextChanged(mRemainingText);

        if (compareLine.length() == currentLine.length()) {
            if (mNbrOfCharacters >= mSpeedTextLength) {
                // When the entire text has been correctly entered return end.
                emit ended();
            } else {
                // If at the end of a line, update the line and enteredLines
                // property with the number of lines and the text entered so far
                mLine++;

                // Update the entered lines.
                mEnteredLines = mSpeedText.section('\n', 0, mLine - 1) + "\n";
                emit enteredLinesChanged(mEnteredLines);

                // lineChanged signal needs to be emitted after enteredLinesChanged. If not, the
                // speedtext will be updated with the just entered line instead of a new one.
                emit lineChanged(mLine);

                // Start a new line by setting the current correct line to an empty string.
                mCurrentCorrectLine = "";
                emit currentCorrectLineChanged(mCurrentCorrectLine);
            }

        } else {
            mCurrentCorrectLine = currentLine;
            emit currentCorrectLineChanged(mCurrentCorrectLine);
        }
        setValid(true);
    } else {
        setValid(false);
    }
}

void WordChecker::setSpeedText(QString speedText)
{
    if (mSpeedText.compare(speedText) != 0) {
        mSpeedText = speedText;
        mSpeedTextLength = mSpeedText.length();
        emit speedTextChanged(mSpeedText);

        // At startup we need to set a valid text for mRemainingText to be
        // able to show the initial text to write. At start up mRemaining is
        // equal with the string we use for the text since nothing has been written yet.
        mRemainingText = speedText;
        emit remainingTextChanged(mRemainingText);
    }
}

QString WordChecker::speedText()
{
    return mSpeedText;
}

int WordChecker::line()
{
    return mLine;
}

QString WordChecker::currentCorrectLine()
{
    return mCurrentCorrectLine;
}

QString WordChecker::enteredLines()
{
    return mEnteredLines;
}

QString WordChecker::remainingText()
{
    return mRemainingText;
}

int WordChecker::nbrOfCharacters()
{
    return mNbrOfCharacters;
}

void WordChecker::setValid(bool valid)
{
    if (mValid != valid) {
        mValid = valid;
        emit validChanged(mValid);
    }
}

bool WordChecker::valid()
{
    return mValid;
}

