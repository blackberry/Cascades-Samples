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

#ifndef MMEDIAAD_HPP_
#define MMEDIAAD_HPP_

#include <QUrl>
#include <QTimer>

#include <bb/cascades/Color>
#include <bb/cascades/Container>
#include <bb/cascades/WebView>
#include <bb/cascades/WebSettings>
#include <bb/cascades/InvokeQuery>
#include <bb/cascades/CustomControl>
#include <bb/cascades/WebLoadRequest>
#include <bb/cascades/WebNavigationRequest>

#include <bb/device/HardwareInfo>

#include <bb/system/InvokeRequest>
#include <bb/system/InvokeManager>

/**
 * Recommended Permissions:
 * read_device_identifying_information
 **/
class MMediaAd: public bb::cascades::CustomControl {
    Q_OBJECT
    Q_PROPERTY(QString apid READ apid WRITE setApid NOTIFY apidChanged FINAL)
    Q_PROPERTY(int refreshRate READ refreshRate WRITE setRefreshRate NOTIFY refreshRateChanged FINAL)
    Q_PROPERTY(bool showAd READ showAd WRITE setShowAd NOTIFY showAdChanged FINAL)

public:
    MMediaAd(bb::cascades::Container *parent = 0);
    virtual ~MMediaAd(void);

    /**
     * Setter and getter for the ad apid.
     **/
    QString apid(void) const;
    void setApid(const QString apid);

    /**
     * Setter and getter for the refresh rate in seconds.
     * Default: 30 seconds
     **/
    int refreshRate(void) const;
    void setRefreshRate(const int refreshRate);

    /**
     * Setter and getter for showing the ad.
     * Default: True
     **/
    bool showAd(void) const;
    void setShowAd(const bool value);


Q_SIGNALS:
    void apidChanged(QString apid);
    void refreshRateChanged(int value);
    void showAdChanged(bool value);

private slots:
    /**
     * Handles the ad being touched.
     **/
    void onAdTouched(bb::cascades::WebNavigationRequest *request);

    /**
     * Handles no ad fills.
     **/
    void onNoFill(bb::cascades::WebLoadRequest *loadRequest);

    /**
     * Handles the ad refresh.
     **/
    void onRefresh(void) const;

private:
    bb::cascades::Container* parent_; // The ad's parent
    bool created_; // True if the ad components have been created
    bool showAd_; // True if the ad is being displayed
    QString apid_;
    bb::cascades::WebView* bannerAds_; // Ad webview
    QTimer* refreshTimer_; // Refreshes the ad every x seconds
    int refreshRate_; // The current refresh rate in seconds
    bool retried_; // True if the current ad has been retried once.

    // The devicePIN stripped
    QString auid_;

    /**
     * Creates all ad elements.
     **/
    void createAd(void);

    /**
     * Destroys all ad elements.
     * Called to save memory when an ad is hidden.
     **/
    void destroyAd(void);

    /**
     * Called when the ad needs to be loaded.
     **/
    void loadAd(void);
};

#endif /* MMEDIAAD_HPP_ */
