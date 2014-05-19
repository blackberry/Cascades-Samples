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
import "Common"

// This is a simple ActivityIndicator recipe. Check out the
// Common/InlineActivityIndicator.qml component.
RecipePage {
    RecipeContainer {
        
        // The egg boiling recipe that starts and stops the activity indicator.
        ActivityIndicatorSimulator {
            id: simulator            
        }

        // Instead of using the ActivityIndicator directly its recommended  to
        // show it together with a descriptive text at the bottom of the screen
        // This is achieved by constructing a Component (see Common/InlineActivityIndicator.qml)           
        InlineActivityIndicator {
            running: simulator.simulatorRunning
            title: "Boiling, Boiling, Boiling ..."
        }
    }
}

