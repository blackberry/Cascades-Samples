/* Copyright (c) 2011-2014 BlackBerry Limited.
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
import Advertising.MMedia 1.0

Page {
  Container {
       layout: DockLayout {
       }
       
       ImageView {
           horizontalAlignment: HorizontalAlignment.Fill
           verticalAlignment: VerticalAlignment.Fill
           
           imageSource: "asset:///images/background.png"
       }
       Container {
           horizontalAlignment: HorizontalAlignment.Fill
           verticalAlignment: VerticalAlignment.Fill
           
           // this component is used for displaying MMedia Ad's
           MMediaAd {
               //Apid is an identifier for the application when you register it in MMedia
               //You cannot display Ads without first receiving this identifier from MMedia
               apid: "######" // <-- replace # with your apid from mmedia
               showAd: true
               refreshRate: 60
               horizontalAlignment: HorizontalAlignment.Center
               preferredWidth: 480
               preferredHeight: 60
           }
    
           //includede from weather.qml
           Weather {
           }
       }
   }
}
