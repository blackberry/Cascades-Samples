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
#ifndef _WORDCHECKER_H_
#define _WORDCHECKER_H_

#include <string>
using namespace std;

// The text the space and end of line \n
#define SPEED_TEXT "Mary had a little lamb, its fleece \nwas white as snow. Sea shells, \nsea shells, by the sea shore. The \nflirtatious flamingo relentlessly \nargued with the aerodynamic \nalbatross. Admire the \nmiscellaneous velociraptors \nbasking in the sun. Egotistic \naardvarks enthusiastically \neating lollipops. \"Precisely\", \npronounced the presidential \nparrot presiding over the \n\purple pachyderms."

// Evaluation of the word check may result in one of the following.
enum WordResult
{
    WordResult_Correct, WordResult_Wrong, WordResult_NewLine, WordResult_End
};

/**
 * WordChecker
 *
 * This class purpose is to evaluate if the string being sent
 * to it corresponds to a given
 *
 */
class WordChecker
{
public:
    WordChecker();
    ~WordChecker();

    /**
     * This function checks the current input towards the actual
     * text that should be written (hence forth called the speed text).
     *
     * @param the current word entered into the text input field.
     * return the result of checking the text.
     */
    WordResult checkWord(const char *pCurrentWord);

    /**
     * The number of correctly input characters can be accessed
     * via this function.
     *
     * return the current position in the text plus end of line characters
     */
    int getCursorPosition();

    /**
     * To get the entire speed text including end of line characters
     * call this function.
     *
     * @return the current text.
     */
    string getCurrentString();

private:

    // State variables
    int mCursorPosition;
    int mLineStartPosition;
    int mSpeedTextLength;
    string mSpeedString;
};

#endif //  _WORDCHECKER_H_
