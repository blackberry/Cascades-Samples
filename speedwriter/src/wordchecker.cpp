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


#include "wordchecker.h"
#include <QDebug>

WordChecker::WordChecker(QObject *parent): QObject(parent)
{
    // Initializing variables.
    mSpeedText = QString(SPEED_TEXT);
    mNbrOfCharacters = 0;
    mSpeedTextLength = mSpeedText.length();
    mEnteredLines = "";
    mLine = 0;
}

WordChecker::~WordChecker()
{
}

WordResult WordChecker::checkWord(const QString currentLine)
{
    QString compareLine = mSpeedText.section('\n', mLine, mLine);
    QString compareString = compareLine.left(currentLine.length());

    if (currentLine.compare(compareString) == 0) {

        // Update the number of correct characters entered.
        mNbrOfCharacters = mEnteredLines.length() + currentLine.length();
        emit nbrOfCharactersChanged(mNbrOfCharacters);

        if (mNbrOfCharacters >= mSpeedTextLength) {
            // When the entire text has been correctly entered return end.
            return WordResult_End;
        }

        if (compareLine.length() == currentLine.length()) {
            // If at the end of a line update the line and entereLines
            // property with the number of lines and the text entered so far.
            mLine++;
            emit lineChanged(mLine);

            mEnteredLines = mSpeedText.section('\n', 0, mLine - 1) + "\n";
            emit enteredLinesChanged(mEnteredLines);

            return WordResult_NewLine;
        }
        return WordResult_Correct;
    }

    return WordResult_Wrong;
}

void WordChecker::setSpeedText(QString speedText)
{
    if(mSpeedText.compare(speedText) != 0) {
        mSpeedText = speedText;
        emit speedTextChanged(mSpeedText);
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

QString WordChecker::enteredLines()
{
    return mEnteredLines;
}

int WordChecker::nbrOfCharacters()
{
    return mNbrOfCharacters;
}
