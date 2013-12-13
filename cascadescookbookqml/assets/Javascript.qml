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

// Import a JavaScript file containing myPow(a, b) function 
// which returns the value of a to the power of b.
import "mypowfile.js" as MathFunctions

RecipePage {
    RecipeContainer {
        Container {
            horizontalAlignment: HorizontalAlignment.Center
            verticalAlignment: VerticalAlignment.Center

            Container {
                Label {
                    // Calling the local function
                    text: "Local function :" + myPow (10, 2)
                    textStyle {
                        base: SystemDefaults.TextStyles.BigText
                    }
                }
                Label {
                    // Call the JavaScript function that resides in mypowfile.js 
                    text: "From file :" + MathFunctions.myPow (10, 3)
                    textStyle.base: SystemDefaults.TextStyles.BigText
                }
            }// Container
        }// Container
    }// RecipeContainer

    // This is a local function that does a little power of calculation
    function myPow (a, b) {
        a = parseInt (a);
        b = parseInt (b);
        var result = a;
        for (var i = 1; i < b; i++) {
            result *= a;
        }
        return result;
    }
}
