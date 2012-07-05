/* Copyright (c) 2012 Research In Motion Limited.
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/

import bb.cascades 1.0
import com.sample.payment 1.0

// Standard TabbedPane to provide the various payment screens
TabbedPane {
    // property used for the traversal of each receipt string character
    property int i: 0
    
    // property holding the receipt string for display
    property string receiptArea: ""
    
    Tab {
        title: "MAIN"
	    Page {
	        
	        // Container for oraganizing the different visual components
	        content: Container {
	            background: Color.Black
	            layout: StackLayout {
	                rightPadding: 10
	                leftPadding: 10
	            }
	            // Container for holding the title
	            Container {
	                layout: StackLayout {
	                    rightPadding: 100
	                }
	                layoutProperties: StackLayoutProperties {
	                    horizontalAlignment: HorizontalAlignment.Right
	                    verticalAlignment: VerticalAlignment.Top
	                    spaceQuota: -1
	                }
	                
	                // A standard ImageView displaying the title
	                ImageView {
	                    topMargin: 100
	                    layoutProperties: StackLayoutProperties {
	                        horizontalAlignment: HorizontalAlignment.Right
	                        verticalAlignment: VerticalAlignment.Top
	                    }
	                    imageSource: "asset:///images/goods_title.png"
	                    preferredWidth: 200
	                    preferredHeight: 100
	                }
	            }
	            
	            // Container for holding the item iamges
	            Container {
	                id: imagine
	                layout: StackLayout {
	                    layoutDirection: LayoutDirection.LeftToRight
	                    topPadding: 40
	                    rightPadding: 40
	                }
	                layoutProperties: StackLayoutProperties {
	                    horizontalAlignment: HorizontalAlignment.Right
	                    verticalAlignment: VerticalAlignment.Top
	                    spaceQuota: -1
	                }
	                
	                // A standard ImageView
	                ImageView {
	                    imageSource: "asset:///images/banana.png"
	                    preferredWidth: 128
	                    preferredHeight: 86
	                    rightMargin: 30
	                }
	                
	                // A standard ImageView
	                ImageView {
	                    imageSource: "asset:///images/leek.png"
	                    preferredWidth: 128
	                    preferredHeight: 86
	                    rightMargin: 30
	                }
	                
	                // A standard ImageView
	                ImageView {
	                    imageSource: "asset:///images/strawberries.png"
	                    preferredWidth: 128
	                    preferredHeight: 86
	                }
	            }
	            
	            // Container for displaying the Divider
	            Container {
	                topMargin: 20
	                bottomMargin: 20
	                layoutProperties: StackLayoutProperties {
	                    spaceQuota: -1
	                }
	                background: Color.create ("#615A5F")
	                Divider {
	                }
	            }
	            
	            // Container for the application control Button's
	            Container {
	                layout: StackLayout {
	                    layoutDirection: LayoutDirection.LeftToRight
	                    topPadding: 40
	                    rightPadding: 40
	                }
	                layoutProperties: StackLayoutProperties {
	                    horizontalAlignment: HorizontalAlignment.Center
	                    spaceQuota: -1
	                }
	                translationX: 1000
	                
	                // A standard Button
//! [0]
	                Button {
	                    text: "BUY"
	                    // Perform the purchase transaction on click
	                    onClicked: {
	                        // Reset some old values
	                        receipt.text = ""
	                        message.visible = false
	                        if ( paymentControl.id.length != 0 )
	                            paymentControl.purchase (paymentControl.id, paymentControl.sku, paymentControl.name, paymentControl.metadata)
	                    }
	                }
	                
	                // A standard Button
	                Button {
	                    text: "SUB TERMS"
	                    
	                    // Display the subscription terms on click
	                    onClicked: {
	                        if ( paymentControl.id.length != 0 )
	                            paymentControl.getSubscriptionTerms (paymentControl.id, paymentControl.sku)
	                    }
	                }
//! [0]
	                // Animation for sliding button effect
	                animations: [
	                    TranslateTransition {
	                        id: showButtons
	                        toX: 0
	                        duration: 1000
	                    }
	                ]
	            }
	            
	            // Container for the ListView
	            Container {
	                layoutProperties: StackLayoutProperties {
	                    spaceQuota: 3
	                }
	                translationY: 1000
//! [1]	                
	                // A standard ListView
	                ListView {
	                    // The data model to use for propagating the list
	                    dataModel: XmlDataModel {
	                        source: "models/goods.xml"
	                    }
	                    // The component to use for data items
	                    listItemComponents: [
	                        ListItemComponent {
	                            type: "digitalgood"
	                            DigitalGood {
	                            }
	                        }
	                    ]
	                    
	                    // Retrieve and save the selected items data
	                    onSelectionChanged: {
	                        if (selected) {
	                            // Reset some old values
	                            receipt.text = ""
	                            message.visible = false
	                            resetSubscriptionText()
	                            var chosenItem = dataModel.data (indexPath)
	                            paymentControl.id = chosenItem.id
	                            paymentControl.sku = chosenItem.sku
	                            paymentControl.name = chosenItem.name
	                            paymentControl.metadata = chosenItem.metadata
	                            paymentControl.getPrice (paymentControl.id, paymentControl.sku)
	                        }
	                    }
	                }
//! [1]
	                // Animation for sliding list effect upon creation
	                animations: [
	                    TranslateTransition {
	                        id: list
	                        toY: 0
	                        duration: 1000
	                    }
	                ]
	            }
	            
	            // Container for the price/check subscription labels
	            Container {
	                layout: DockLayout {
	                }
	                layoutProperties: StackLayoutProperties {
	                    horizontalAlignment: HorizontalAlignment.Fill
	                }
	                // A standard Label for displaying item price
	                Label {
	                    id: priceLabel
	                    layoutProperties: DockLayoutProperties {
	                        horizontalAlignment: HorizontalAlignment.Left
	                        verticalAlignment: VerticalAlignment.Center
	                    }
	                    pivotY: preferredHeight / 2
	                    text: "$$$"
	                    textStyle {
	                        base: SystemDefaults.TextStyles.BodyText
	                        color: Color.create ("#E138AA")
	                    }
	                    animations: [
	                         // A sequential animation is used to trigger consequtive rotations of
	                        // the image to make it wiggle.
	                        SequentialAnimation {
	                            id: priceAnim
	                            animations: [
	                                RotateTransition {
	                                    toAngleZ: 15
	                                    duration: 700 * 0.4
	                                    delay: 200
	                                },
	                                RotateTransition {
	                                    toAngleZ: -5
	                                    duration: 700 * 0.3
	                                },
	                                RotateTransition {
	                                    toAngleZ: 0
	                                    duration: 700 * 0.3
	                                }
	                            ]
	                        }
	                    ]
	                }
	                
	                // A standard Label used as a palceholder for specific function initiation
//! [2]
			Label {
	                    id: subscription
	                    layoutProperties: DockLayoutProperties {
	                        horizontalAlignment: HorizontalAlignment.Right
	                        verticalAlignment: VerticalAlignment.Center
	                    }
	                    text: "CHECK subscription"
	                    textStyle.color: Color.White
	                    
	                    // Check the item subscription and display on touch
	                    onTouch: {
	                        if ( paymentControl.id.length != 0 )
	                            paymentControl.checkSubscriptionStatus (paymentControl.id, paymentControl.sku)
	                    }
	                }
//! [2]
	            }
	            // Container for displaying various user messages/results
	            MessageBox {
	                id: message
	                visible: false
	                layoutProperties: StackLayoutProperties {
	                    horizontalAlignment: HorizontalAlignment.Center
	                    verticalAlignment: VerticalAlignment.Center
	                }
	                // remove message box on touch
	                onTouch: message.visible = false
	            }
	            
	            // Container for displaying the receipt text area
	            Container {
	                layoutProperties: StackLayoutProperties {
	                    horizontalAlignment: HorizontalAlignment.Fill
	                    verticalAlignment: VerticalAlignment.Top
	                    spaceQuota: 4
	                }
	                layout: DockLayout { }
	                Container {
	                   background: Color.White
	                   // A standard TextArea
	                   TextArea {
	                       id: receipt
	                       editable: false
	                       hintText: ""
	                   }
	                }
	            }
	            
	            // QTimer fired for every receipt string character
	            // to animate message typing effect
	            attachedObjects: [
	                QTimer {
	                    id: timer1
	                    singleShot: false
	                    onTimeout: {
	                        animTextAreaText ();
	                        // incremented for each character
	                        i ++;
	                    }
	                }
	            ]
//! [3]	            
	            // The custom payment element for holding item data
	            // and used for method invocation and receiving events as a
	            // result of those invocations
	            PaymentServiceControl {
	                id: paymentControl
	                property string id
	                property string sku
	                property string name
	                property string metadata
	                onPriceResponseSuccess: {
	                    // Play price request animation on response
	                    priceLabel.text = price;
	                    priceAnim.play ()
	                }
	                
	                // Play receipt animation on purchase response
	                onPurchaseResponseSuccess: {
	                    i = 0
	                    receiptArea = receiptString
	                    timer1.start (50);
	                }
	                onExistingPurchasesResponseSuccess: {
	                    purchases.text = receiptsString
	                }
	                onSubscriptionTermsResponseSuccess: {
	                    message.text = "Price: " + price + " \nInitialPeriod: " + initialPeriod + " \nRenewalPrice: " + renewalPrice + " \nRenewalPeriod: " + renewalPeriod
	                    message.visible = true
	                }
	                onCancelSubscriptionResponseSuccess: {
	                    cancellations.text = "Canceled? " + cancelled
	                }
	                onCheckStatusResponseSuccess: {
	                    subscription.textStyle.color = Color.Green
	                    subscription.text = "Active?" + status
	                }
	                onInfoResponseError: {
	                                            //outputArea.text = "Failed to get price. " + errorCode + " " + errorText;
	                }
	            }
//! [3]
	        }
	        // Animate various visual components on creation of this page 
	        onCreationCompleted: animScreen ()
	    }
    }
    
    // Play the list/Button animations
    function animScreen () {
        list.play ();
        showButtons.play ();
    }
    
    // Check the receipt length and stop animation on reaching condition
    function animTextAreaText () {
        receipt.text += receiptArea.charAt (i);
        if (i >= receiptArea.length) {
            timer1.stop ();
        }
    }
    
    // Reset the look of the subscription Label
    function resetSubscriptionText() {
        subscription.textStyle.color = Color.White
        subscription.text = "CHECK subscription"
    }
    
    Tab {
        title: "Purchases"
	    // Page for displaying purchases made data
	    Page {
	        content: Container {
	            
	            // A standard Button
//! [4]
	            Button {
	                text: "PURCHASES"
	                
	                // Retrieve purchases made data on click
	                onClicked: paymentControl.getExisting (false);
	            }
//! [4]	            
	            // A standard TextArea for displaying retrieved purchases data
	            TextArea {
	                id: purchases
	                editable: false
	            }
	        }
	    }
    }
    
    Tab {
        title: "Cancellation"
        // Page for cancelling subscription's
        Page {
            content: Container {
                // A standard Label
                Label {
                    text: "Purchase ID"
                    textStyle {
                        base: SystemDefaults.TextStyles.TitleText
                        fontWeight: FontWeight.Bold
                    }
                }
            
                // A standard TextField for purchase id input
                TextField {
                    id: purchaseId
                }
            
                // A standard Button
//! [5]
                Button {
                    text: "Cancel"
                
                    // Cancel entered purchase on click
                    onClicked: {
                        if (purchaseId.text.length > 0) paymentControl.cancelSubscription (purchaseId.text)
                    }
                }
//! [5]
            
                // A standard TextArea for displaying the cancellation results
                TextArea {
                    id: cancellations
                    editable: false
                    backgroundVisible: false
                }
            }
        }
    }
}
