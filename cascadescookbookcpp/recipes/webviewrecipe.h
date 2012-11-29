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

#ifndef _WEBVIEWRECIPE_H_
#define _WEBVIEWRECIPE_H_

#include <bb/cascades/CustomControl>

using namespace bb::cascades;

namespace bb
{
    namespace cascades
    {
        class ProgressIndicator;
        class WebLoadRequest;
        class WebNavigationRequest;
    }
}

/* WebViewRecipe Description:
 * 
 * This recipe shows how the WebView can be used. The signal handlers of
 * the Control is used to show loading progress in a ProgressBar.
 */
class WebViewRecipe: public bb::cascades::CustomControl
{
    Q_OBJECT

public:
    /**
     * Constructor; sets up the recipe
     * @param parent The parent Container, if not specified, 0 is used
     */
    WebViewRecipe(Container *parent = 0);

public slots:
    /**
     * This Slot function is for loading status changed
     *
     * @param loadRequest The current status of the URL loading.
     */
    void onLoadingChanged(bb::cascades::WebLoadRequest *loadRequest);

    /**
     * This Slot function is for loading progress. In this recipe, we use this
     * to update a ProgressIndicator.
     */
    void onProgressChanged();

    /**
     * This Slot function is for Navigation requested signal, where a progress
     * bar is shown in this recipe.
     *
     * @param request The request to be changed to
     */
    void onNavigationRequested(bb::cascades::WebNavigationRequest *request);

private:
    ProgressIndicator *mLoadingIndicator;
};

#endif // ifndef _WEBVIEWRECIPE_H_
