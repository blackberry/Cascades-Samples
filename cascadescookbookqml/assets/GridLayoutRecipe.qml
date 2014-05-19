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
import "Common"

// A recipe showing the Container GridLayout in action. A GridLayout
// has a coloumCount and fills row by row the content put into it.
RecipePage {
    RecipeContainer {
        Container {
            verticalAlignment: VerticalAlignment.Bottom
            
            Container {
                id: bookShelfContainer
                property int columns: panel.columns                
                horizontalAlignment: HorizontalAlignment.Center
                bottomPadding: ui.du(3)
                
                Container {
                    // The GridLayout with a variable coloumnCount
                    layout: GridLayout {
                        columnCount: bookShelfContainer.columns
                    }
                    
                    // This Grid is populated with a number of custom Containers, some of them
                    // with an ImageView as content.
                    GridLayoutCustomCell {
                        ImageView {
                            imageSource: "asset:///images/gridlayout/pepper.png"
                            horizontalAlignment: HorizontalAlignment.Right
                            verticalAlignment: VerticalAlignment.Bottom
                        }
                    }
                    
                    GridLayoutCustomCell {
                    }
                    GridLayoutCustomCell {
                    }
                    GridLayoutCustomCell {
                        ImageView {
                            imageSource: "asset:///images/gridlayout/egg.png"
                            horizontalAlignment: HorizontalAlignment.Right
                            verticalAlignment: VerticalAlignment.Bottom
                        }
                    }
                    GridLayoutCustomCell {
                        ImageView {
                            imageSource: "asset:///images/gridlayout/pot.png"
                            horizontalAlignment: HorizontalAlignment.Center
                            verticalAlignment: VerticalAlignment.Bottom
                        }
                    }
                    GridLayoutCustomCell {
                    }
                    GridLayoutCustomCell {
                        ImageView {
                            imageSource: "asset:///images/gridlayout/pepper.png"
                            horizontalAlignment: HorizontalAlignment.Left
                            verticalAlignment: VerticalAlignment.Bottom
                        }
                    }
                    GridLayoutCustomCell {
                    }
                    GridLayoutCustomCell {
                    }
                    GridLayoutCustomCell {
                        ImageView {
                            imageSource: "asset:///images/gridlayout/egg.png"
                            horizontalAlignment: HorizontalAlignment.Left
                            verticalAlignment: VerticalAlignment.Bottom
                        }
                    }
                    GridLayoutCustomCell {
                        ImageView {
                            imageSource: "asset:///images/gridlayout/egg.png"
                            horizontalAlignment: HorizontalAlignment.Right
                            verticalAlignment: VerticalAlignment.Bottom
                        }
                    }
                    GridLayoutCustomCell {
                    }
                }
                // The legs of the bookcase.
	            ImageView {
	                imageSource: "asset:///images/gridlayout/legs4.amd"
	                horizontalAlignment: HorizontalAlignment.Fill
	                topMargin: 0
	            }
            }

            // The bottom panel is also a GridLayout checkout GridLayoutPanel.qml 
			GridLayoutPanel {
			    id: panel			    
			}
        }
    }
}
