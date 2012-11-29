import bb.cascades 1.0

// This is the file used for the SceneCover shown when the application
// is running in minimized mode on the running apps page.
// Currently to use a SceneCover it has to be regitered from C++
// see src/cascadescookbookapp.cpp (registerQMLTypes)

SceneCover {
    // In the SceneCover the content property keyword has to be explicitly added
    content: Container {
        Container {
            layout: DockLayout {
            }
            background: Color.Black
            ImageView {
                imageSource: "asset:///images/application-cover.png"
            }

            // A title is put on a dark background, it is pixelaligned in order
            // to get it to match the checkerd pattern in the background image.
            Container {
                bottomPadding: 31
                horizontalAlignment: HorizontalAlignment.Center
                verticalAlignment: VerticalAlignment.Bottom

                Container {
                    preferredWidth: 84
                    preferredHeight: 42
                    background: Color.create("#121212")
                    layout: DockLayout {
                    }
                    Label {
                        horizontalAlignment: HorizontalAlignment.Center
                        verticalAlignment: VerticalAlignment.Center
                        text: "QML"
                        textStyle.color: Color.create("#ebebeb")
                        textStyle.fontSize: FontSize.PointValue
                        textStyle.fontSizeValue: 6
                    }
                }
            }
        }
    }
}
