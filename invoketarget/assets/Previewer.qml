import bb.cascades 1.0
//! [0]
NavigationPane {
    backButtonsVisible: true
    peekEnabled: true
    Page {
        titleBar: TitleBar {
            title: qsTr("Previewer")
        }
        Container {
            horizontalAlignment: HorizontalAlignment.Fill
            ImageView {
                imageSource: _app.uri
                scalingMethod: ScalingMethod.AspectFit
            }
        }
    }
}
//! [0]
