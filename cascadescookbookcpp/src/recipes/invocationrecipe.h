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
#ifndef INVOCATIONRECIPE_H_
#define INVOCATIONRECIPE_H_

#include <bb/cascades/CustomControl>

using namespace bb::cascades;

namespace bb
{
    namespace cascades
    {
        class Button;
        class DropDown;
        class Invocation;
        class InvokeQuery;
        class Label;
        class Option;
        class TextField;
        class TextArea;
    }
}

/* InvocationRecipe Description:
 *
 * Illustrating the usage of Invocation.
 */
class InvocationRecipe: public bb::cascades::CustomControl
{
    Q_OBJECT
public:
    InvocationRecipe(Container *parent = 0);
    virtual ~InvocationRecipe();

private:

    /**
     * This function is connected to a DropDown selectedValue changed signal,
     * in this recipe this is used to show and hide an email address field.
     *
     * @param selectedValue The value of the newly selected option, or an invalid @c QVariant if none is selected.
     */
    Q_SLOT void onInvokeSelectionChanged(const QVariant& selectedValue);

    /**
     * A function that is connected to a button that initiates the invocation
     * selected in a DropDown menu.
     */
    Q_SLOT void onClicked();

    /**
     * All the Invocation armed signals are connected to this function, it
     * only calls the launchPendingInvokation function.
     */
    Q_SLOT void onInvocationArmed();

    /**
     * Helper function for triggering an invocation, only happens if the Button has
     * just been pressed and is in an disabled state.
     */
    void launchPendingInvokation();

    // UI components
    DropDown *mInvokeSelection;
    Label *mTitle;
    TextField *mAdress;
    TextArea *mMessage;
    Button *mInvokeButton;

    // We show three Invocations in this recipe
    Invocation *mUnboundShare;
    Invocation *mBBMShare;
    Invocation *mEmailShare;

    // The Invocations has query«s associated with them.
    InvokeQuery *mUnboundQuery;
    InvokeQuery *mBBMQuery;
    InvokeQuery *mEmailQuery;
};

#endif /* INVOCATIONRECIPE_H_ */
