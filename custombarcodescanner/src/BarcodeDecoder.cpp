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
#include "BarcodeDecoder.hpp"

#include <bb/cascades/DisplayDirection>
#include <bb/cascades/StackLayout>
#include <bb/cascades/multimedia/CameraTypes.hpp>

#include <QSharedPointer>

#include <zxing/common/GreyscaleLuminanceSource.h>
#include <zxing/common/HybridBinarizer.h>

using namespace bb::cascades;
using namespace bb::cascades::multimedia;
using namespace bb::community::barcode;
using namespace zxing;

//! [0]
BarcodeDecoderControl::BarcodeDecoderControl(Container *parent)
    : CustomControl(parent)
    , m_camera(new Camera(parent))
    , m_cameraSettings(new CameraSettings(this))
    , m_landscapePreviewFrames(false)
    , m_nbuffers(2)
{
    setRoot(m_camera);

    bool ok = connect(m_camera, SIGNAL(cameraOpened()),
                      this, SLOT(onCameraOpened()));
    Q_ASSERT(ok);

    ok = connect(m_camera, SIGNAL(viewfinderStopped()),
                 this, SLOT(onViewfinderStopped()));
    Q_ASSERT(ok);

    //Prepare the camera
    m_camera->open(CameraUnit::Rear);

    //Configure camera settings
    m_camera->getSettings(m_cameraSettings);
    m_cameraSettings->setCameraMode(CameraMode::Photo);
    m_cameraSettings->setFocusMode(CameraFocusMode::ContinuousMacro);

    if (m_camera->applySettings(m_cameraSettings))
        qDebug() << "settings applied successfully";

    //Prepare the decoder
    m_reader = Ref<MultiFormatReader>(new MultiFormatReader());
    DecodeHints *hints = new DecodeHints();
    hints->addFormat(BarcodeFormat_QR_CODE);
    hints->addFormat(BarcodeFormat_EAN_8);
    hints->addFormat(BarcodeFormat_EAN_13);
    hints->addFormat(BarcodeFormat_UPC_A);
    hints->addFormat(BarcodeFormat_UPC_E);
    hints->addFormat(BarcodeFormat_DATA_MATRIX);
    hints->addFormat(BarcodeFormat_CODE_128);
    hints->addFormat(BarcodeFormat_CODE_39);
    hints->addFormat(BarcodeFormat_ITF);
    hints->addFormat(BarcodeFormat_AZTEC);
    m_reader.object_->setHints(*hints);
}
//! [0]

//! [1]
void BarcodeDecoderControl::onPreviewFrameAvailable(
        SharedUCharPointer previewBuffer, quint64 size, unsigned int width,
        unsigned int height, unsigned int stride)
{
    try {
        Ref<LuminanceSource> source(
                new GreyscaleLuminanceSource(previewBuffer.data(), stride,
                        size / stride, 0, 0, width, height));
        Ref<Binarizer> binarizer(new HybridBinarizer(source));
        Ref<BinaryBitmap> bitmap(new BinaryBitmap(binarizer));
        Ref<Result> result;

        // If the preview buffer is in landscape, we can rotate out bitmap to let us scan 1D codes
        if (m_landscapePreviewFrames) {
            Ref<BinaryBitmap> rotated = bitmap->rotateCounterClockwise();
            result = m_reader->decode(rotated);
        } else {
            result = m_reader->decode(bitmap);

        }

        const QString newBarcodeData = QString::fromStdString(result->getText()->getText().data());

        if (newBarcodeData != m_barcodeData) {
            m_barcodeData = newBarcodeData;
            emit newBarcodeDetected(m_barcodeData);
        }

    } catch (std::exception &e) {
        qDebug() << "+++++++ ERROR: " << e.what() << endl;
    }

    m_camera->addPreviewBuffer(previewBuffer, size);
}
//! [1]

//! [2]
void BarcodeDecoderControl::onCameraOpened()
{
    const quint64 bufferSize = m_camera->previewBufferSize();
    //Use two buffers for double buffering goodness.
    for (int i = 0; i < m_nbuffers; i++)
        m_camera->addPreviewBuffer(QSharedPointer<unsigned char>(new unsigned char[bufferSize]), bufferSize);

    bool ok = connect(m_camera, SIGNAL(previewFrameAvailable(bb::cascades::multimedia::SharedUCharPointer, quint64, unsigned int, unsigned int, unsigned int)),
                      this, SLOT(onPreviewFrameAvailable(bb::cascades::multimedia::SharedUCharPointer, quint64, unsigned int, unsigned int, unsigned int)));
    Q_ASSERT(ok);
    Q_UNUSED(ok);

    // If the preview frames are oriented East or West, it means we will have to rotate them 90 degrees to reliably detect 1D barcodes
    if (m_camera->devicePreviewFrameDirection() == DisplayDirection::East || m_camera->devicePreviewFrameDirection() == DisplayDirection::West)
        m_landscapePreviewFrames = true;

    m_camera->startViewfinder();
}
//! [2]

//! [3]
void BarcodeDecoderControl::startScanning() const
{
    m_camera->startViewfinder();
}

void BarcodeDecoderControl::stopScanning() const
{
    m_camera->stopViewfinder();
}

void BarcodeDecoderControl::onViewfinderStopped()
{
    m_barcodeData.clear();
}
//! [3]
