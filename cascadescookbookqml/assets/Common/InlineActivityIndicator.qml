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

// A component that holds both an ActivityIndicator and a descriptive text.
Container {
    id: recipeIndicatorCon

    // The text describing the ongoing activity.
    property alias title: indicatorText.text

    // Indicates whether the activity indicator is running. 
    property bool running: false

	// The signals emitted by the ActiviyIndicator is also emitted by this component.
	signal started()
	signal stopped()
	signal stopping()

    // Need to set the right color dependent on theme.
    background: setBgColor(Application.themeSupport.theme.colorTheme.style)

	// Initial alignment, padding and visibility.
    rightPadding: ui.du(2)
    leftPadding: rightPadding
    preferredHeight: ui.du(14)
    visible: false;
    verticalAlignment: VerticalAlignment.Bottom
    horizontalAlignment: HorizontalAlignment.Fill

    layout: DockLayout {
    }

    Label {
        id: indicatorText
        horizontalAlignment: HorizontalAlignment.Left
        verticalAlignment: VerticalAlignment.Center
        text: "Default text"
        textStyle.base: SystemDefaults.TextStyles.SubtitleText
    }

    // The activity indicator control.
    ActivityIndicator {
        id: indicator
        verticalAlignment: VerticalAlignment.Center
        horizontalAlignment: HorizontalAlignment.Right
        running: recipeIndicatorCon.running
    }

    // Function used for setting background color dependent on theme.
    function setBgColor(style) {

        switch (style) {
            case VisualStyle.Bright:    return Color.create("#e6f8f8f8")
            case VisualStyle.Dark:      return Color.create("#ff000000")
        }
        return Color.create("#e6f8f8f8")
    }    
    
    // Start animating of the activity indicator.
    function startIndicator () {
        indicator.start();
        visible = true;
    }
    
    // Stop animating the activity indicator.
    function stopIndicator () {
        indicator.stop();
        visible = false;
    }

	onRunningChanged: {
     	if(running) {
     	    startIndicator();
     	} else {
     	    stopIndicator();
     	}
    }

    onCreationCompleted: {
        // Connect the ActivityIndicators signals to this components signals.
        indicator.started.connect(started);
        indicator.stopping.connect(stopping);
        indicator.stopped.connect(stopped);
    }
}
