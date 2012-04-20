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
#include <stdlib.h>
#include <string>

WordChecker::WordChecker()
{
    // Initializing variables.
    mSpeedString = SPEED_TEXT;
    mCursorPosition = 1;
    mLineStartPosition = 0;
    mSpeedTextLength = mSpeedString.length();
}

WordChecker::~WordChecker()
{
}

WordResult WordChecker::checkWord(const char *pCurrentWord)
{
    // Strings are easier to compare then (const char*)
    string currentString = pCurrentWord;
    string endOfLine = "\n";

    // Get the correct answer to which the input should be compared.
    string compareString = mSpeedString.substr(mLineStartPosition,
            mCursorPosition - mLineStartPosition);

    if (currentString.compare(compareString) == 0) {
        if (mCursorPosition >= mSpeedTextLength) {
            // When the entire text has been correctly entered return end.
            return WordResult_End;
        }

        // Check for end of line in the speed text.
        string checkLineEnd = mSpeedString.substr(mCursorPosition, 1);
        mCursorPosition++;

        if (checkLineEnd.compare(endOfLine) == 0) {
            // If at the end of a line, adjust the cursor for this and
            // update the staring position in order to get the correct substring
            // when starting on the next line.
            mLineStartPosition = mCursorPosition;
            mCursorPosition++;

            return WordResult_NewLine;
        }
        return WordResult_Correct;
    }
    return WordResult_Wrong;
}

int WordChecker::getCursorPosition()
{
    return mCursorPosition - 1;
}

string WordChecker::getCurrentString()
{
    // Get the sub string of the cursor position minus 1
    // to account for incrementing done in checkWord().
    return mSpeedString.substr(0, mCursorPosition - 1);
}
