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
#include "webviewrecipe.h"

#include <bb/cascades/Container>
#include <bb/cascades/DockLayout>
#include <bb/cascades/Label>
#include <bb/cascades/ProgressIndicator>
#include <bb/cascades/ScrollView>
#include <bb/cascades/WebLoadRequest>
#include <bb/cascades/WebNavigationRequest>
#include <bb/cascades/WebSettings>
#include <bb/cascades/WebView>

using namespace bb::cascades;

WebViewRecipe::WebViewRecipe(Container *parent) :
        CustomControl(parent)
{
    // The recipe Container
    Container *recipeContainer = new Container();
    recipeContainer->setLayout(new DockLayout());
    recipeContainer->setPreferredHeight(1280);

    WebView *webView = new WebView();
    webView->setUrl(
            QUrl("https://github.com/blackberry/Cascades-Samples/blob/master/cascadescookbookqml/assets/Slider.qml"));

    // We let the scroll view scroll in both x and y and enable zooming,
    // max and min content zoom property is set in the WebViews onMinContentScaleChanged
    // and onMaxContentScaleChanged signal handlers.
    mScrollView = ScrollView::create().scrollMode(ScrollMode::Both).pinchToZoomEnabled(true);
    mScrollView->setContent(webView);

    // Connect to signals to manage the progress indicator.
    connect(webView, SIGNAL(loadingChanged(bb::cascades::WebLoadRequest *)), this,
            SLOT(onLoadingChanged(bb::cascades::WebLoadRequest *)));
    connect(webView, SIGNAL(loadProgressChanged(int)), this, SLOT(onProgressChanged(int)));
    connect(webView, SIGNAL(navigationRequested(bb::cascades::WebNavigationRequest *)), this,
            SLOT(onNavigationRequested(bb::cascades::WebNavigationRequest *)));

    // Connect signals to manage the web contents suggested size.
    connect(webView, SIGNAL(maxContentScaleChanged(float)), this, SLOT(onMaxContentScaleChanged(float)));
    connect(webView, SIGNAL(minContentScaleChanged(float)), this, SLOT(onMinContentScaleChanged(float)));

    // Connect signal to handle java script calls to navigator.cascades.postMessage()
    connect(webView, SIGNAL(messageReceived(const QVariantMap&)), this, SLOT(onMessageReceived(const QVariantMap&)));

    WebSettings *settings = webView->settings();
    QVariantMap settingsMap;
    settingsMap["width"] = QString("device-width");
    settingsMap["initial-scale"] = 1.0;
    settings->setViewportArguments(settingsMap);

    // A progress indicator that is used to show the loading status
    Container *progressContainer = Container::create().bottom(25);
    progressContainer->setLayout(new DockLayout());
    progressContainer->setVerticalAlignment(VerticalAlignment::Bottom);
    progressContainer->setHorizontalAlignment(HorizontalAlignment::Center);
    mLoadingIndicator = ProgressIndicator::create().opacity(0.0);
    progressContainer->add(mLoadingIndicator);

    // Add the controls and set the root Container of the Custom Control.
    recipeContainer->add(mScrollView);
    recipeContainer->add(progressContainer);
    setRoot(recipeContainer);
}

void WebViewRecipe::onLoadingChanged(bb::cascades::WebLoadRequest *loadRequest)
{
    if (loadRequest->status() == WebLoadStatus::Started) {
        // Show the ProgressBar when navigation is requested.
        mLoadingIndicator->setOpacity(1.0);
    } else if (loadRequest->status() == WebLoadStatus::Succeeded) {
        // The ProgressIndicator is hidden on success or failure.
        mLoadingIndicator->setOpacity(0.0);
    } else if (loadRequest->status() == WebLoadStatus::Failed) {
        WebView *webView = dynamic_cast<WebView*>(sender());
        mLoadingIndicator->setOpacity(0.0);

        // If loading failed, fallback a local html file which will also send a java script message
        QUrl fallbackUrl("local:///assets/WebViewFallback.html");
        webView->setUrl(fallbackUrl);
    }
}

void WebViewRecipe::onProgressChanged(int loadProgress)
{
    // Update the progress in the loading ProgressIndicator.
    mLoadingIndicator->setValue((float) loadProgress / 100.0f);
}

void WebViewRecipe::onNavigationRequested(bb::cascades::WebNavigationRequest *request)
{
    // Navigation requested the signal handler, just print to console to illustrate usage.
    qDebug() << "onNavigationRequested " << request->url() << " navigationType: "
            << request->navigationType();
}

void WebViewRecipe::onMinContentScaleChanged(float minContentScale)
{
    ScrollViewProperties* scrollViewProp = mScrollView->scrollViewProperties();

    // Update the scroll view properties to match the content scale
    // given by the WebView.
    scrollViewProp->setMinContentScale(minContentScale);

    // Let's show the entire page to start with.
    mScrollView->zoomToPoint(0, 0, minContentScale, ScrollAnimation::None);
}

void WebViewRecipe::onMaxContentScaleChanged(float maxContentScale)
{
    ScrollViewProperties* scrollViewProp = mScrollView->scrollViewProperties();

    // Update the scroll view properties to match the content scale
    // given by the WebView.
    scrollViewProp->setMaxContentScale(maxContentScale);
}

void WebViewRecipe::onMessageReceived(const QVariantMap& message)
{
    // If not connected to a network the java script in the fallback page
    // WebViewFallback.html will send a message to this signal handler
    // illustrating communication between a java script and Cascades.
    qDebug() << "message.origin: " << message["origin"];
    qDebug() << "message.data: " << message["data"];
}

