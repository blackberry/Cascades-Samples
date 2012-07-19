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

#include <QObject>

// The text the space and end of line \n
#define SPEED_TEXT "Mary had a little lamb, its fleece \nwas white as snow. Sea shells, \nsea shells, by the sea shore. The \nflirtatious flamingo relentlessly \nargued with the aerodynamic \nalbatross. Admire the \nmiscellaneous velociraptors \nbasking in the sun. Egotistic \naardvarks enthusiastically \neating lollipops. \"Precisely\", \npronounced the presidential \nparrot presiding over the \npurple pachyderms."

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
class WordChecker: public QObject
{
    Q_OBJECT

    /**
     *  Property that holds the entire speed text that the player is supposed to type.
     */
    Q_PROPERTY(QString speedText READ speedText WRITE setSpeedText NOTIFY speedTextChanged)

    /**
     *  Property that reflects the number of correctly entered lines thus far.
     */
    Q_PROPERTY(int line READ line NOTIFY lineChanged)

    /**
     * Property that holds the number of correctly entered characters thus far.
     */
    Q_PROPERTY(int nbrOfCharacters READ nbrOfCharacters NOTIFY nbrOfCharactersChanged)

    /**
     * The text on all entered lines entered before the current line.
     */
    Q_PROPERTY(QString enteredLines READ enteredLines NOTIFY enteredLinesChanged)

public:
    WordChecker(QObject *parent = 0);
    ~WordChecker();

    /**
     * Function that sets the speedText property.
     *
     * @param speedText the text that the checker will compare against including end of line characters.
     */
    void setSpeedText(QString speedText);

    /**
     * To get the entire speed text including end of line characters
     * call this function.
     *
     * @return the current text.
     */
    QString speedText();

    /**
     * Return the number of lines correctly entered.
     */
    int line();

    /**
     * Returns the number of correctly entered characters.
     */
    int nbrOfCharacters();

    /**
     * Returns a string of all the text that has been entered prior to the current line.
     */
    QString enteredLines();

    /**
     * This function checks the current input towards the actual
     * text that should be written (hence forth called the speed text).
     *
     * @param the current word entered into the text input field.
     * return the result of checking the text.
     */
    WordResult checkWord(const QString currentWord);

signals:
    /**
     * Emitted when a new speed text has been set.
     */
    void speedTextChanged(QString speedText);

    /**
     * Emitted when a line break as reached, the parameter line is the number of lines.
     */
    void lineChanged(int line);

    /**
     * Emitted when the correctly entered lines changes (the parameter contains only full lines).
     */
    void enteredLinesChanged(QString enteredLines);

    /**
     * Emitted the number of correctly entered characters have changed.
     */
    void nbrOfCharactersChanged(int nbrOfCharacters);

private:

    // State variables
    int mSpeedTextLength;

    // Property variables.
    QString mSpeedText;
    QString mEnteredLines;
    int mLine;
    int mNbrOfCharacters;
};

#endif //  _WORDCHECKER_H_
