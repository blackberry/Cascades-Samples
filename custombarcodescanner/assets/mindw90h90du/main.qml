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
import bb.community.barcode 1.0

NavigationPane {
    onCreationCompleted: {
        _barcodeScanner.startScan.connect(barcodeDecoder.startScanning)
        _barcodeScanner.stopScan.connect(barcodeDecoder.stopScanning)
    }

    Page {
        titleBar: TitleBar {
            title: qsTr("Barcode Scanner")
            appearance: TitleBarAppearance.Branded
        }

        Container {
            maxHeight: ui.du(80)
            layout: DockLayout {
            }

            background: Color.Black

            //! [0]
            Container {
                horizontalAlignment: HorizontalAlignment.Center
                verticalAlignment: VerticalAlignment.Top
                // Custom control for reading barcodes
                BarcodeDecoder {
                    id: barcodeDecoder
                    onNewBarcodeDetected: {
                        barcodeLabel.text = barcode
                        _barcodeScanner.newBarcodeDetected(barcode)
                    }
                }
            }
            //! [0]
            //! [1]
            Container {
                horizontalAlignment: HorizontalAlignment.Center
                verticalAlignment: VerticalAlignment.Bottom
                bottomPadding: ui.du(2.2)

                // Label for displaying required action and barcode scan result
                Label {
                    id: barcodeLabel
                    text: qsTr("Scan a barcode")
                    textStyle.color: Color.White
                }
            }
            //! [1]
        }
    }
}
