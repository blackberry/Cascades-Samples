import bb.cascades 1.0

//! [0]
NavigationPane {
    backButtonsVisible: false
    peekEnabled: true
    Page {
        titleBar: TitleBar {
            title: qsTr("Pick an Egg")
            dismissAction: ActionItem {
                title: qsTr("Cancel")
                onTriggered: {
                    _app.cardDone(qsTr("Boo! No eggs today.."));
                }
            }
        }
        Container {
            layout: DockLayout {
            }
            horizontalAlignment: HorizontalAlignment.Fill
            ImageButton {
                horizontalAlignment: HorizontalAlignment.Left
                verticalAlignment: VerticalAlignment.Top
                defaultImageSource: "asset:///images/egg.png"
                onClicked: {
                    _app.cardDone(qsTr("Yummy!"));
                }
            }
            ImageButton {
                horizontalAlignment: HorizontalAlignment.Center
                verticalAlignment: VerticalAlignment.Top
                defaultImageSource: "asset:///images/egg.png"
                onClicked: {
                    _app.cardDone(qsTr("Quack Quack.."));
                }
            }
            ImageButton {
                horizontalAlignment: HorizontalAlignment.Right
                verticalAlignment: VerticalAlignment.Top
                defaultImageSource: "asset:///images/egg.png"
                onClicked: {
                    _app.cardDone(qsTr("Mmmmmm.."));
                }
            }
            ImageButton {
                horizontalAlignment: HorizontalAlignment.Left
                verticalAlignment: VerticalAlignment.Center
                defaultImageSource: "asset:///images/egg.png"
                onClicked: {
                    _app.cardDone(qsTr("Delicio!"));
                }
            }
            ImageButton {
                horizontalAlignment: HorizontalAlignment.Center
                verticalAlignment: VerticalAlignment.Center
                defaultImageSource: "asset:///images/egg.png"
                onClicked: {
                    _app.cardDone(qsTr("Yuck!"));
                }
            }
            ImageButton {
                horizontalAlignment: HorizontalAlignment.Right
                verticalAlignment: VerticalAlignment.Center
                defaultImageSource: "asset:///images/egg.png"
                onClicked: {
                    _app.cardDone(qsTr("Whoa!"));
                }
            }
            ImageButton {
                horizontalAlignment: HorizontalAlignment.Left
                verticalAlignment: VerticalAlignment.Bottom
                defaultImageSource: "asset:///images/egg.png"
                onClicked: {
                    _app.cardDone(qsTr("Love it.."));
                }
            }
            ImageButton {
                horizontalAlignment: HorizontalAlignment.Center
                verticalAlignment: VerticalAlignment.Bottom
                defaultImageSource: "asset:///images/egg.png"
                onClicked: {
                    _app.cardDone(qsTr("Get it cooked already!"));
                }
            }
            ImageButton {
                horizontalAlignment: HorizontalAlignment.Right
                verticalAlignment: VerticalAlignment.Bottom
                defaultImageSource: "asset:///images/egg.png"
                onClicked: {
                    _app.cardDone(qsTr("Sunny side up"));
                }
            }
        }
    }
}
//! [0]
