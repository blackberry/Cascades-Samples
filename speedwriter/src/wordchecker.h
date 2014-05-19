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
#ifndef _WORDCHECKER_H_
#define _WORDCHECKER_H_

#include <QObject>

/**
 * WordChecker Description:
 *
 * This class is to evaluate if the string being sent
 * to it corresponds to a given string in the text to be typed.
 * In other words, the word (string) passed is valid or invalid
 * relative to what should be typed.
 *
 */
class WordChecker: public QObject
{
    Q_OBJECT

    /**
     *  Property that holds the entire speed text that the player is supposed to type
     */
    Q_PROPERTY(QString speedText READ speedText WRITE setSpeedText NOTIFY speedTextChanged)

    /**
     *  Property that reflects the number of correctly entered lines so far by the user
     */
    Q_PROPERTY(int line READ line NOTIFY lineChanged)

    /**
     * Property that holds the number of correctly inputted characters so far by the user
     */
    Q_PROPERTY(int nbrOfCharacters READ nbrOfCharacters NOTIFY nbrOfCharactersChanged)

    /**
     * Property that holds the text of all entered lines entered up to the current line
     */
    Q_PROPERTY(QString enteredLines READ enteredLines NOTIFY enteredLinesChanged)

    /**
     * The part of current line that has been entered correctly.
     */
    Q_PROPERTY(QString currentCorrectLine READ currentCorrectLine NOTIFY currentCorrectLineChanged)

    /**
     * The part of the text that is still to be written.
     */
    Q_PROPERTY(QString remainingText READ remainingText NOTIFY remainingTextChanged)

    /**
     * The valid property is true as long as no error in typing has been done.
     */
    Q_PROPERTY(bool valid READ valid NOTIFY validChanged)

public:
    /**
     * This is our constructor which initializes the member variables.
     *
     * @param parent The parent Container, if not specified, 0 is used.
     */
    WordChecker(QObject *parent = 0);
    ~WordChecker();

    /**
     * This function sets the speedText property.
     *
     * @param speedText The text that the checker will compare against including end of line characters.
     */
    void setSpeedText(QString speedText);

    /**
     * This function return the entire speed text including end of line characters as a QString.
     *
     * @return The current speed text
     */
    QString speedText();

    /**
     * This function returns the number of lines correctly entered so far.
     *
     * @return The number of lines correctly entered so far
     */
    int line();

    /**
     * This function returns the number of correctly entered characters so far.
     *
     * @return The number of correctly entered characters so far
     */
    int nbrOfCharacters();

    /**
     * This function returns a QString of all the text that has been entered prior to the current line.
     *
     * @return A QString of all entered text before the current line
     */
    QString enteredLines();

    /**
     * This function returns a string of all the correctly entered text for the current line.
     */
    QString currentCorrectLine();

    /**
     * This function returns a QString of all the text that is not jet written.
     *
     * @return A QString of all text that is left to enter.
     */
    QString remainingText();

    /**
     * Returns the valid state of the word checker.
     *
     * @return True if all input so far has been correct otherwise false;
     */
    bool valid();

    /**
     * This function checks the current input towards the actual
     * text that should be written (hence forth called the speed text).
     *
     * @param currentWord The current word entered into the text input field.
     */
    Q_INVOKABLE void checkWord(const QString currentWord);

signals:
    /**
     * This signal is emitted when a new speed text has been set
     */
    void speedTextChanged(QString speedText);

    /**
     * This signal is emitted when a line break as reached, the parameter, line, is the number of lines
     */
    void lineChanged(int line);

    /**
     * This signal is emitted when progress has been made on correctly entering text on the current line.
     */
    void currentCorrectLineChanged(QString currentCorrectLine);

    /**
     * This signal is emitted when the correctly entered lines changes (the parameter contains only full lines).
     */
    void enteredLinesChanged(QString enteredLines);

    /**
     * This signal is emitted when the text that is left to enter changes.
     */
    void remainingTextChanged(QString remainingText);

    /**
     * This signal is emitted when the number of correctly entered characters have changed.
     */
    void nbrOfCharactersChanged(int nbrOfCharacters);

    /**
     * This signal is emitted when all text has been entered correctly.
     */
    void ended();

    /**
     * This signal is emitted the validity of the input has changed.
     */
    void validChanged(bool valid);

private:

    void setValid(bool valid);

    // State variables
    int mSpeedTextLength;
    int mRemainingTextLength;

    // Property variables
    QString mSpeedText;
    QString mEnteredLines;
    QString mCurrentCorrectLine;
    QString mRemainingText;

    bool mValid;
    int mLine;
    int mNbrOfCharacters;
};

#endif //  _WORDCHECKER_H_
