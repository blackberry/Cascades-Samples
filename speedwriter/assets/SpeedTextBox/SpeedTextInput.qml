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

// The input field configured to be used as input for the SpeedTextBox.
TextField {
    id: textInput
    
    // The changing of the Application scene is what triggers the field to have focus
    // when the application starts.
    property variant appScene: Application.scene
    
    // If validSpeedText is false TextField validation will visually show that an error has occurred.
    property bool validSpeedText: true;
    
    hintText: "Type here to see how fast you are."
    input.flags: TextInputFlag.AutoCapitalizationOff | TextInputFlag.AutoCorrectionOff | TextInputFlag.PredictionOff | TextInputFlag.SpellCheckOff | TextInputFlag.WordSubstitutionOff  

    // We want the keyboard to be text input.
    input
    {
        keyLayout: KeyLayout.Text
    }
    
    clearButtonVisible: false
    
    textStyle {
        fontSize: FontSize.PointValue
        fontSizeValue: 10
    }
    
    // Custom validation is performed by the wordchecker object below
    validator: Validator {
        mode: ValidationMode.Custom
        errorMessage: "You are doing something wrong"
        state: ValidationState.Unknown
        
        onValidate: {
            if (validSpeedText) {
                state = ValidationState.Valid;
            } else {
                state = ValidationState.Invalid;
            }
        }
    }
    
    onValidSpeedTextChanged: {
        // Run validation on the text field if the word checker valid state changes.
        validator.validate();
    }
    
    onAppSceneChanged: {
        // Focus cannot be requested until the TextInput is actually added to the scene,
        // so we wait until the scene has been set on the application.
        requestFocus();
    }

}
