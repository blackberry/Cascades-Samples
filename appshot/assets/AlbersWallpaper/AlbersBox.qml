/* Copyright (c) 2013, 2014 BlackBerry Limited.
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

Container {
    opacity: 1
    property alias color1: box1.background
    property alias color2: box2.background
    property alias color3: box3.background
    property alias color4: box4.background
    
    property alias box1Top: box1.topPadding
    property alias box1Left: box1.leftPadding
    property alias box2Top: box2.topPadding
    property alias box2Left: box2.leftPadding
    property alias box3Top: box3.topPadding
    property alias box3Left: box3.leftPadding
    
    leftPadding: ui.du(2)
    topPadding: ui.du(2)
    
    layout: DockLayout {
    }
    
    Container {
        id: box1
        preferredWidth: ui.du(19.3)
        preferredHeight: preferredWidth

		layout: DockLayout {
        }
        
        Container {
            id: box2
            preferredWidth: box1.preferredWidth * 0.7
            preferredHeight: preferredWidth

            layout: DockLayout {
            }
            
            Container {
                id: box3
                preferredWidth: box2.preferredWidth * 0.6
                preferredHeight: preferredWidth    
                
                layout: DockLayout {
                }
                
                Container {
                    id: box4
                    preferredWidth: box3.preferredWidth * 0.6
                    preferredHeight: preferredWidth            
                }    
            }    
        }    
    }    
}
