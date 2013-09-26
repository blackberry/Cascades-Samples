import bb.cascades 1.0
import bb.utility 1.0
import bb.cascades.pickers 1.0

Page {
    actionBarVisibility: ChromeVisibility.Overlay
    
    Container {
        horizontalAlignment: HorizontalAlignment.Fill
        verticalAlignment: VerticalAlignment.Fill
        ImageView {
            id: imageView
            scalingMethod: ScalingMethod.AspectFit
            horizontalAlignment: HorizontalAlignment.Center
            verticalAlignment: VerticalAlignment.Center
            attachedObjects: [
                ExifReader {
                    id: exifReader
                    onRotationChanged: {
                        imageView.rotationZ = rotation
                        imageView.visible = true
                    }
                }
            ]
            image: exifReader.image
            maxHeight: (exifReader.rotation == 270 || exifReader.rotation == 90) ? layout.layoutFrame.width : layout.layoutFrame.height
            maxWidth: (exifReader.rotation == 270 || exifReader.rotation == 90) ? layout.layoutFrame.height : layout.layoutFrame.width
            minHeight: maxHeight
            minWidth: maxWidth
            
            // adjust layout for image rotation
            translationY: (exifReader.rotation == 270 || exifReader.rotation == 90) ? (layout.layoutFrame.height - layout.layoutFrame.width)/2 : 0
        }
        attachedObjects: LayoutUpdateHandler {
            id: layout
        }
    }
    actions: [
        ActionItem {
            title: qsTr("select_an_image") + Retranslate.onLocaleOrLanguageChanged
            imageSource: "asset:///ic_open.png"
            ActionBar.placement: ActionBarPlacement.OnBar
            onTriggered:picker.open()
            attachedObjects: [
                FilePicker {
                    id: picker
                    mode: FilePickerMode.Picker
                    type: FileType.Picture
                    onFileSelected: {
                        var src = "file://" + selectedFiles[0]
                        if (exifReader.imageSource == src)
                        	return
                        
                        imageView.visible = false
                        imageView.imageSource = exifReader.imageSource = src
                    }
                }
            ]
        }
    ]
}
