/*
 * Copyright (c) 2011-2014 BlackBerry Limited.
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

#include "MMediaAd.hpp"

using namespace bb::cascades;
using namespace bb::device;
using namespace bb::system;

MMediaAd::MMediaAd(Container *parent)
        : CustomControl(parent), parent_(parent), created_(false), showAd_(true), refreshTimer_(
                new QTimer(this)), refreshRate_(30), retried_(false) {
    qDebug() << "Creating Millennial Media Ad";
} /* MMediaAd */

MMediaAd::~MMediaAd() {
    this->destroyAd();
} /* ~MMediaAd */

void MMediaAd::onRefresh() const {
    qDebug() << "Refreshing Ad for:" << this->apid_;
    this->bannerAds_->reload();
} /* onRefresh */

QString MMediaAd::apid() const {
    return this->apid_;
} /* apid */

void MMediaAd::setApid(const QString apid) {
    this->apid_ = apid;

    //Navigate if ad shown
    if( this->showAd_ && this->created_ ) {
        this->loadAd();
    } /* if */
    this->apidChanged(apid);
} /* setApid */

int MMediaAd::refreshRate() const {
    return this->refreshRate_;
} /* refreshRate */

void MMediaAd::setRefreshRate(const int value) {
    this->refreshRate_ = value;
    // Update the timer
    if( this->showAd_ && this->created_ ) {
        this->refreshTimer_->start(value * 1000);
    } /* if */
    refreshRateChanged(value);
} /* setRefreshRate */

bool MMediaAd::showAd() const {
    return this->showAd_;
} /* showAd */

void MMediaAd::setShowAd(const bool value) {
    this->showAd_ = value;

    // If ad is hidden, destroy it completely.
    if( value ) {
        this->createAd();
    } else {
        this->destroyAd();
    } /* if */
    showAdChanged(value);
} /* setShowAd */

void MMediaAd::onNoFill(WebLoadRequest *loadRequest) {
    qDebug() << "Load Request Status:" << loadRequest->status();
    // Ideally we also want to check for:
    // loadRequest->status() == WebLoadStatus::Succeeded && (bannerAds_->html()).length() == 0)
    // as well as per mmedia documentation, but the WebView does not actually return loaded HTML,
    // only explicitly specified HTML. We also can't check for HTTP error code 200 (easily)
    if( loadRequest->status() == WebLoadStatus::Failed ) {
        qDebug() << "No ad fill with apid:" << this->apid_ << "(Maybe)";
        // Retry ONCE
        if( !this->retried_ ) {
            qDebug() << "Attempting Retry";
            this->loadAd();
            this->retried_ = true;
        } /* if */
    } /* if */

    if( loadRequest->status() == WebLoadStatus::Succeeded ) {
        // Removes borders around the ad image
        const QString script = "document.body.style.margin='0';document.body.style.padding = '0'";
        this->bannerAds_->evaluateJavaScript(script);
        this->bannerAds_->settings()->setBackground(Color::Transparent);
        this->retried_ = false;
        qDebug() << "Ad successfully loaded";
    } /* if */
} /* onNoFill */

void MMediaAd::onAdTouched(WebNavigationRequest *request) {
    // If we're simply refreshing the ad, don't do anything and let the lifecycle continue.
    // If it's a click, stop the WebView: we want to spawn a new browser.
    // detect what the ad is
    //TODO: Handle specific ad requests here
    const QString url = (request->url()).toString(QUrl::None);
    qDebug() << "Navigating to ad url:" << url;
    if( url.contains("getAd") ) {
        qDebug() << "Getting Ad";
        request->setAction(WebNavigationRequestAction::Accept);

    } else {
        qDebug() << "Ad Clicked!";
        request->setAction(WebNavigationRequestAction::Ignore);

        InvokeRequest ir;
        ir.setAction("bb.action.OPEN");
        ir.setTarget("sys.browser");
        ir.setMimeType("text/html");
        ir.setUri(url);

        InvokeManager im(this);
        im.invoke(ir);
    } /* if */
} /* onAdTouched */

/**
 * Private Methods
 *****************/
void MMediaAd::createAd() {
    if( this->created_ )
        return;

    // Create the WebView for the ad
    this->bannerAds_ = new WebView(parent_);
    this->setRoot(this->bannerAds_);
    this->bannerAds_->accessibility()->setName("Ad Banner");

    //Try to get the auid
    HardwareInfo info;
    this->auid_ = info.pin();
    // If we have permission, otherwise mmedia will use IP as default
    if( this->auid_.length() != 0 ) {
        // Strip 0x as recommended by mmedia
        this->auid_.remove("0x");
        this->auid_ = "&auid=" + this->auid_;
    } /* if */

    bool ok = connect(this->bannerAds_,
            SIGNAL(navigationRequested(bb::cascades::WebNavigationRequest *)), this,
            SLOT(onAdTouched(bb::cascades::WebNavigationRequest *)));
    Q_ASSERT (ok);
    ok = connect(this->bannerAds_, SIGNAL(loadingChanged(bb::cascades::WebLoadRequest *)), this,
            SLOT(onNoFill(bb::cascades::WebLoadRequest *)));
    Q_ASSERT(ok);

    // Configure the refresh ad timer
    ok = connect(this->refreshTimer_, SIGNAL(timeout()), this, SLOT(onRefresh()));
    Q_ASSERT(ok);

    this->loadAd();

    this->created_ = true;
} /* createAd */

void MMediaAd::destroyAd() {
    if( this->created_ ) {
        this->bannerAds_->deleteLater();
        this->refreshTimer_->stop();
        this->refreshTimer_->deleteLater();
        this->created_ = false;
    } /* if */
} /* destroysAd */

void MMediaAd::loadAd() {
    if( this->auid_.length() == 0 )
        return;

    const QUrl adUrl("http://ads.mp.mydas.mobi/getAd?apid=" + this->apid_ + this->auid_);
    this->bannerAds_->setUrl(adUrl);
    this->refreshTimer_->start(this->refreshRate_ * 1000);
} /* loadAd */
