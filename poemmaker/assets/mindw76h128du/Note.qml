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

// This is the note-component template that is used to setup the three notes in the PoemMaker application.
Container {
    id: noteContainer
    maxWidth: ui.du(31.7)
    
    // Property aliases for the poem text and the animation values of the show animation.
    // Theses can be set on the Note component from another QML document (main.qml).
    property alias poem: poem_part.text
    property alias showAnimStartX: show.fromX

    // The Note component emits a newNote signal when the hide animation has ended,
    // at that point we set up a new note and show the animation.
    signal newNote;

    // All notes use a DockLayout for centering the text.
    layout: DockLayout {
    }
    
    animations: [
        // The hide animation moves the note off the screen in the y-direction.
        // Note: there is no "from" value is specified since we want to avoid the
        // notes jumping back to the starting position if multiple triggers occur
        // before all notes are back in their original position.
        TranslateTransition {
            id: hide
            toY: -400
            duration: 500
            easingCurve: StockCurve.CubicOut
            onEnded: {
                // Emit the new note signal.
                newNote();
            }
        },

        // The show animation is where the Note slides in from the left and comes to a rest at
        // its original position (the starting point is different for specific notes so it is set
        // to a specific location via the property alias showAnimStartX).
        TranslateTransition {
            id: show
            fromX: 0
            toX: 0
            duration: 500
            easingCurve: StockCurve.QuarticOut
        }
    ]

    // The note background image
    ImageView {
        id: noteBackground
        imageSource: "asset:///images/line_background.png"
    }

    // The poem part, text is set via the poem property on the Note component
    Container {
        horizontalAlignment: HorizontalAlignment.Center
        verticalAlignment: VerticalAlignment.Center
        rightPadding:  ui.du(1) 
        leftPadding:  ui.du(1) 
        
        Label {
            id: poem_part
            text: "text"
            multiline: true
            textStyle.base: noteStyle.style
        }
    }

    // This is the implicit-animation controller for the Note. 
    // We need this to set the Note to a new position before showing it without 
    // triggering implicit animations (see showNote function below).
    attachedObjects: [
        ImplicitAnimationController {
            id: offScreenController
            propertyName: "translationY"
            enabled: false
        }
    ]

    // This function that triggers the show animation on a Note.
    function showNote () {

        // Implicit animations are turned off for the translationY property,
        // so the movement of a note to align it will happen immediately. 
        translationY = 0;
        
        show.play ();
    }

    // This function that trigger the hide animation on a Note.
    function hideNote () {
        hide.play ();
    }
}
