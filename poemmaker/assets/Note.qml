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

// Note component template, used to set up the three notes in the poem maker application.

Container {
    id: noteContainer
    // Property aliases for the poem text and the animation values of the show animation.
    // Theses can be set on the Note component from another QML document (main.qml).
    property alias poem: poem_part.text
    property alias showAnimStartX: show.fromX    
    
    // The Note emits a newNote signal when the hide animation has ended, 
    // it this is when it is suitable to set up a new note and trigger the show animation.
    signal newNote;
        
    // All notes use a DockLayout for centering the text.
    layout: DockLayout {
    }
    animations: [
        // The hide animation translates the note off the screen in y-direction.
        // Note that no from value is specified since we want to avoid that the 
        // note jumps back to the starting position if multiple triggers occurs
        // before all notes are back in their original position.
        TranslateTransition {
            id: hide
            toY: -400
            duration: 500
            easingCurve: StockCurve.CubicOut
            onEnded: {
                // Emit the new note signal.
                newNote ();
            }
        },
	   	        
        // The shoe animation is where the Note slides in from the left and comes to a rest at 
        // its original position (the starting point is different for the notes so it is set
        // specific via the property alias showAnimStartX).
        TranslateTransition {
            id: show
            fromX: 0
            toX: 0
            duration: 500
            easingCurve: StockCurve.QuarticOut
        }
    ]
    
    // Implicit animation controller for the Note. All properties are handled by this controller.
    // We need this to set the Note to a new position before showing it without triggering implicit animations 
    // (see showNote function below).
    attachedObjects: [
        ImplicitAnimationController {
            id: offScreenController
        }
    ]
    
    // The note background image.
    ImageView {
        id: noteBackground
        imageSource: "asset:///images/line_background.png"
        preferredWidth: 317
        preferredHeight: 154
    }
    
    // The poem part, text is set via the poem property on the Note component.
    Container {
        layout: DockLayout {
            rightPadding: 10
            leftPadding: 10
        }
        layoutProperties: DockLayoutProperties {
            horizontalAlignment: HorizontalAlignment.Fill
            verticalAlignment: VerticalAlignment.Center
        }
        
        TextArea {
            id: poem_part
            text: "text"
            maxWidth: noteBackground.preferredWidth - 10
            editable: false
            backgroundVisible: false
            textStyle {
                base: SystemDefaults.TextStyles.BodyText
                alignment: TextAlignment.Center
            }
            
            layoutProperties: DockLayoutProperties {
                horizontalAlignment: HorizontalAlignment.Fill
                verticalAlignment: VerticalAlignment.Center
            }
        }
    }
    
    // Function that trigger the show animation on a Note.
    function showNote () {
        
        // Stop the show animation if it is already running.
        if (show.isPlaying ()) {
            show.stop ();
        }
        
        // Turn off implicit animations and move the note in position for sliding into screen. 
        offScreenController.enabled = false;
        translationX = -400;
        noteContainer.translationY = 0; 
        
        // Turn implicit animations back on again
        offScreenController.enabled = true;
        show.play ();
    }
    
    // Function that trigger the hide animation on a Note.
    function hideNote () {
        
        // We only allow triggering the hide animation if it is not already running.
        if (!hide.isPlaying ()) {
            hide.play ();
        }
    }
}
