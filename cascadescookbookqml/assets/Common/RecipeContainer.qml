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

// Common Container for all recipes with appropriate background image
Container {
    // The Container background is set depending on the current theme.
    verticalAlignment: VerticalAlignment.Fill
    background: (Application.themeSupport.theme.colorTheme.style == VisualStyle.Bright) ? backgroundPaint.imagePaint : Color.Black 
    
    attachedObjects: [
        ImagePaintDefinition {
            id: backgroundPaint
            imageSource: "asset:///images/background.amd"
            repeatPattern: RepeatPattern.XY
        }
    ]
    
    layout: DockLayout {
    }
}
