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

Page {
    content: Container {
        layout: DockLayout {
        }
        
        ImageView {
            imageSource: "asset:///images/background.png"
            preferredWidth: 768
            preferredHeight: 1280
        }
        
        Container {
            
            layout: StackLayout {
                topPadding: 110
            }
            
            layoutProperties: DockLayoutProperties {
                horizontalAlignment: HorizontalAlignment.Center
            }
            
            Label {
                text: "KAKEL"
                textStyle {
                    base: SystemDefaults.TextStyles.BigText
                    fontWeight: FontWeight.Bold
                    color: Color.create ("#E6E9F8")
                }
                
                layoutProperties: StackLayoutProperties {
                    horizontalAlignment: HorizontalAlignment.Center
                }
            }
            
            Label {
                id: propertytime
                
                // kakel.numMoves is a property that we keep track on in cpp code
                // every time it's changed the text will be updated.
                text: "Moves:  " + kakel.numMoves
                topMargin: 40
                textStyle {
                    base: SystemDefaults.TextStyles.TitleText
                    fontWeight: FontWeight.Bold
                    color: Color.create ("#E6E9F8")
                }
                
                layoutProperties: StackLayoutProperties {
                    horizontalAlignment: HorizontalAlignment.Center
                }
            }
            
            // Here we group the containers playarea, a background and a nice flare ontop
            Container {
                topMargin: 100
                
                layout: DockLayout {
                }
                
                layoutProperties: StackLayoutProperties {
                    horizontalAlignment: HorizontalAlignment.Center
                }

                //This container is the one we find from CPP and add all the tiles to
                // It's so much easier to do that from C++.
                Container {
                    objectName: "playArea"
                                        
                    //The red is an awesome trick, we can use it to se where we put our playarea when layouting
                    background: Color.Red
                    preferredWidth: 724
                    preferredHeight: preferredWidth
                
                    layout: StackLayout {
                        layoutDirection: LayoutDirection.LeftToRight
                    }
                }
            }
            
            Container {
                topMargin: 40
                
                layout: StackLayout {
                    layoutDirection: LayoutDirection.LeftToRight
                }

                // These buttons will call the CPP code to generate new playfields
                // try other sizes, I dare you!
                Button {
                    text: "Simple"
                    imageSource: "asset:///images/icon_simple"
                    onClicked: {
                        kakel.setNumTiles (4);
                        kakel.resetTiles ();
                    }
                }
                
                Button {
                    text: "Complex"
                    imageSource: "asset:///images/icon_complex"
                    onClicked: {
                        kakel.setNumTiles (6);
                        kakel.resetTiles ();
                    }
                }
            }
        }
    }
}
