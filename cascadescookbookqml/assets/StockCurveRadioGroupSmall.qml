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

// See StockCurveRadioGroup.qml for comments. The main difference is that
// this component only has 7 RadioOptions in its RadioGroup (instead of 11).
Container {
    id: stockCurveRadioGroupContainer
    property alias stockCurves: stockCurveRadioGroup.curves
    property alias stockCurveNames: stockCurveRadioGroup.names
    property alias title: titleLabel.text

    signal selectedCurveChanged (variant curve, string name)

    topPadding: 20;

    Label {
        id:titleLabel
        horizontalAlignment: HorizontalAlignment.Center
        textStyle.base: SystemDefaults.TextStyles.SmallText
    }

    RadioGroup {
        id: stockCurveRadioGroup
        property variant curves: null
        property variant names: null

        Option {
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

        onSelectedIndexChanged: {
            if(selectedIndex >= 0) {
                stockCurveRadioGroupContainer.selectedCurveChanged(stockCurveRadioGroup.curves[selectedIndex], stockCurveRadioGroup.names[selectedIndex])
            }
        }
    }

    function resetSelectedOption() {
        stockCurveRadioGroup.resetSelectedIndex();
    }
}
