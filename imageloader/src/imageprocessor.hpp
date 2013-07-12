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

#ifndef IMAGESCALER_HPP
#define IMAGESCALER_HPP

#include <QtCore/QByteArray>
#include <QtCore/QObject>
#include <QtGui/QImage>

/**
 * @short A class to convert raw image data into a QImage and process it further.
 *
 * The class is designed to be used inside a QThread by providing a separated
 * start() slot and a finished() signal.
 */
//! [0]
class ImageProcessor : public QObject
{
    Q_OBJECT

public:
    /*
     * Creates a new image processor.
     *
     * @param imageData The raw image data.
     * @param parent The parent object.
     */
    ImageProcessor(const QByteArray &imageData, QObject *parent = 0);

public Q_SLOTS:
    /*
     * Starts the actual operation.
     */
    QImage start();

private:
    // The raw image data
    QByteArray m_data;
};
//! [0]

#endif
