/* Copyright (c) 2012 BlackBerry Limited.
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
import bb.cascades 1.2

// A RadioGroup QML component used by the stock curve recipe.
// This component contains 11 options that are set by defining the
// stockCurveNames variant list (these names are used to set the option image).
Container {
    id: stockCurveRadioGroupContainer

    // The curves used for storing a different StockCurve in a property for each option
    property alias stockCurves: stockCurveRadioGroup.curves

    // The name of the curve
    property alias stockCurveNames: stockCurveRadioGroup.names

    // A title put on top of the RadioGroup
    property alias title: titleLabel.text

    // When a option is selected, a signal is emitted by this component.
    signal selectedCurveChanged (variant curve, string name)

    topPadding: 20    

    // The Component title
    Label {
        id: titleLabel
        horizontalAlignment: HorizontalAlignment.Center
        textStyle.base: SystemDefaults.TextStyles.SmallText
    }

    // The RadioGroup presenting the different curves
    RadioGroup {
        id: stockCurveRadioGroup
        property variant curves: null
        property variant names: null

        Option {
            // This is the images presented with each radio option. These images help make it
			// possible to see the difference in timing behavior of different StockCurves.
            imageSource: "asset:///images/stockcurve/icon_" + stockCurveRadioGroup.names[0] + ".png"
        }
        Option {
            imageSource: "asset:///images/stockcurve/icon_" + stockCurveRadioGroup.names[1] + ".png"
        }
        Option {
            imageSource: "asset:///images/stockcurve/icon_" + stockCurveRadioGroup.names[2] + ".png"
        }
        Option {
            imageSource: "asset:///images/stockcurve/icon_" + stockCurveRadioGroup.names[3] + ".png"
        }
        Option {
            imageSource: "asset:///images/stockcurve/icon_" + stockCurveRadioGroup.names[4] + ".png"
        }
        Option {
            imageSource: "asset:///images/stockcurve/icon_" + stockCurveRadioGroup.names[5] + ".png"
        }
        Option {
            imageSource: "asset:///images/stockcurve/icon_" + stockCurveRadioGroup.names[6] + ".png"
        }
        Option {
            imageSource: "asset:///images/stockcurve/icon_" + stockCurveRadioGroup.names[7] + ".png"
        }
        Option {
            imageSource: "asset:///images/stockcurve/icon_" + stockCurveRadioGroup.names[8] + ".png"
        }
        Option {
            imageSource: "asset:///images/stockcurve/icon_" + stockCurveRadioGroup.names[9] + ".png"
        }
        Option {
            imageSource: "asset:///images/stockcurve/icon_" + stockCurveRadioGroup.names[10] + ".png"
        }

        // When the selected option changes, relay the signal so that it can be picked
        // up outside this component.
        onSelectedIndexChanged: {
            if(selectedIndex >= 0) {
                stockCurveRadioGroupContainer.selectedCurveChanged(stockCurveRadioGroup.curves[selectedIndex], stockCurveRadioGroup.names[selectedIndex])
            }
        }
    }

    // This is a reset function for the component RadioGroup. If called, it removes any
    // selected RadioOption.
    function resetSelectedOption () {
        stockCurveRadioGroup.resetSelectedIndex ();
    }

    // This function allows selecting a specific index in the RadioGroup.
    function setSelectedIndex (index) {
        stockCurveRadioGroup.setSelectedIndex (index);
    }
}// Container
