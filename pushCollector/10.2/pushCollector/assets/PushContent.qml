/*!
 * Copyright (c) 2012, 2013 Research In Motion Limited.
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

// The control that shows the content of a push object
Container {
    Container {
        horizontalAlignment: HorizontalAlignment.Fill
        topPadding: 10
        bottomPadding: 15
        leftPadding: 20

        layout: DockLayout {}
        background: Color.create("#cccccccc")

        Label {
            text: _pushAPIHandler.currentPushContent.pushDateTime
        }
    }

    Container {
        topPadding: 20
        leftPadding: 20
        rightPadding: 20

        TextArea {
            visible: (_pushAPIHandler.currentPushContent.contentType == "plainText")
            text: _pushAPIHandler.currentPushContent.textContent
            editable: false
            input{
                flags: TextInputFlag.SpellCheckOff
            }
        }

        ImageView {
            id: imageView

            visible: (_pushAPIHandler.currentPushContent.contentType == "image")
            image: _pushAPIHandler.currentPushContent.imageContent
        }

        WebView {
            visible: (_pushAPIHandler.currentPushContent.contentType == "richText")
            html: _pushAPIHandler.currentPushContent.textContent
        }
    }
}
