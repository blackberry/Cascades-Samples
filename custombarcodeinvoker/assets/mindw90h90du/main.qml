/* Copyright (c) 2012, 2013  BlackBerry Limited.
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

// creates one page with a label and button to invoke barcodescanner

Page {
    titleBar: TitleBar {
        title: qsTr("Barcode Invoker")
        appearance: TitleBarAppearance.Branded
    }
    //! [0]
    Container {
        maxHeight: ui.du(80)
        layout: DockLayout {
        }

        background: Color.Black

        Container {
            horizontalAlignment: HorizontalAlignment.Fill
            verticalAlignment: VerticalAlignment.Fill

            bottomPadding: ui.du(2.2)

            layout: DockLayout {
            }

            // The background image
            ImageView {
                horizontalAlignment: HorizontalAlignment.Fill
                verticalAlignment: VerticalAlignment.Fill
                imageSource: "asset:///images/background.png"
            }
        }

        Container {
            horizontalAlignment: HorizontalAlignment.Fill
            verticalAlignment: VerticalAlignment.Fill

            topPadding: ui.du(2.2)
            leftPadding: ui.du(2.2)
            bottomPadding: ui.du(2.2)

            layout: DockLayout {
            }

            //Button to invoke the barcodescanner card - for this to work
            //you have to have barcodescanner sample on device
            Button {
                horizontalAlignment: HorizontalAlignment.Center
                verticalAlignment: VerticalAlignment.Top

                text: qsTr("Scan Barcode")
                onClicked: {
                    _barcodeInvoker.onInvokeButtonClicked()
                }
            }
            //displays barcode data
            Label {
                horizontalAlignment: HorizontalAlignment.Left
                verticalAlignment: VerticalAlignment.Bottom

                text: qsTr("Barcode: %1").arg(_barcodeInvoker.barcode)
                textStyle.color: Color.White
            }
        }
    }
    //! [0]
}
