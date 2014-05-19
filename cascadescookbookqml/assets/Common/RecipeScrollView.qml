/* Copyright (c) 2012, 2013, 2014 BlackBerry Limited.
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

// For some recipes in the cookbook we use a "ScrollView" when the
// we know that content will not fit on the given device. The default
// content is designed to fit on 720x720 so in that case a ScrollView is used.
// For other cases matching minimum 76du in width and 128du in height 
// (like e.g. Z10 and Z30) just a Container is used (see mindw76h128du folder).
ScrollView {
    scrollViewProperties {
        scrollMode: ScrollMode.Vertical
    }
}
