/*
 * Copyright (c) 2013 BlackBerry Limited.
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
import bb.cascades.datamanager 1.2
import bb.cascades.decorators 1.2

// ActionItem dealing with the GroupArtist decorator, which
// decorates list by highlighting music ensembles.
ActionItem {
    ActionBar.placement: ActionBarPlacement.InOverflow
    onTriggered: {
        listView.dataModel = groupArtistDecorator
    }
    attachedObjects: [
        // Decorator to change music ensambles display by changing the item type,
        // which provides the custom look for music groups.
        GroupArtistDataModelDecorator {
            id: groupArtistDecorator
            model: defaultModel
        }
    ]
}
