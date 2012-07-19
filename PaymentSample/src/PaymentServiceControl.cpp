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

#include "PaymentServiceControl.hpp"
#include <bb/platform/PaymentService>
#include <bb/platform/PaymentServiceResponse>
#include <bb/platform/PurchaseReceipt>
#include <QSharedPointer>
#include <QString>
#include <QDateTime>
#include <QList>

#include <iostream>
#include <stdlib.h>

using namespace bb::platform;
using namespace bb::cascades;

PaymentServiceControl::PaymentServiceControl() :
    bb::cascades::CustomControl(NULL)
{
    setRoot(Container::create());
}

PaymentServiceControl::PaymentServiceControl(Container *parent) :
    bb::cascades::CustomControl(parent)
{
    setRoot(Container::create());
}

PaymentServiceControl::~PaymentServiceControl() {
}

/**
 * Request the purchase from the payment service based on the item's id, sku, name and metadata.
 */
void PaymentServiceControl::purchase(const QString &id, const QString &sku, const QString &name, const QString &metadata)
{
    std::cout << "\nRequesting purchase. ID: " << id.toStdString() <<
            " SKU: " << sku.toStdString() << " Name: " << name.toStdString() <<
            " Metadata: " << metadata.toStdString() << std::endl;

    PaymentService::getInstance().requestPurchase(id, sku, name, metadata,
        this, SLOT(purchaseResponse(bb::platform::PurchaseResponseInfo)));
}

/**
 * Format the receipt into a user readable string.
 */
static QString receiptToString(bb::platform::PurchaseReceipt r)
{
    char initPeriod[3];
    QString initialPeriod(itoa(r.initialPeriod(), initPeriod, 10));
    QDateTime startDate = r.startDate();
    QString startDateStr = startDate.isNull() ? "N/A" : startDate.toString();
    QDateTime endDate = r.endDate();
    QString endDateStr = endDate.isNull() ? "N/A" : endDate.toString();
    QString isSubscr = r.isSubscription() ? "true":"false";

    char itemStateBuf[3];
    int itemState = (int)r.state();
    QString itemStateStr(itoa(itemState, itemStateBuf, 10));

    QString displayString = "Date: " + r.date().toString() +
        "\nID/SKU: " + r.digitalGoodId() + "/" + r.digitalGoodSku() +
        "\nPurchaseID/licenseKey: " + r.purchaseId() + "/" + r.licenseKey() +
        "\nMetadata: " + r.purchaseMetadata() +
        "\nItemState/isSubscription?: " + itemStateStr + "/" + isSubscr +
        "\nStart/End: " + startDateStr + "/" + endDateStr +
        "\nInitialPeriod: " + initialPeriod + "\n";

    return displayString;
}

/**
 * Invoked in response to the purchase signal. It differentiates between successful and
 * error responses and emits appropriate signal for each.
 */
void PaymentServiceControl::purchaseResponse(bb::platform::PurchaseResponseInfo response)
{
	//emits error signal upon errors.
    if (response.error()) {
        std::cout << "Purchase response error. Code(" << response.errorCode() << ") Text(" << response.errorText().toStdString() << ")" << std::endl;
        emit infoResponseError(response.errorCode(), response.errorText());

    //emits success signal upon success
    } else {
        QSharedPointer<PurchaseReceipt> r = response.receipt();
        QString displayString = receiptToString(*(r.data()));
        std::cout << "Purchase response success. " << displayString.toStdString() << std::endl;

        emit purchaseResponseSuccess(displayString);
    }
}

/**
 * Request existing purchases from the payment service.
 */
void PaymentServiceControl::getExisting(bool refresh)
{
    std::cout << "Get existing. refresh: " << refresh << std::endl;

    PaymentService::getInstance().requestExistingPurchases(refresh,
        this, SLOT(existingPurchasesResponse(bb::platform::ExistingPurchasesResponseInfo)));
}

/**
 * Invoked in response to retrieve existing purchases made and emit appropriate signal based
 * on the response data.
 */
void PaymentServiceControl::existingPurchasesResponse(bb::platform::ExistingPurchasesResponseInfo response)
{
    if (response.error()) {
        std::cout << "Existing purchases response error. Code(" << response.errorCode() << ") Text(" << response.errorText().toStdString() << ")" << std::endl;
        emit infoResponseError(response.errorCode(), response.errorText());
    } else {
        std::cout << "Existing purchases response success. (TODO)" << std::endl;
        const QList<PurchaseReceipt> receipts = response.purchases();

        if (receipts.isEmpty()) {
            std::cout << "Existing purchases response success. (No purchases)" << std::endl;
            emit existingPurchasesResponseSuccess("(No purchases)");
            return;
        }

        //For each purchase format a user readable string representation of the receipt.
        QString displayString;
        Q_FOREACH(PurchaseReceipt r, receipts) {
            displayString += (receiptToString(r) + "\n");
        }
        emit existingPurchasesResponseSuccess(displayString);
    }
}

/**
 * Query the payment service for an item's price based on its ID and SKU.
 */
void PaymentServiceControl::getPrice(const QString &id, const QString &sku)
{
    std::cout << "Requesting price. ID: " << id.toStdString() << " SKU: " << sku.toStdString() << std::endl;

    //Make the price request and indicate what method to invoke on signal response.
    PaymentService::getInstance().requestPrice(id, sku,
        this, SLOT(priceResponse(bb::platform::PriceResponseInfo)));
}

/**
 * Invoked in response to price request for an item. Emit appropriate signal based on response data.
 */
void PaymentServiceControl::priceResponse(bb::platform::PriceResponseInfo response)
{
    if (response.error()) {
        std::cout << "Price response error. Code(" << response.errorCode() << ") Text(" << response.errorText().toStdString() << ")" << std::endl;
        emit infoResponseError(response.errorCode(), response.errorText());
    } else {
        std::cout << "Price response success. Price: " << response.price().toStdString() << std::endl;
        //Emit success response if no errors occurred.
        emit priceResponseSuccess(response.price());
    }
}

/**
 * Query the payment service for itmes subscription terms based on its ID, and SKU.
 */
void PaymentServiceControl::getSubscriptionTerms(const QString &id, const QString &sku)
{
    std::cout << "Requesting subscription terms. ID: " << id.toStdString() << " SKU: " << sku.toStdString() << std::endl;

    PaymentService::getInstance().requestSubscriptionTerms(id, sku,
            this, SLOT(subscriptionTermsResponse(bb::platform::SubscriptionTermsResponseInfo)));
}

/**
 * Invoked based on items subscription terms request.
 */
void PaymentServiceControl::subscriptionTermsResponse(bb::platform::SubscriptionTermsResponseInfo response)
{
    if (response.error()) {
        std::cout << "Sub terms response error. Code(" << response.errorCode() << ") Text(" << response.errorText().toStdString() << ")" <<std::endl;
        emit infoResponseError(response.errorCode(), response.errorText());
    } else {
        std::cout << "Sub terms response success. Price: " << response.price().toStdString() <<
            "\nInitialPeriod: " << response.initialPeriod().toStdString() <<
            "\nRenewalPrice: " << response.renewalPrice().toStdString() <<
            "\nRenewalPeriod: " << response.renewalPeriod().toStdString() << std::endl;
        //emit success response signal when query generated no errors.
        emit subscriptionTermsResponseSuccess(response.price(), response.initialPeriod(), response.renewalPrice(), response.renewalPeriod());
    }
}

/**
 * Query the payment service for an item's subscription status based on its ID and SKU.
 */
void PaymentServiceControl::checkSubscriptionStatus(const QString &id, const QString &sku)
{
    std::cout << "Check subscription status. ID: " << id.toStdString() << " SKU: " << sku.toStdString() << std::endl;

    PaymentService::getInstance().checkSubscriptionStatus(id, sku,
        this, SLOT(checkStatusResponse(bb::platform::CheckStatusResponseInfo)));
}

/**
 * Invoked upon response from the subscription status query.
 */
void PaymentServiceControl::checkStatusResponse(bb::platform::CheckStatusResponseInfo response)
{
    if (response.error()) {
        std::cout << "Check status response error. Code(" << response.errorCode() << ") Text(" << response.errorText().toStdString() << ")" <<std::endl;
        emit infoResponseError(response.errorCode(), response.errorText());
    } else {
        std::cout << "Check status response success. Active? " << response.active() << std::endl;
        emit checkStatusResponseSuccess(response.active());
    }
}

/**
 * Cancel item's subscription based on the purchase ID of that item.
 */
void PaymentServiceControl::cancelSubscription(const QString &purchaseId)
{
    std::cout << "Cancel subscription. Purchase ID: " << purchaseId.toStdString() << std::endl;

    PaymentService::getInstance().cancelSubscription(purchaseId,
        this, SLOT(cancelSubscriptionResponse(bb::platform::CancelSubscriptionResponseInfo)));
}

/**
 * Invoked in response to the subscription cancellation of a purchased item.
 */
void PaymentServiceControl::cancelSubscriptionResponse(bb::platform::CancelSubscriptionResponseInfo response)
{
    if (response.error()) {
        std::cout << "Cancel subscription response error. Code(" << response.errorCode() << ") Text(" << response.errorText().toStdString() << ")" <<std::endl;
        emit infoResponseError(response.errorCode(), response.errorText());
    } else {
        std::cout << "Cancel subscription response success. Canceled? " << response.canceled() << std::endl;
        //emit success response if cancellation of item's subscription was without errors.
        emit cancelSubscriptionResponseSuccess(response.canceled());
    }
}

