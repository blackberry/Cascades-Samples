/* Copyright (c) 2013 BlackBerry Limited.
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
import bb.cascades 1.0

Container {
    property alias url: webview.url
    property alias webview: webview
    property alias scrollview: scrollview

    verticalAlignment: VerticalAlignment.Fill
    horizontalAlignment: HorizontalAlignment.Fill
    preferredHeight: Infinity
    preferredWidth: Infinity
    background: Color.create("#ffffff")

    layout: DockLayout {
        // hi
    }

    ScrollView {
        id: scrollview

        implicitLayoutAnimationsEnabled: false
        scrollViewProperties.minContentScale: 1.0

        WebView {
            id: webview
            implicitLayoutAnimationsEnabled: false
            preferredHeight: Infinity
            preferredWidth: Infinity

            // specify the css file for our css overrides
            settings.userStyleSheetLocation: "asset:///settings.css"

            // enable web-inspector
            //settings.webInspectorEnabled: true

            // viewport setup
            settings.viewport: {
                "width": "device-width",
                "height": "device-height",
                "initial-scale": 1.0
            }

            // progress indicator
            onLoadProgressChanged: {
                progress.value = loadProgress;
                if (loadProgress == 100) {
                    progress.visible = false;
                } else {
                    progress.visible = true;
                }
            }

            // 'back button' handling - when user touches screen, show the button
            // if tab url is not the original url from settings.json, show back button
            onTouch: {
                var templateUrl = init.activeTab.template.url;
                var currentUrl = init.activeTab.webcontainer.url;

                if (templateUrl != currentUrl) {
                    console.log('show button');
                    if (! backButton.visible) {
                        fadeIn.play();
                    }
                }
            }

        } // WebView

        onCreationCompleted: {
            // disable the cache, because we like cash, not cache.  >:|
            webview.storage.clearCache();

            // pinch to zoom DISABLED
            if (appSettings.settings.pinchToZoom === "false") {
                scrollview.scrollViewProperties.scrollMode = ScrollMode.Vertical;
                scrollview.scrollViewProperties.pinchToZoomEnabled = false;

                // pinch to zoom ENABLED
            } else {
                scrollview.scrollViewProperties.scrollMode = ScrollMode.Both;
                scrollview.scrollViewProperties.pinchToZoomEnabled = true;
            }
        } // onCreationCompleted
    } // ScrollView

    // the back button
    Container {
        id: backButtonWrapper
        preferredHeight: 73
        preferredWidth: 105
        translationY: 20.0
        translationX: 20.0
        visible: true

        ImageButton {
            id: backButton
            defaultImageSource: "asset:///images/back.png"
            pressedImageSource: "asset:///images/back.png"
            verticalAlignment: VerticalAlignment.Bottom
            horizontalAlignment: HorizontalAlignment.Left
            opacity: 0
            visible: false

            animations: [
                FadeTransition {
                    id: fadeIn
                    delay: 0
                    duration: 700
                    easingCurve: StockCurve.BackIn
                    fromOpacity: 0
                    toOpacity: 1.0

                    onStarted: {
                        backButton.visible = true;
                    }

                    onEnded: {
                        fadeOut.play();
                    }
                },

                FadeTransition {
                    delay: 6000
                    id: fadeOut
                    duration: 700
                    easingCurve: StockCurve.BackOut
                    fromOpacity: 1.0
                    toOpacity: 0

                    onEnded: {
                        backButton.visible = false

                    }
                }
            ]

            onClicked: {
                webview.goBack();
            }

        } // imagebutton
    } // container

    // the progress indicator control
    ProgressIndicator {
        id: progress
        fromValue: 0
        toValue: 100
        horizontalAlignment: HorizontalAlignment.Center
        verticalAlignment: VerticalAlignment.Bottom
    }
}// Container