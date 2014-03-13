/*
 * Copyright (c) 2011-2014 BlackBerry Limited.
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 * http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, 
 * software distributed under the License is distributed on an 
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, 
 * either express or implied. See the License for the specific 
 * language governing permissions and limitations under the License.
 */

import bb.cascades 1.2

Container {
    topPadding: 30.0
    rightPadding: 30.0
    bottomPadding: 30.0
    leftPadding: 30.0

    Container {
        topPadding: 40.0
        leftPadding: 50.0
        horizontalAlignment: HorizontalAlignment.Fill

        layout: StackLayout {
            orientation: LayoutOrientation.LeftToRight
        }

        TextField {
            hintText: "Enter host name i.e. www.somewhere.com"
            text: "www.blackberry.com"
            preferredWidth: 593.0
            verticalAlignment: VerticalAlignment.Center

            inputMode: TextFieldInputMode.Url

            // Save url on input
            onTextChanging: _app.hostName = text
        }
        ImageView {
            verticalAlignment: VerticalAlignment.Center
            horizontalAlignment: HorizontalAlignment.Center
            preferredWidth: 26.0
            preferredHeight: 32.0

            imageSource: "asset:///images/encrypted.png"

            // Visibility control
            visible: _app.sessionActive

            // Display certificate info view on touch
            onTouch: _CertInfoControl.visible = true
        }
    }
    Container {
        horizontalAlignment: HorizontalAlignment.Fill
        topMargin: 30.0

        layout: StackLayout {
            orientation: LayoutOrientation.LeftToRight
        }

        Label {
            text: "Port:"            
            horizontalAlignment: HorizontalAlignment.Right
            verticalAlignment: VerticalAlignment.Center
            preferredWidth: 90.0
            textFit.maxFontSizeValue: 7.0
            textStyle.color: Color.LightGray
            textStyle.textAlign: TextAlign.Right
        }
        TextField {
            text: "443"
            preferredWidth: 100.0
            textStyle.fontSizeValue: 3.0
            textFit.maxFontSizeValue: 6.0

            // Save port on input
            onTextChanging: _app.port = text
        }
        Button {
            objectName: "connectBtnObj"
            text: "Connect to host"
            horizontalAlignment: HorizontalAlignment.Right
            maxHeight: 100.0
            preferredWidth: 415.0

            // Connect to url on click
            onClicked: _app.secureConnect()
        }
    }
    Container {
        topMargin: 30.0
        layout: StackLayout {
            orientation: LayoutOrientation.LeftToRight
        }

        // Enable these controls only when connection has
        // been established
        enabled: _app.sessionActive

        Label {
            text: "Input:"
            horizontalAlignment: HorizontalAlignment.Right
            verticalAlignment: VerticalAlignment.Center
            preferredWidth: 90.0
            textFit.maxFontSizeValue: 7.0
            textStyle.color: Color.LightGray
            textStyle.textAlign: TextAlign.Right
        }
        TextField {
            id: inputField
            text: "GET / HTTP/1.0"
            hintText: "Enter input to send to server"
            verticalAlignment: VerticalAlignment.Center
            textFit.maxFontSizeValue: 6.0
            preferredWidth: 375.0
        }
        Button {
            text: "Send"
            verticalAlignment: VerticalAlignment.Center
            horizontalAlignment: HorizontalAlignment.Right
            preferredWidth: 100.0

            onClicked: {
                _app.sendData(inputField.text)
                inputField.text = ""
            }
        }
    }
    Label {
        text: "=== Active session ==="
        horizontalAlignment: HorizontalAlignment.Fill
        textFit.maxFontSizeValue: 6.0
        textStyle.color: Color.Green
        textStyle.textAlign: TextAlign.Center
    }
    Container {
        layout: StackLayout {
            orientation: LayoutOrientation.LeftToRight
        }
        Label {
            text: "Cryptographic Cipher: "

            verticalAlignment: VerticalAlignment.Center
            textFit.maxFontSizeValue: 6.0
            textStyle.color: Color.LightGray
        }

        Label {
            // Displays cipher information
            // upon successful connection
            text: _app.cipher
            horizontalAlignment: HorizontalAlignment.Center
            verticalAlignment: VerticalAlignment.Center
            textFit.maxFontSizeValue: 5.0
            textStyle.color: Color.Red
        }
    }
    TextArea {
        // Displays connection response
        // after sending data
        text: _app.response
        horizontalAlignment: HorizontalAlignment.Fill
        verticalAlignment: VerticalAlignment.Fill
        textFit.maxFontSizeValue: 8.0

        editable: false
        textStyle.color: Color.Green
    }
}
