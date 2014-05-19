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

// This is a component used by the GridLayoutRecipe, for devices fulfilling the
// mindw76h128du, one more option will be added to the SegmentedControl in the panel.
Container {
    property alias columns: columnCount.selectedValue
    
    background: Color.create("#33ffffff")
    topPadding: ui.du(2)
    bottomPadding: ui.du(2)
    leftPadding: ui.du(2)
    rightPadding: ui.du(2)
    
    // The Container use a GridLayout with default column count (2).
    layout: GridLayout {
    }
    
    Label {
        text: "Columns: "        
        verticalAlignment: VerticalAlignment.Center
        
        // The width of the column has to be set since the SegmentedControl will try and span the entire screen width.
        minWidth: ui.du(15)
    }
    
    SegmentedControl {
        id: columnCount
        Option {
            text: "4"
            value: 4
        }
        Option {
            text: "6"
            value: 6
        }
    }
}
