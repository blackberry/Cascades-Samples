import bb.cascades 1.2
import com.netimage 1.0

Page {
    id: pageRoot
    property variant chosenItem
    property variant listImageManager
    property NavigationPane nav: null

    Container {
        horizontalAlignment: HorizontalAlignment.Fill
        ImageView {
            id: itemImage
            layoutProperties: StackLayoutProperties {
                spaceQuota: 1
            }
            bottomMargin: 0
            horizontalAlignment: HorizontalAlignment.Fill
            scalingMethod: ScalingMethod.AspectFill
            accessibility.name: "Article image"
            accessibility.description: ""

            // This is where we get the image as soon as it's downloaded from the internet via the netimagetracker
            image: tracker.image

            attachedObjects: [
                // The NetImageTracker is a custom ImageTracker with the ability to
                // load images from the internet (see netimagetracker.h/.cpp).
                NetImageTracker { // TODO:: replace with better image cache
                    id: tracker

                    // The image source is set
                    source: chosenItem.imageSource

                    // In this sample the same manager is used for all list item trackers,
                    // it handles the networked requests and contain the image cache (see netimagetracker.cpp/.h).
                    // Note that if managers with many different cacheIds are used you have to be
                    // careful not to use up to much disk space for the different caches.
                    manager: feedImageManager

                },
                NetImageManager { // TODO:: replace with better image cache
                    id: feedImageManager
                    cacheId: "feedImageManager"
                }
            ]
        }
        Container {
            horizontalAlignment: HorizontalAlignment.Fill
            preferredHeight: 4
            background: Color.create("#0394c3")
        }
        Container {
            topPadding: 20
            bottomPadding: 20
            leftPadding: 20
            rightPadding: 20
            background: Color.create("#333333")
            horizontalAlignment: HorizontalAlignment.Fill
            Label {
                id: titelLabel
                text: chosenItem.title
                textStyle.base: SystemDefaults.TextStyles.TitleText
                multiline: true
            }

            Label {
                // We render as HTML to ensure we don't display gibberish.
                id: descriptionLabel
                text: "<html><div>" + chosenItem.description + "</div></html>"
                textStyle.base: SystemDefaults.TextStyles.BodyText
                multiline: true
            }

            Label {
                id: writerLabel
                text: (chosenItem)["dc:creator"]
                textStyle.base: SystemDefaults.TextStyles.SmallText
                horizontalAlignment: HorizontalAlignment.Right
            }
        }
    }

    actions: [
        InvokeActionItem {
            objectName: "readMore"
            id: readMore
            title: qsTr("Read More")
            ActionBar.placement: ActionBarPlacement.OnBar
            imageSource: "file:///usr/share/icons/bb_action_open.png"
            query {
                invokeTargetId: "sys.browser"
                // A valid URI is needed for the action item to appear.
                // We will change the URI when an item is selected.
                uri: "http://www.blackberry.com/" // TODO:: replace with real url?
                // We have to tell it to tell it to watch for updates,
                // since the URI is dynamic (see the ListView onTriggered).
                onQueryChanged: readMore.query.updateQuery()
            }
        },
        InvokeActionItem {
            objectName: "shareArticle"
            id: shareArticle
            title: qsTr("Share Article")
            ActionBar.placement: ActionBarPlacement.InOverflow
            query {
                mimeType: "text/plain"
                data: "..." // TODO:: replace with real text and link?
                invokeActionId: "bb.action.SHARE"
            }
            onTriggered: {
                // First, convert unicode string before calling invoke; the
                // receiver app will have to call decodeURIComponent(escape(str))
                // to get the unicode string back. BBM, Browser, etc. all do this.
                //
                // The replacements here convert any numeric entities in the
                // form of &#032; into their proper characters, to have more
                // WYSIWYG output when sharing. Note that this doesn't clean
                // up HTML entities such as &amp; and &quot; nor does it clean
                // up HTML tags such as <a> and <p>.
                // Those are exercises for the reader, as Cascades doesn't do it.
                data = unescape(encodeURIComponent(chosenItem.title.replace(/&#(\d*);/g, function (m, c) {
                                return String.fromCharCode(c)
                            }) + ": " + chosenItem.description.replace(/&#(\d*);/g, function (m, c) {
                                return String.fromCharCode(c)
                            }) + " " + chosenItem.link))
            }
        }
    ]
}
