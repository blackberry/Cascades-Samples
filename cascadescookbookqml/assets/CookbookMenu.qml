/* Copyright (c) 2013 BlackBerry Limited.
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
import bb.cascades 1.2
import bb.system 1.2

MenuDefinition {
    settingsAction: SettingsActionItem {
        onTriggered: {
            toast.body = "Settings"
            toast.show();
        }
    }
    helpAction: HelpActionItem {
        onTriggered: {
            toast.body = "Help"
            toast.show();
        }
    }
    actions: [
        ActionItem {
            title: "Add Salt"
            imageSource: "asset:///images/salt_icon.png"
            onTriggered: {
                toast.body = "Salt Added"
                toast.show();
            }
        }
    ]

    attachedObjects: [
        SystemToast {
            id: toast
            body: "Toasty"
        }
    ]
}
