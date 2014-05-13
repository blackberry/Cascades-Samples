/*!
 * Copyright (c) 2012, 2013  BlackBerry Limited.
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

// The page to configure the settings of the notification service

Page {
    id: sheetConfig

    // load the configuration values into the QML components
    function setValues() {
        appId.text = _pushAPIHandler.appId;
        ppgUrl.text = _pushAPIHandler.ppgUrl;
        pushInitiatorUrl.text = _pushAPIHandler.pushInitiatorUrl;
        publicPpgOption.selected = _pushAPIHandler.usePublicPpg;
        enterprisePpgOption.selected = ! _pushAPIHandler.usePublicPpg;
        launchAppOnPush.checked = _pushAPIHandler.launchApplicationOnPush;
        useSDKasPushInitiator.checked = _pushAPIHandler.pushInitiatorUrl != "";
    }

    // Signals for canceling, saving and refreshing values
    signal cancel()
    signal save()
    signal refresh()

    // Refreshes the configuration values to their last saved values
    onRefresh: {
        _pushAPIHandler.loadConfiguration();
        setValues();
    }

    // When this has finished being created, load the configuration values into the components
    onCreationCompleted: {
        setValues();
    }
    titleBar: TitleBar {
        id: addBar
        title: qsTr("Configuration")
        visibility: ChromeVisibility.Visible
        dismissAction: ActionItem {
            title: qsTr("Cancel")
            onTriggered: {
                // Cancel has been pressed so we emit cancel (will hide the sheet).
                sheetConfig.cancel();
            }
        }
        acceptAction: ActionItem {
            title: qsTr("Save")
            onTriggered: {
                // Emit save signal
                sheetConfig.save();
            }
        }
    }
    ScrollView {
        scrollViewProperties {
            scrollMode: ScrollMode.Vertical
        }
        Container {
            Container {
                topPadding: ui.du(3.3)
                leftPadding: ui.du(2.2)
                rightPadding: ui.du(2.2)
                bottomPadding: ui.du(1.1)
                DropDown {
                    horizontalAlignment: HorizontalAlignment.Center
                    title: qsTr("Mode")
                    enabled: true
                    Option {
                        id: publicPpgOption
                        text: qsTr("Public/BIS")
                        value: text
                        selected: true
                    }
                    Option {
                        id: enterprisePpgOption
                        text: qsTr("Enterprise/BDS")
                        value: text
                    }

                    // When a new option is selected determine which fields should be enabled
                    onSelectedIndexChanged: {
                        _pushAPIHandler.usePublicPpg = publicPpgOption.selected;
                    }
                }
            }
            Divider {
            }
            CustomCheckBox {
                id: useSDKasPushInitiator
                text: qsTr("Subscribe with Push Service SDK")
                onCheckedChanged: {
                    _pushAPIHandler.useSdk = checked
                }
            }
            Divider {
            }
            CustomTextField {
                id: appId
                description: qsTr("Application ID")
                inputMode: TextFieldInputMode.Text
                onTextChanged: {
                    _pushAPIHandler.appId = text;
                }
                textEnabled: (publicPpgOption.selected ? true : useSDKasPushInitiator.checked)
            }
            Divider {
            }
            CustomTextField {
                id: ppgUrl
                description: qsTr("PPG Url")
                inputMode: TextFieldInputMode.Url
                textEnabled: publicPpgOption.selected
                onTextChanged: {
                    _pushAPIHandler.ppgUrl = text;
                }
            }
            Divider {
            }
            CustomTextField {
                id: pushInitiatorUrl
                description: qsTr("Push Initiator Url")
                inputMode: TextFieldInputMode.Url
                textEnabled: useSDKasPushInitiator.checked
                onTextChanged: {
                    _pushAPIHandler.pushInitiatorUrl = text;
                }
            }
            Divider {
            }
            CustomCheckBox {
                id: launchAppOnPush
                text: qsTr("Launch Application on New Push")
                onCheckedChanged: {
                    _pushAPIHandler.launchApplicationOnPush = checked;
                }
            }
            Divider {
            }
        }
    }
}
