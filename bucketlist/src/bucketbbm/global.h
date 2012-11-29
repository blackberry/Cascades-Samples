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

#ifndef GLOBAL_HPP_
#define GLOBAL_HPP_

#include <bb/platform/bbm/Context>
#include <bb/cascades/NavigationPane>

namespace bucketbbm
{
    /**
     * Global Description:
     *
     * A singleton class for sharing the BBM Context within the application.
     */
    class Global
    {

    public:
        /**
         * Function for getting a GLobal instance for accessing the
         * BBM context.
         *
         * @return An instance of the Global context object
         */
        static Global* instance();

        /**
         * Function for accessing the current BBM Context.
         *
         * @return The BBM Context.
         */
        bb::platform::bbm::Context* getContext();

        /**
         * Sets the current BBM Context of the application.
         *
         * @param context The BBM Context.
         */
        void setContext(bb::platform::bbm::Context *context);

    private:
        //A bb::platform::bbm::Context object that can be shared within the application.
        bb::platform::bbm::Context * mContext;
    };
}
#endif /* GLOBAL_HPP_ */
