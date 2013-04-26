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
#include "invocationrecipe.h"
#include "uivalues.h"

#include <bb/cascades/Button>
#include <bb/cascades/Container>
#include <bb/cascades/DropDown>
#include <bb/cascades/Invocation>
#include <bb/cascades/InvokeQuery>
#include <bb/cascades/Label>
#include <bb/cascades/Option>
#include <bb/cascades/TextArea>
#include <bb/cascades/TextField>

using namespace bb::cascades;

InvocationRecipe::InvocationRecipe(Container *parent) :
        CustomControl(parent)
{
    int standardPadding = UiValues::instance()->intValue(UiValues::UI_PADDING_STANDARD);
    Container *recipeContainer =
            Container::create().top(standardPadding).left(standardPadding).right(standardPadding).preferredSize(
                    UiValues::instance()->intValue(UiValues::SCREEN_WIDTH),
                    UiValues::instance()->intValue(UiValues::SCREEN_HEIGHT));

    // The first invocation is an unbound share. By not setting a invokeTargetId
    // a specific application will not be targeted instead the user will be presented
    // with a list of apps that can handle the MIME type and the share action.
    mUnboundQuery = InvokeQuery::create()
                        .mimeType("text/plain")
                        .invokeActionId("bb.action.SHARE");
    mUnboundShare = Invocation::create(mUnboundQuery).parent(this);

    // This is a bound invocation to the BBM share handler, notice that the invokeTargetId has been set here.
    mBBMQuery = InvokeQuery::create()
                        .mimeType("text/plain")
                        .invokeActionId("bb.action.SHARE")
                        .invokeTargetId("sys.bbm.sharehandler");
    mBBMShare = Invocation::create(mBBMQuery).parent(this);

    // A query for a pre-populated email, the URI follows the specification set up in documentation.
    mEmailQuery = InvokeQuery::create()
                    .mimeType("text/plain")
                    .invokeActionId("bb.action.SENDEMAIL")
                    .invokeTargetId("sys.pim.uib.email.hybridcomposer")
                    .uri("mailto:someemailaddress@cookbook.xyz?subject=sometext&body=someothertext");
    mEmailShare = Invocation::create(mEmailQuery).parent(this);

    // The Invocations are connected to the armed signal, so that they can be triggered
    // once they are ready to fire.
    connect(mUnboundShare, SIGNAL(armed()), this, SLOT(onInvocationArmed()));
    connect(mBBMShare, SIGNAL(armed()), this, SLOT(onInvocationArmed()));
    connect(mEmailShare, SIGNAL(armed()), this, SLOT(onInvocationArmed()));



    // A DropDown Control is used for selecting delivery method the Invocation is stored in the value for easy access.
    mInvokeSelection = DropDown::create().title("Delivery method");
    mInvokeSelection->add(Option::create().text("Don't know").value(QVariant::fromValue(mUnboundShare))
                            .selected(true));
    mInvokeSelection->add(Option::create().text("BBM").value(QVariant::fromValue(mBBMShare)));
    mInvokeSelection->add(Option::create().text("Email").value(QVariant::fromValue(mEmailShare)));

    connect(mInvokeSelection, SIGNAL(selectedValueChanged(const QVariant&)), this,
            SLOT(onInvokeSelectionChanged(const QVariant&)));


    // A title is added so the user know what is shown.
    mTitle = Label::create().text("Shopping List");

    // The email address field is initially hidden, only shown when the Email option is selected in the DropDown.
    mAdress = TextField::create()
                    .text("someemailaddress@cookbook.xyz")
                    .hintText("Email address")
                    .inputMode(TextFieldInputMode::EmailAddress).visible(false);

    // The list of things to buy.
    mMessage = TextArea::create()
                .hintText("Enter items to buy")
                .text("Chocolate\nFudge\nTurkish Delight");

    // The button that will initiate the invocation selected in the DropDown.
    mInvokeButton = Button::create().text("Invoke!");
    connect(mInvokeButton, SIGNAL(clicked()), this, SLOT(onClicked()));

    // Add UI Components to the recipe Container.
    recipeContainer->add(mInvokeSelection);
    recipeContainer->add(mTitle);
    recipeContainer->add(mAdress);
    recipeContainer->add(mMessage);
    recipeContainer->add(mInvokeButton);

    // and set the CustomControl root.
    setRoot(recipeContainer);
}

InvocationRecipe::~InvocationRecipe()
{
}

void InvocationRecipe::onInvokeSelectionChanged(const QVariant& selectedValue)
{
    if (selectedValue.canConvert<Invocation*>()) {
        Invocation *invocation = selectedValue.value<Invocation*>();

        // The address field is only shown if the email option is selected.
        if (invocation == mEmailShare) {
            mAdress->setVisible(true);
        } else {
            mAdress->setVisible(false);
        }
    }
}

void InvocationRecipe::onClicked()
{
    // As the button is clicked the invoke query is changed this takes
    // a little bit of time and its not until the Invocation is armed again
    // it is possible to actually trigger the invocation. So the query is updated
    // and the button is temporarily disabled.
    QVariant optionValue = mInvokeSelection->selectedValue();

    if (optionValue.canConvert<Invocation*>()) {
        Invocation *invocation = optionValue.value<Invocation*>();
        if (invocation == mEmailShare) {
            // For an email the URI is used to provide the information.
            mEmailQuery->setUri("mailto:" + mAdress->text() + "?subject=" + mTitle->text() + "&body="+ mMessage->text());

            // Updating the query which will cause the Invocation to re-arm itself and not until
            // then it can be triggered with the updated query.
            mEmailQuery->updateQuery();

        } else if (invocation == mUnboundShare) {
            QString message = mTitle->text() + "\n" + mMessage->text();

            // The message is converted to a QByteArray and the data of the query is set.
            QByteArray queryData = message.toUtf8();
            mUnboundQuery->setData(queryData);

            mUnboundQuery->updateQuery();

        } else if (invocation == mBBMShare) {
            QString message = mTitle->text() + "\n" + mMessage->text();
            QByteArray queryData = message.toUtf8();
            mBBMQuery->setData(queryData);
            mBBMQuery->updateQuery();
        }
    }

    mInvokeButton->setEnabled(false);
}

void InvocationRecipe::onInvocationArmed()
{
    launchPendingInvokation();
}

void InvocationRecipe::launchPendingInvokation()
{
    if (!mInvokeButton->isEnabled()) {
        // The button is disable when its pressed, so we only end up here
        // if the selected options Invocation«s query has been been updated and
        // it is armed.
        QVariant optionValue = mInvokeSelection->selectedValue();

        if (optionValue.canConvert<Invocation*>()) {
            // Trigger the Invocation.
            Invocation *invocation = optionValue.value<Invocation*>();
            invocation->trigger(invocation->query()->invokeActionId());
        }
    }
    mInvokeButton->setEnabled(true);
}

