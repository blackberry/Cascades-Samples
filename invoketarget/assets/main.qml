/* Copyright (c) 2012, 2013  BlackBerry Limited.
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

import bb.cascades 1.3

Page {
    titleBar: TitleBar {
        title: _app.title
    }
    ScrollView {
        horizontalAlignment: HorizontalAlignment.Fill
        verticalAlignment: VerticalAlignment.Top
        scrollViewProperties.scrollMode: ScrollMode.Vertical
        Container {
            topPadding: ui.du(5.6)
            leftPadding: ui.du(5.6)
            rightPadding: ui.du(5.6)
            Label {
                text: "<html>Try invoking this app as a <b>Card</b> with the following parameters. You can also use the InvokeClient sample app on blackberry.github.com/cascades-samples to invoke these cards.<hr/><p/><br/><b>Composer:</b><br/><i>Action:</i> bb.action.SHARE<br/><i>MIME:</i> text/plain<br/><i>Data:</i> [ANYTHING]<p/><br/><b>Previewer:</b><br/><i>Action:</i> bb.action.OPEN<br/><i>URI:</i> file:///path/to/file.jpg<p/><br/><b>Picker:</b><br/><i>Action:</i> com.example.PICKEGG<br/><i>MIME:</i> food/egg</html>"
                multiline: true
                textStyle {
                    fontSize: FontSize.XSmall
                    color: Color.LightGray
                }
            }
        }
    }
}
