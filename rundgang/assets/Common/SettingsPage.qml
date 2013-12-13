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

// A page used to alter application settings, it use the global settings object
// to persist the settings between application runs (see src/common/globalsettings.cpp/.h)
Page {
    id: settingsPage

    titleBar: TitleBar {
        title: qsTr("Settings") + Retranslate.onLocaleOrLanguageChanged
    }

    ScrollView {
        Container {
            
            // Photo settings
            Header {
                title: qsTr("Photo feedback settings") + Retranslate.onLocaleOrLanguageChanged
            }

            ToggleComponent {
                description: qsTr("Send low resolution images") + Retranslate.onLocaleOrLanguageChanged
                checked: _appSettings.loresPhoto

                onCheckedChanged: {
                    // Change the photo quality settings.
                    if (_appSettings.loresPhoto != checked) {
                        _appSettings.loresPhoto = checked;
                    }
                }
            }

            // Text feedback settings
            Header {
                title: qsTr("Text feedback settings") + Retranslate.onLocaleOrLanguageChanged
            }

            ToggleComponent {
                description: qsTr("Trigger email directly") + Retranslate.onLocaleOrLanguageChanged
                checked: _appSettings.directTextEmail

                onCheckedChanged: {
                    // Change the directEmail on the global settings object.
                    if (_appSettings.directTextEmail != checked) {
                        _appSettings.directTextEmail = checked;
                    }
                }
            }

            // Email settings.
            Header {
                title: qsTr("Email settings") + Retranslate.onLocaleOrLanguageChanged
            }

            ToggleComponent {
                id: emailSignatureToggle
                description: qsTr("Auto Signature") + Retranslate.onLocaleOrLanguageChanged
                checked: _appSettings.autoEmailSignature

                onCheckedChanged: {
                    // Change the autoEmailSiganture on the global settings object.
                    if (_appSettings.autoEmailSignature != checked) {
                        _appSettings.autoEmailSignature = checked;
                    }
                }
            }

            // TextField for entering a simple email signature.
            Container {
                leftPadding: 20
                rightPadding: 20

                TextField {
                    enabled: emailSignatureToggle.checked
                    text: _appSettings.emailSignature
                    hintText: qsTr("Enter email signature") + Retranslate.onLocaleOrLanguageChanged

                    onTextChanged: {
                        _appSettings.emailSignature = text;
                    }
                }
            }
        }
    }
}