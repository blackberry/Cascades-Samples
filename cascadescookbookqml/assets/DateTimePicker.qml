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

// In this recipe the DateTimePicker Control is introduced.
// By setting different dates in the past, present, and future
// one can time-warp the ripening and decay of a banana.
RecipePage {
    resizeBehavior: PageResizeBehavior.None
    RecipeContainer {
        ScrollView {
            scrollViewProperties {
                scrollMode: ScrollMode.Vertical
            }
            Container {
                // Today's date
                topPadding: ui.du(3)
                leftPadding: ui.du(2)
                rightPadding: ui.du(2)

                Label {
                    id: title
                    text: "Today is: " + (new Date()).toDateString()
                }

                // Create the DateTimePicker. We set the mode so that we
                // can change the date and update the state of the fruit depending on that date.
                DateTimePicker {
                    mode: DateTimePickerMode.Date
                    title: "Banana at date:"
                    horizontalAlignment: HorizontalAlignment.Center

                    onValueChanged: {
                        var today = new Date();
                        if (value < today) {
                            timeWarpFruit.imageSource = "asset:///images/picker/banana_past.png"
                        } else if (value.getYear() == today.getYear() && value.getMonth() > today.getMonth() + 1 || value.getYear() > today.getYear()) {
                            timeWarpFruit.imageSource = "asset:///images/picker/banana_ancient.png"
                        } else if (value.getDate() <= today.getDate() + 4 && value.getMonth() == today.getMonth()) {
                            timeWarpFruit.imageSource = "asset:///images/picker/banana_new.png"
                        } else {
                            timeWarpFruit.imageSource = "asset:///images/picker/banana_old.png"
                        }
                    }
                }

                // An image of a fruit is used to show how we can use the QDateTime value reported by 
                // the picker, set scaling to AspectFit since the width might vary depending on screen size
                ImageView {
                    id: timeWarpFruit
                    topMargin: ui.du(2) // 2 is the UI default for a medium padding.
                    imageSource: "asset:///images/picker/banana_new.png"
                    scalingMethod: ScalingMethod.AspectFit
                }
            }
        }
    }
}
