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

// A radio group QML component used by the stock curve recipe.
// This component contain 11 options that are set by defining the
// stockCurveNames variant list (these names are used to set the option image).
Container {
    id: stockCurveRadioGroupContainer

    // The curves used for storing a different StockCurve in a property for each option.
    property alias stockCurves: stockCurveRadioGroup.curves

    // The name of the curve.
    property alias stockCurveNames: stockCurveRadioGroup.names

    // A title put on top of the RadioGroup.
    property alias title: titleLabel.text

    // When a option is selected a signal is emitted by this component.
    signal selectedCurveChanged (variant curve, string name)

    layout: StackLayout {
        topPadding: 20
    }

    // The Component title.
    Label {
        id: titleLabel
        text: ""
        textStyle {
            base: SystemDefaults.TextStyles.SmallText
            alignment: TextAlignment.Center
        }
        layoutProperties: StackLayoutProperties {
            horizontalAlignment: HorizontalAlignment.Fill
        }
    }

        // The radio group presenting the different curves.
    RadioGroup {
        id: stockCurveRadioGroup
        property variant curves: null
        property variant names: null

        Option {
            // The image presented with the radio option, which makes it possible to see
            // the difference in timing behavior of different StockCurves.
            imageSource: "asset:///images/stockcurve/icon_" + stockCurveRadioGroup.names[0]
        }
        Option {
            imageSource: "asset:///images/stockcurve/icon_" + stockCurveRadioGroup.names[1]
        }
        Option {
            imageSource: "asset:///images/stockcurve/icon_" + stockCurveRadioGroup.names[2]
        }
        Option {
            imageSource: "asset:///images/stockcurve/icon_" + stockCurveRadioGroup.names[3]
        }
        Option {
            imageSource: "asset:///images/stockcurve/icon_" + stockCurveRadioGroup.names[4]
        }
        Option {
            imageSource: "asset:///images/stockcurve/icon_" + stockCurveRadioGroup.names[5]
        }
        Option {
            imageSource: "asset:///images/stockcurve/icon_" + stockCurveRadioGroup.names[6]
        }
        Option {
            imageSource: "asset:///images/stockcurve/icon_" + stockCurveRadioGroup.names[7]
        }
        Option {
            imageSource: "asset:///images/stockcurve/icon_" + stockCurveRadioGroup.names[8]
        }
        Option {
            imageSource: "asset:///images/stockcurve/icon_" + stockCurveRadioGroup.names[9]
        }
        Option {
            imageSource: "asset:///images/stockcurve/icon_" + stockCurveRadioGroup.names[10]
        }

        // When the selected option changes relay the signal so that it can be picked
        // up outside this component.
        onSelectedIndexChanged: {
            if(selectedIndex >= 0) {
                stockCurveRadioGroupContainer.selectedCurveChanged(stockCurveRadioGroup.curves[selectedIndex], stockCurveRadioGroup.names[selectedIndex])
            }
        }
    }

    // Reset function for the component RadioGroup. If called removes any
    // selected RadioOption.
    function resetSelectedOption () {
        stockCurveRadioGroup.resetSelectedIndex ();
    }

    // Function that allows selecting a specific index in the RadioGroup.
    function setSelectedIndex (index) {
        stockCurveRadioGroup.setSelectedIndex (index);
    }
}
