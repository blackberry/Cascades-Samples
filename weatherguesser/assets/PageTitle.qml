import bb.cascades 1.0

// A common title text that is used on many of the tab pages.

Container {
    property alias titleText: titleLabel.text

    layout: StackLayout {
        topPadding: 10
        bottomPadding: topPadding
    }

    Label {
        id: titleLabel
        text: "Title text"
        bottomMargin: 0

        textStyle {
            base: SystemDefaults.TextStyles.BigText
            fontWeight: FontWeight.Light
            fontStyleHint: FontStyleHint.Serif
            color: Color.White
        }

        layoutProperties: StackLayoutProperties {
            horizontalAlignment: HorizontalAlignment.Center
            verticalAlignment: VerticalAlignment.Center
        }
    }
}
