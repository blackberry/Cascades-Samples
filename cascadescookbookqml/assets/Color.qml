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
import bb.cascades 1.0
import "Common"

// This recipe shows how Color can be set on Label and Containers.
// The QML uses a helper QML file (ColorHelper.qml) that layouts a rect and a text
// so that we can easily call it multiple times with all the colors!
RecipePage {
    RecipeContainer {

        Container {
            layout: StackLayout {
                leftPadding: 80
                rightPadding: leftPadding
            }
            layoutProperties: DockLayoutProperties {
                horizontalAlignment: HorizontalAlignment.Center
                verticalAlignment: VerticalAlignment.Center
            }
            ColorHelper {
                colortext: "Red"
                containerColor: Color.Red
            }
            ColorHelper {
                colortext: "DarkRed"
                containerColor: Color.DarkRed
            }
            ColorHelper {
                colortext: "Magenta"
                containerColor: Color.Magenta
            }
            ColorHelper {
                colortext: "DarkMagenta"
                containerColor: Color.DarkMagenta
            }
            ColorHelper {
                colortext: "Blue"
                containerColor: Color.Blue
            }
            ColorHelper {
                colortext: "DarkBlue"
                containerColor: Color.DarkBlue
            }
            ColorHelper {
                colortext: "Cyan"
                containerColor: Color.Cyan
            }
            ColorHelper {
                colortext: "DarkCyan"
                containerColor: Color.DarkCyan
            }
            ColorHelper {
                colortext: "Green"
                containerColor: Color.Green
            }
            ColorHelper {
                colortext: "DarkGreen"
                containerColor: Color.DarkGreen
            }
            ColorHelper {
                colortext: "Yellow"
                containerColor: Color.Yellow
            }
            ColorHelper {
                colortext: "DarkYellow"
                containerColor: Color.DarkYellow
            }
            ColorHelper {
                colortext: "White"
                containerColor: Color.White
            }
            ColorHelper {
                colortext: "LightGray"
                containerColor: Color.LightGray
            }
            ColorHelper {
                colortext: "Gray"
                containerColor: Color.Gray
            }
            ColorHelper {
                colortext: "DarkGray"
                containerColor: Color.DarkGray
            }
            ColorHelper {
                colortext: "Black"
                containerColor: Color.Black
            }
            ColorHelper {
                colortext: "Custom"
                containerColor: Color.create (0.5, 0.5, 0.5)
            }
            ColorHelper {
                colortext: "Strawberry"
                containerColor: Color.create ("#ab2025")
            }
            /*
            ColorHelper {
                colortext: "Transparent"
                containerColor: Color.Transparent
            }
            */
        }
    }
}
