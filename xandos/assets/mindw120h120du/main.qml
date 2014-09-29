import bb.cascades 1.3
import bb.system 1.2

Page {
    //! [0]
    // global variable holding the user winning game count
    property int userScore: 0
    // global variable holding the droid winning game count
    property int droidScore: 0
    // global variable for enabling/disabling game interaction
    property bool gameEnabled: false
    
    property int gridMaxHeight: ui.du(83)
    property int gridMaxWidth: ui.du(83)
    
    property int spriteMaxHeight: ui.du(25)
    property int spriteMaxWidth: ui.du(25)
    
    property int spriteRowSpacing: (gridMaxHeight - (spriteMaxHeight*3))/2
    
    //! [0]
    Container {
        rightPadding: ui.du(10)
        leftPadding: ui.du(10)
        layout: DockLayout {
        }
        //! [1]
        Container {
            verticalAlignment: VerticalAlignment.Top
            horizontalAlignment: HorizontalAlignment.Fill
            
            // Container for displaying the user game win count
            Container {
                horizontalAlignment: HorizontalAlignment.Fill
                verticalAlignment: VerticalAlignment.Top
                layout: DockLayout {
                }
                Container {
                    horizontalAlignment: HorizontalAlignment.Left
                    verticalAlignment: VerticalAlignment.Top
                    layout: StackLayout {
                        orientation: LayoutOrientation.LeftToRight
                    }
                    Label {
                        text: "USER:"
                        textStyle.color: Color.White
                        textStyle.fontSize: FontSize.Large
                    }
                    Label {
                        id: user
                        text: "" + userScore
                        textStyle.color: Color.create("#40BB9E")
                        textStyle.fontSize: FontSize.Large
                    }
                }
                Container {
                    horizontalAlignment: HorizontalAlignment.Right
                    verticalAlignment: VerticalAlignment.Top
                    layout: StackLayout {
                        orientation: LayoutOrientation.LeftToRight
                    }
                    Label {
                        id: droid
                        text: "" + droidScore
                        textStyle.color: Color.create("#DF6054")
                        textStyle.fontSize: FontSize.Large
                    }
                    Label {
                        text: ":DROID"
                        textStyle.color: Color.White
                        textStyle.fontSize: FontSize.Large
                    }   
                }
            }
            // Container for displaying the droid game win count
        
        }
        //! [1]
        //! [2]
        // Container for displaying the 3x3 game grid
        Container {
            layout: DockLayout {
            }
            horizontalAlignment: HorizontalAlignment.Center
            verticalAlignment: VerticalAlignment.Center
            ImageView {
                id: gameGrid
                minWidth: gridMaxWidth
                minHeight: gridMaxHeight
                visible: !gameEnabled
                imageSource: "asset:///images/grid.png"
            }
            // the green grid is used when the droid is ready to play
            ImageView {
                minWidth: gridMaxWidth
                minHeight: gridMaxHeight
                visible: gameEnabled
                imageSource: "asset:///images/grid_start.png"
            }
        }
        //! [2]
        //! [3]
        // Container representing the first row of grid choices
        Container {
            minHeight: gameGrid.minHeight
            minWidth: gameGrid.minWidth
            horizontalAlignment: HorizontalAlignment.Center
            verticalAlignment: VerticalAlignment.Center
            Container {
                //minWidth: ui.du(26.6)
                layout: StackLayout {
                    orientation: LayoutOrientation.LeftToRight
                }
                ImageView {
                    id: i0
                    objectName: "0"
                    minWidth: spriteMaxWidth
                    minHeight: spriteMaxHeight
                    visible: gameEnabled
                    imageSource: "asset:///images/blank.png"
                    onTouch: {
                        // conditions to make sure that a selection can only be made
                        // by the user in grid cells that have not already been selected
                        if (event.isDown() && imageSource.toString().search("blank") != -1) {
                            imageSource = "asset:///images/x.png"
                            _xandos.select(0, 1)
                        }
                    }
                }
                ImageView {
                    id: i1
                    objectName: "1"
                    visible: gameEnabled
                    leftMargin: ui.du(8.3)
                    minWidth: spriteMaxWidth
                    minHeight: spriteMaxHeight
                    imageSource: "asset:///images/blank.png"
                    onTouch: {
                        if (event.isDown() && imageSource.toString().search("blank") != -1) {
                            imageSource = "asset:///images/x.png"
                            _xandos.select(1, 1)
                        }
                    }
                }
                ImageView {
                    id: i2
                    objectName: "2"
                    visible: gameEnabled
                    leftMargin: ui.du(8)
                    minWidth: spriteMaxWidth
                    minHeight: spriteMaxHeight
                    imageSource: "asset:///images/blank.png"
                    onTouch: {
                        if (event.isDown() && imageSource.toString().search("blank") != -1) {
                            imageSource = "asset:///images/x.png"
                            _xandos.select(2, 1)
                        }
                    }
                }
            }
            // Container representing the second row of grid choices
            Container {
                //minWidth: ui.du(26.6)
                topMargin: spriteRowSpacing
                layout: StackLayout {
                    orientation: LayoutOrientation.LeftToRight
                }
                ImageView {
                    id: i3
                    objectName: "3"
                    visible: gameEnabled
                    minWidth: spriteMaxWidth
                    minHeight: spriteMaxHeight
                    imageSource: "asset:///images/blank.png"
                    onTouch: {
                        if (event.isDown() && imageSource.toString().search("blank") != -1) {
                            imageSource = "asset:///images/x.png"
                            _xandos.select(3, 1)
                        }
                    }
                }
                ImageView {
                    id: i4
                    objectName: "4"
                    visible: gameEnabled
                    leftMargin: ui.du(8.3)
                    minWidth: spriteMaxWidth
                    minHeight: spriteMaxHeight
                    imageSource: "asset:///images/blank.png"
                    onTouch: {
                        if (event.isDown() && imageSource.toString().search("blank") != -1) {
                            imageSource = "asset:///images/x.png"
                            _xandos.select(4, 1)
                        }
                    }
                }
                ImageView {
                    id: i5
                    objectName: "5"
                    visible: gameEnabled
                    leftMargin: ui.du(8)
                    minWidth: spriteMaxWidth
                    minHeight: spriteMaxHeight
                    imageSource: "asset:///images/blank.png"
                    onTouch: {
                        if (event.isDown() && imageSource.toString().search("blank") != -1) {
                            imageSource = "asset:///images/x.png"
                            _xandos.select(5, 1)
                        }
                    }
                }
            }
            // Container representing the third row of grid choices
            Container {
                //minWidth: ui.du(26.6)
                topMargin: spriteRowSpacing
                layout: StackLayout {
                    orientation: LayoutOrientation.LeftToRight
                }
                ImageView {
                    id: i6
                    objectName: "6"
                    visible: gameEnabled
                    minWidth: spriteMaxWidth
                    minHeight: spriteMaxHeight
                    imageSource: "asset:///images/blank.png"
                    onTouch: {
                        if (event.isDown() && imageSource.toString().search("blank") != -1) {
                            imageSource = "asset:///images/x.png"
                            _xandos.select(6, 1)
                        }
                    }
                }
                ImageView {
                    id: i7
                    objectName: "7"
                    visible: gameEnabled
                    leftMargin: ui.du(8.3)
                    minWidth: spriteMaxWidth
                    minHeight: spriteMaxHeight
                    imageSource: "asset:///images/blank.png"
                    onTouch: {
                        if (event.isDown() && imageSource.toString().search("blank") != -1) {
                            imageSource = "asset:///images/x.png"
                            _xandos.select(7, 1)
                        }
                    }
                }
                ImageView {
                    id: i8
                    objectName: "8"
                    visible: gameEnabled
                    leftMargin: ui.du(8)
                    minWidth: spriteMaxWidth
                    minHeight: spriteMaxHeight
                    imageSource: "asset:///images/blank.png"
                    onTouch: {
                        if (event.isDown() && imageSource.toString().search("blank") != -1) {
                            imageSource = "asset:///images/x.png"
                            _xandos.select(8, 1)
                        }
                    }
                }
            }
        }
        //! [3]
    }
    attachedObjects: [
        //! [5]
        // System dialog that questions user if to start the game at all
        SystemDialog {
            id: dialog
            title: qsTr("Start Game of XandOs?")
            body: qsTr("The droid is ready when the grid turns green")
            confirmButton.label: qsTr("Start")
            confirmButton.enabled: true
            cancelButton.label: qsTr("Cancel")
            cancelButton.enabled: true
            onFinished: {
                var x = result;
                if (x == SystemUiResult.ConfirmButtonSelection) {
                    // start droid and make the necessary signal/slot connections
                    console.log("confirm");
                    _xandos.startDroid();
                    _xandos.won.connect(playerWon);
                    _xandos.droidReady.connect(droidStart)
                } else if (x == SystemUiResult.CancelButtonSelection) {
                    console.log("cancel");
                    Application.requestExit()
                }
            }
        },
        //! [5]
        //! [6]
        SystemDialog {
            // System dialog that represents the game result (win,loss, or tie) and
            // queries user if they wish to play again.
            id: gameResult
            title: qsTr("")
            body: qsTr("Would you like to play again?")
            confirmButton.label: qsTr("Yes")
            confirmButton.enabled: true
            cancelButton.label: qsTr("No")
            cancelButton.enabled: true
            onFinished: {
                var x = result;
                if (x == SystemUiResult.ConfirmButtonSelection) {
                    // Do the necessary functions to clear/reset the game state
                    // start a new droid and make the necessary signal/slot connections
                    console.log("confirm");
                    clearGrid()
                    _xandos.resetGame();
                    _xandos.startDroid();
                    _xandos.won.connect(playerWon);
                    _xandos.droidReady.connect(droidStart)
                } else if (x == SystemUiResult.CancelButtonSelection) {
                    console.log("cancel");
                    Application.requestExit()
                }
            }
        }
        //! [6]
    ]
    onCreationCompleted: {
        dialog.show();
    }
    
    //! [7]
    // Functions to adjust the the game result dialog based
    // on the ending game state
    function playerWon(x) {
        gameEnabled = false;
        console.log("winner is: " + x)
        _xandos.won.disconnect(playerWon);
        if(-1 == x) {
            droidScore += 1
            gameResult.title = qsTr("Droid has WON!")
        } else if(1 == x) {
            gameResult.title = qsTr("You have WON!")
            userScore += 1
        } else {
            gameResult.title = qsTr("It's a TIE!")
        }
        gameResult.show()
    }
    //! [7]
    
    //! [8]
    // Convenience function to reset the grid
    // back to pre game state.
    function clearGrid() {
        i0.imageSource = "asset:///images/blank.png"
        i1.imageSource = "asset:///images/blank.png"
        i2.imageSource = "asset:///images/blank.png"
        i3.imageSource = "asset:///images/blank.png"
        i4.imageSource = "asset:///images/blank.png"
        i5.imageSource = "asset:///images/blank.png"
        i6.imageSource = "asset:///images/blank.png"
        i7.imageSource = "asset:///images/blank.png"
        i8.imageSource = "asset:///images/blank.png"
    }
    //! [8]
    //! [9]
    // function to enable/disable game pending the
    // droid status.
    function droidStart() {
        gameEnabled = true
    }
    //! [9]
}