import bb.cascades 1.2

Container {
    layout: DockLayout {}
    property alias text: mainLabel.text
    property variant textStyleBase
    property int maxLineCount

	// simulates a dropdown on the label. Could be removed for performance optimization when using large amounts of list items (at the cost of less readable text).	
    Label {
        text: mainLabel.text
        textStyle.base: textStyleBase
        autoSize.maxLineCount: maxLineCount
        textStyle.color: Color.Black
        translationY: 2
        multiline: true
        accessibilityMode: A11yMode.Collapsed
    }
    Label {
        id: mainLabel
        textStyle.color: Color.White
        multiline: true
        textStyle.base: textStyleBase
        autoSize.maxLineCount: maxLineCount
        accessibilityMode: A11yMode.Collapsed
    }

    accessibility: CustomA11yObject {
        role: A11yRole.Label
        labelledBy: mainLabel
        description: mainLabel.text
    }

}
