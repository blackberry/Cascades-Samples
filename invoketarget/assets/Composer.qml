import bb.cascades 1.3
//! [0]
NavigationPane {
    backButtonsVisible: false
    peekEnabled: true
    Page {
        titleBar: TitleBar {
            title: qsTr("Composer")
            acceptAction: ActionItem {
                title: qsTr("Share")
                onTriggered: {
                    _app.cardDone(qsTr("Content Shared."));
                }
            }
            dismissAction: ActionItem {
                title: qsTr("Cancel")
                onTriggered: {
                    _app.cardDone(qsTr("Dont feel like sharing today?"));
                }
            }
        }
        Container {
            horizontalAlignment: HorizontalAlignment.Fill
            Header {
                title: qsTr("Share via Sample Composer")
            }
            Container {
                horizontalAlignment: HorizontalAlignment.Fill
                TextArea {
                    horizontalAlignment: HorizontalAlignment.Center
                    preferredHeight: ui.du(44.4)
                    text: _app.data
                    onCreationCompleted: {
                        requestFocus();
                    }
                }
            }
        }
    }
}
//! [0]
