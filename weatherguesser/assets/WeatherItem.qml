import bb.cascades 1.0

// A custom item used in the list on the WeatherPage.
Container {
    id: weatherItem
    layout: DockLayout {
    }

    ImageView {
        imageSource: {
            if (ListItemData.icon == undefined) {
                "asset:///images/icons/small/0.png";
            } else {
                "asset:///images/icons/small/" + ListItemData.icon + ".png";
            }
        }
        preferredWidth: 768
        preferredHeight: 201
    }

    Container {
        layout: StackLayout {
            leftPadding: 77
        }

        layoutProperties: DockLayoutProperties {
            verticalAlignment: VerticalAlignment.Center
        }

        Label {
            text: ListItemData.tempaverage + "\u00B0"
            textStyle {
                base: SystemDefaults.TextStyles.BigText
                color: Color.White
                size: 100
                fontWeight: FontWeight.Normal
            }
        }
    }
}
