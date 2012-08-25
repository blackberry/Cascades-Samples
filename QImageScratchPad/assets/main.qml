import bb.cascades 1.0

Page {
    content: Container {
        layout: StackLayout {}
        
        ImageView {
            id: finalImage
            objectName: "finalImage"
            preferredHeight: 500
            preferredWidth:  700
            layoutProperties: StackLayoutProperties {
                horizontalAlignment: HorizontalAlignment.Center
            }
        }

        // Container with images to select
        Container {
            layout: StackLayout {
                layoutDirection: LayoutDirection.LeftToRight
            }
            
            Button {
                objectName: "img1"
                imageSource: "asset:///images/Blue_20Nose_20Thumb.png"
            }
            Button {
                objectName: "img2"
                imageSource: "asset:///images/Zeppelin_Thumb.png"
            }
            Button {
                objectName: "img3"
                imageSource: "asset:///images/warning.png"
            }
            Button {
                objectName: "circle"
            }
            Button {
                objectName: "square"
            }
	    }
	    Container {
		    Container {
		        layout: StackLayout {}
                TextField {
                    objectName: "numRows"
                    hintText: "Enter # rows (1 or more) of replicants"
                }
                TextField {
                    objectName: "numCols"
                    hintText: "Enter # columns (1 or more) of replicants"
                }
            }
	    }
	        
	    // vertical spacer
	    // Is about the size of the keyboard
	    Container {}
	}
}
