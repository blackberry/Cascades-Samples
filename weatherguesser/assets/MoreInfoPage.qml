/* Copyright (c) 2012, 2013, 2014 BlackBerry Limited.
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
import bb.cascades 1.3

// Additional information Page for the user.
Page {
    ScrollView {
        scrollViewProperties {
            scrollMode: ScrollMode.Both
            minContentScale: 0.5
            maxContentScale: 3.0
            pinchToZoomEnabled: true
            overScrollEffectMode: OverScrollEffectMode.OnPinch
            initialScalingMethod: ScalingMethod.AspectFill
        }
        
        Container {
            background: Color.create("#F1EFE9")

            WebView {
                url: "http://developer.blackberry.com/cascades/sampleapps"

                onNavigationRequested: {
                    console.debug("NavigationRequested: " + request.url + " navigationType=" + request.navigationType)
                }
            }
        }
    }
}
