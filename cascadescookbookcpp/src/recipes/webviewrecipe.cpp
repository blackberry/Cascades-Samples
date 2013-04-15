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
            QUrl(
                    "https://github.com/blackberry/Cascades-Samples/blob/master/cascadescookbookqml/assets/Slider.qml"));

    // To enable scrolling in the WebView, it is added to a ScrollView. In this case, we
    // restrict scrolling to vertical mode because it fits with the content being presented.
    ScrollView* scrollView = ScrollView::create().scrollMode(ScrollMode::Vertical);
    scrollView->setContent(webView);
    scrollView->setVerticalAlignment(VerticalAlignment::Fill);
    scrollView->setHorizontalAlignment(HorizontalAlignment::Fill);

    // Connect to signals to manage the progress indicator.
    connect(webView, SIGNAL(loadingChanged(bb::cascades::WebLoadRequest *)), this,
            SLOT(onLoadingChanged(bb::cascades::WebLoadRequest *)));
    connect(webView, SIGNAL(loadProgressChanged( )), this, SLOT(onProgressChanged( )));
    connect(webView, SIGNAL(navigationRequested(bb::cascades::WebNavigationRequest *)), this,
            SLOT(onNavigationRequested(bb::cascades::WebNavigationRequest *)));

    // A progress indicator that is used to show the loading status
    Container *progressContainer = Container::create().bottom(25);
    progressContainer->setLayout(new DockLayout());
    progressContainer->setVerticalAlignment(VerticalAlignment::Bottom);
    progressContainer->setHorizontalAlignment(HorizontalAlignment::Center);
    mLoadingIndicator = ProgressIndicator::create().opacity(0.0);
    progressContainer->add(mLoadingIndicator);

    // Add the controls and set the root Container of the Custom Control.
    recipeContainer->add(scrollView);
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

        // If the load fails, we need a fallback scenario.
        // An example is, if WIFI is not connected, a fallback HTML is presented instead.
        QString fallback =
                QString(
                        "\
                <html>\
                  <head>\
                    <title>Fallback HTML on Loading Failed</title>\
                    <style>\
                      * { margin: 0px; padding 0px; }\
                      body { font-size: 48px; font-family: monospace; border: 1px solid #444; padding: 4px; }\
                    </style>\
                  </head>\
                  <body>\
                    Oh ooh, loading of the URL that was set on this WebView failed. Perhaps you are not connected to the Internet?\
                  </body>\
                </html>");
        webView->setHtml(fallback);
    }
}

void WebViewRecipe::onProgressChanged()
{
    WebView *webView = dynamic_cast<WebView*>(sender());

    if (webView) {
        // Update the progress, divide by 100 to transform from percent to a fraction.
        float progress = (float) webView->loadProgress() / 100.0;
        mLoadingIndicator->setValue(progress);
    }
}

void WebViewRecipe::onNavigationRequested(bb::cascades::WebNavigationRequest *request)
{
    // Navigation requested the signal handler, just print to console to illustrate usage.
    qDebug() << "onNavigationRequested " << request->url() << " navigationType: "
            << request->navigationType();
}
