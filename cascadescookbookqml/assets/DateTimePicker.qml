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

// In this recipe the DateTimePicker Control is introduced.
// By setting different dates in the past, present and future
// one can time warp the ripening and decay of a banana.

Page {
    content: RecipeContainer {
        layout: StackLayout {
            topPadding: 50
        }

        // Title label
        Container {
            layout: StackLayout {
                leftPadding: 20
                bottomPadding: 20
            }
            Label {
                id: title
                text:{
                    var today = new Date();
                    "Today is: " + (today.getMonth() + 1) + "/" + today.getDate() + "/" + (today.getFullYear() - 2000);
                }
                textStyle {
                    base: SystemDefaults.TextStyles.BodyText
                    color: Color.White
                }
            }
        }

        // This is where we create the DateTimePicker, we set the mode so that we
        // can change the date and update the state of the fruit depending on that.
        DateTimePicker {
            mode: DateTimePickerMode.Date
            title: "Banana at date:"

            onValueChanged: {
                var today = new Date ();

                // We are only interested in whole days so we set all other parts to zero.
                today.setHours (0);
                today.setMilliseconds (0);
                today.setMinutes (0);
                value.setHours (0);
                value.setMilliseconds (0);
                value.setMinutes (0);

                if (value < today) {
                    timeWarpFruit.imageSource = "asset:///images/picker/banana_past.png"
                } else if (value.getYear () == today.getYear () && value.getMonth () > today.getMonth () + 1 ||
                           value.getYear () > today.getYear ()) {
                    timeWarpFruit.imageSource = "asset:///images/picker/banana_ancient.png"
                } else if (value.getDate () <= today.getDate () + 4 && value.getMonth () == today.getMonth ()) {
                    timeWarpFruit.imageSource = "asset:///images/picker/banana_new.png"
                } else {
                    timeWarpFruit.imageSource = "asset:///images/picker/banana_old.png"
                }
            }
        }

        // An image of a fruit is used to show how one can use the QDateTime value
        // reported by the picker.
        Container {
            topMargin: 20
            layoutProperties: StackLayoutProperties {
                    spaceQuota: 1
            }

            ImageView {
                id: timeWarpFruit
                preferredWidth: 768
                preferredHeight: 512
                imageSource: "asset:///images/picker/banana_new.png"
            }
        }
    }
}