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

// Import a JavaScript file used to generate the texts for the poem.
import "poemgenerator.js" as PoemGenerator

Page {
    Container {
        background: backgroundPaint.imagePaint
        
        layout: AbsoluteLayout {
        }

        ImageButton {
            defaultImageSource: "asset:///images/rubber.png"
            pressedImageSource: "asset:///images/rubber_depressed.png"
            disabledImageSource: "asset:///images/rubber.png"
            
            layoutProperties: AbsoluteLayoutProperties {
                positionX: 32
                positionY: 500
            }

            // This is the touch signal-handler for the horn-bulb button. Ordinarily for an image
            // button, you would use the onClicked handler. For this sample, it
            // makes more sense for the notes to display when the bulb is pressed down so we
            // will use isDown touch event. At that point the "wind" blows
            // through the straw, this is where animations are triggered.
            onTouch: {
                if (event.isDown()) {

                    // The hide animations are triggered in the hideNote function in Note.qml.
                    note1.hideNote();
                    note2.hideNote();
                    note3.hideNote();

                    // Change the rotation on the notes as they are blown away.
                    note1.rotationZ = 0;
                    note2.rotationZ = -40;
                    note3.rotationZ = 40;
                }
            }
        } // Image Button

        // For the three notes building up the poem, a Note component defined in Note.qml is used.
        // First note; <adjective> + <noun>
        Note {
            id: note1
            property int initialRotation: 0
            layoutProperties: AbsoluteLayoutProperties {
                positionX: 290
                positionY: 40
            }
            showAnimStartY: (- note1.layoutProperties.positionY - 238)

            // The Note component emits a signal called newNote, we connect to it for the first
            // note only and trigger all the show animations here (its emitted when the hide animation has ended).
            onNewNote: {

                // The show animation is started by calling the showNote function in Note.qml (it is not
                // possible to trigger animations using id:s if they reside in a separate QML document).
                showNote();

                // The note is animated back to its original rotation using implicit animations.
                rotationZ = initialRotation;

                // Update the poem while the note is not visible on screen.
                note1.poem = PoemGenerator.generatePoemLine(1);
            }
        } // First Note

        // The second note <verb> + <adverb>
        Note {
            id: note2
            property int initialRotation: -12
            layoutProperties: AbsoluteLayoutProperties {
                positionX: 300
                positionY: 280
            }
            rotationZ: initialRotation
            showAnimStartY: (- note2.layoutProperties.positionY - 238)
            onNewNote: {
                showNote();
                rotationZ = initialRotation;
                note2.poem = PoemGenerator.generatePoemLine(2);
            }
        }

        // The third note <preposition> + <noun>
        Note {
            id: note3
            property int initialRotation: -5
            layoutProperties: AbsoluteLayoutProperties {
                positionX: 320
                positionY: 480
            }
            rotationZ: initialRotation
            showAnimStartY: (- note3.layoutProperties.positionY - 238  )
            onNewNote: {
                showNote ();
                rotationZ = initialRotation;
                note3.poem = PoemGenerator.generatePoemLine (3);
            }
        }
    }

    // When the Page is completely created we call the updatePoem function to generate
	// a new poem with each note called.
    onCreationCompleted: {
        note1.poem = PoemGenerator.generatePoemLine (1);
        note2.poem = PoemGenerator.generatePoemLine (2);
        note3.poem = PoemGenerator.generatePoemLine (3);
    }
    
    attachedObjects: [
        // Non UI objects are specified as attached objects
        TextStyleDefinition {
            id: noteStyle            
            base: SystemDefaults.TextStyles.BodyText
            textAlign: TextAlign.Center
        },
        ImagePaintDefinition {
            id: backgroundPaint
            imageSource: "asset:///images/background.png"
        }
    ]
}
