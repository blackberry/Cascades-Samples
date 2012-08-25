/*
 * PixelBuffer.cpp
 *
 *  Created on: Jul 9, 2012
 *      Author: smacmartin
 */

#include <bb/cascades/PixelBufferData>
#include "PixelBuffer.h"

using bb::cascades::PixelBufferData;

PixelBuffer::PixelBuffer()
: m_sizeInPixels(0,0)
, m_buffer(0)
{
}

PixelBuffer::~PixelBuffer()
{
    deleteBuffer();
}

void PixelBuffer::deleteBuffer()
{
    if (m_buffer != 0)
    {
        delete [] m_buffer;
        m_buffer = 0;
    }
}

PixelBuffer& PixelBuffer::operator=(const QImage& image)
{
    QImage swapped     = image.rgbSwapped();
    QSize  swappedSize = swapped.size();

    int w = swappedSize.width();
    int h = swappedSize.height();
    int numPixels = w * h;
    int numBytes  = w * h * 4;
    if (swappedSize != m_sizeInPixels)
    {
        deleteBuffer();
        m_sizeInPixels = QSize(w, h);
        m_buffer = new uchar[numBytes];
    }

    // Copy the memory over.
    // We'll add defensive code in case rgbSwapped has a different size
    const uchar* from = swapped.constBits();
    int numFromBytes = swapped.numBytes();
    int numToCopy = std::min(numFromBytes, numBytes);

    memcpy(m_buffer, from, numToCopy);
    if (numToCopy < numBytes)
    {
        memset(m_buffer + numToCopy, 0x00, numBytes - numToCopy);
    }

    return *this;
}

bb::cascades::PixelBufferData PixelBuffer::getBuffer() const
{
    return PixelBufferData(PixelBufferData::RGBX,
                           m_sizeInPixels.width(),
                           m_sizeInPixels.height(),
                           m_sizeInPixels.width(),
                           m_buffer);
}
