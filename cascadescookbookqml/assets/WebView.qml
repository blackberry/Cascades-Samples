/* Copyright (c) 2012 BlackBerry Limited.
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
import bb.cascades 1.2
import "Common"

// This recipe shows how the WebView can be used. The signal handlers of  
// the Control are used to show loading progress in a ProgressBar.
RecipePage {
    RecipeContainer {        
        layout: DockLayout {
        }
        
        // To enable scrolling in the WebView, it is put inside a ScrollView.
        ScrollView {
            id: scrollView

			// We let the scroll view scroll in both x and y and enable zooming,
			// max and min content zoom property is set in the WebViews onMinContentScaleChanged
			// and onMaxContentScaleChanged signal handlers.
            scrollViewProperties {
                scrollMode: ScrollMode.Both
                pinchToZoomEnabled: true
            }

            WebView {
                // The URL that is loaded points to the QML of this recipe on GitHub. 
                url: "https://github.com/blackberry/Cascades-Samples/blob/master/cascadescookbookqml/assets/WebView.qml"

				// WebView settings, initial scaling and width used by the WebView when displaying its content.
                settings.viewport: {
                    "width": "device-width",
                    "initial-scale": 1.0
                }
                
                onLoadProgressChanged: {
                    // Update the ProgressBar while loading.
                    progressIndicator.value = loadProgress / 100.0
                }

                onMinContentScaleChanged: {
                    // Update the scroll view properties to match the content scale
                    // given by the WebView.
                    scrollView.scrollViewProperties.minContentScale = minContentScale;
                    
                    // Let's show the entire page to start with.
                    scrollView.zoomToPoint(0,0, minContentScale,ScrollAnimation.None)
                }

                onMaxContentScaleChanged: {
                    // Update the scroll view properties to match the content scale 
                    // given by the WebView.
                    scrollView.scrollViewProperties.maxContentScale = maxContentScale;
                }
                
                onLoadingChanged: {
                                        
                     if (loadRequest.status == WebLoadStatus.Started) {
                         // Show the ProgressBar when loading started.
                         progressIndicator.opacity = 1.0
                    } else if (loadRequest.status == WebLoadStatus.Succeeded) {
                        // Hide the ProgressBar when loading is complete.
                        progressIndicator.opacity = 0.0
                    } else if (loadRequest.status == WebLoadStatus.Failed) {
                        // If loading failed, fallback a local html file which will also send a Java script message                        
                        url = "local:///assets/WebViewFallback.html"

                        progressIndicator.opacity = 0.0
                    }
                }
                
                // This is the Navigation-requested signal handler so just print to console to illustrate usage. 
                onNavigationRequested: {
                    console.debug ("NavigationRequested: " + request.url + " navigationType=" + request.navigationType)                    
                }
                                
                onMessageReceived: {
                    // If not connected to a network the Java script in the fallback page
                    // WebViewFallback.html will send a message to this signal handler
                    // illustrating communication between a Java script and Cascades.
                    console.debug("message.origin: " + message.origin);
                    console.debug("message.data: " + message.data);
                }
            }
        }// ScrollView
        
        // A progress indicator that is used to show the loading status
        Container {
            bottomPadding: 25
            horizontalAlignment: HorizontalAlignment.Center
            verticalAlignment: VerticalAlignment.Bottom

            ProgressIndicator {
                id: progressIndicator
                opacity: 0
            }
        }
    }// RecipeContainer
}// RecipePage
