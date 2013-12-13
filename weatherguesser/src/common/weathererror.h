/* Copyright (c) 2013 BlackBerry Limited.
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
#ifndef WEATHERERROR_H_
#define WEATHERERROR_H_

#include <QObject>

/**
 * WeatherError Description
 *
 * The error code of the weatherguesser network request.
 */
class WeatherError
{
    Q_GADGET
    Q_ENUMS(Type)

public:

    enum Type {
        NoError = 0,
        ServerError,
        ServerBusy,
        JsonError,
        InvalidCity
    };

private :
    WeatherError();
    ~WeatherError();
};

#endif /* WEATHERERROR_H_ */
