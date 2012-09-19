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

// A custom item used in the list on the WeatherPage
Container {
    id: weatherItem
    layout: DockLayout {
    }
	// Icon image
    ImageView {
        imageSource: "asset:///images/icons/small/" + ListItemData.icon + ".png";
    }

    Container {
        leftPadding: 77
        verticalAlignment: VerticalAlignment.Center

        Label {
            text: ListItemData.tempaverage + "\u00B0"
            textStyle.base: weatherItem.ListItem.view.itemBigTextNormalWhite.style
        }
    }// Container
}// Container
