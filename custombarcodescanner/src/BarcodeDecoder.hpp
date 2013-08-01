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
#ifndef BARCODEDECODER_HPP
#define BARCODEDECODER_HPP

#include <bb/cascades/Container>
#include <bb/cascades/CustomControl>
#include <bb/cascades/multimedia/Camera>
#include <bb/cascades/multimedia/CameraSettings.hpp>

#include <zxing/MultiFormatReader.h>

namespace bb {
namespace community {
namespace barcode {

/**
 * Class tasked with opening camera and reading barcodes.
 */
class BarcodeDecoderControl: public bb::cascades::CustomControl
{
    Q_OBJECT

public:
    BarcodeDecoderControl(bb::cascades::Container *parent = 0);

public Q_SLOTS:
    /**
     * This method is invoked when the preview frame buffer has some
     * data for it to process.
     */
    void onPreviewFrameAvailable(
            bb::cascades::multimedia::SharedUCharPointer previewBuffer,
            quint64 size, unsigned int width, unsigned int height,
            unsigned int stride);
    /**
     * This method is invoked upon the camera opening.
     */
    void onCameraOpened();

    /**
     * This method clears the barcode data. It is invoked
     * when camera generates the view finder stop signal.
     */
    void onViewfinderStopped();

    /**
     * Invoking this method starts view finder preview.
     */
    void startScanning() const;

    /**
     * Invoking this method stops view finder preview.
     */
    void stopScanning() const;

Q_SIGNALS:
    /**
     * Signal generated when barcode data has been detected.
     * @param barcode The barcode data value.
     */
    void newBarcodeDetected(QString barcode);

    /**
     * Signal generated when barcode reading is activated.
     */
    void scanningStarted();

    /**
     * Signal generated when barcode reading is stopped.
     */
    void scanningStopped();

private:
    zxing::Ref<zxing::MultiFormatReader> m_reader;
    QString m_barcodeData;
    bb::cascades::multimedia::Camera* m_camera;
    bb::cascades::multimedia::CameraSettings* m_cameraSettings;
    bool m_landscapePreviewFrames;
    int m_nbuffers;
};
}
}
}

#endif
