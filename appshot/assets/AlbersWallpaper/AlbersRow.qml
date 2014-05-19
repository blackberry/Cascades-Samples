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
    property alias color1: box1.color1
    property alias color2: box1.color2
    property alias color3: box1.color3
    property alias color4: box1.color4
    
    property alias box1Top: box1.box1Top;
    property alias box1Left: box1.box1Left;
    property alias box2Top: box1.box2Top;
    property alias box2Left: box1.box2Left;
    property alias box3Top: box1.box3Top;
    property alias box3Left: box1.box3Left;
    
    layout: StackLayout {
        orientation: LayoutOrientation.LeftToRight
    }
    
    AlbersBox {
        id: box1
    }
    AlbersBox {
        id: box2
        color1: box1.color1
        color2: box1.color2
        color3: box1.color3
        color4: box1.color4
        box1Top: box1.box1Top;
        box1Left: box1.box1Left;
        box2Top: box1.box2Top;
        box2Left: box1.box2Left;
        box3Top: box1.box3Top;
        box3Left: box1.box3Left;       
    }
    AlbersBox {
        id: box3
        color1: box1.color1
        color2: box1.color2
        color3: box1.color3
        color4: box1.color4
        box1Top: box1.box1Top;
        box1Left: box1.box1Left;
        box2Top: box1.box2Top;
        box2Left: box1.box2Left;
        box3Top: box1.box3Top;
        box3Left: box1.box3Left;       

    }
    AlbersBox {
        id: box4
        color1: box1.color1
        color2: box1.color2
        color3: box1.color3
        color4: box1.color4
        box1Top: box1.box1Top;
        box1Left: box1.box1Left;
        box2Top: box1.box2Top;
        box2Left: box1.box2Left;
        box3Top: box1.box3Top;
        box3Left: box1.box3Left;       

    }
}
