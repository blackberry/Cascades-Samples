/*
 * PixelBuffer.h
 *
 *  Created on: Jul 9, 2012
 *      Author: smacmartin
 */

#ifndef PIXELBUFFER_H_
#define PIXELBUFFER_H_

#include <bb/cascades/PixelBufferData>

/*
 * PixelBuffer converts from one pixel format to another.
 * It maintains an array of pixel data in cascades::Image format.
 * This can be set by assigning the PixelBuffer from a QImage.
 * The PixelBufferData structure needed to initialize a cascade::Image
 * is returned by value on demand.
 *
 * The buffer is re-used from call to call until the number of pixels
 * changes or the PixelBuffer object is destroyed.  In the sample, we
 * see only two pixel sizes: we start with one size to set up the
 * button images, then permanently switch to another size.   In a more
 * complex app, we may need a cache of buffers, perhaps limiting the
 * cache to a maximum size.
 */
class PixelBuffer
{
public:
    PixelBuffer();
    ~PixelBuffer();

    PixelBuffer& operator=(const QImage&);
    bb::cascades::PixelBufferData getBuffer() const;

private:
    void deleteBuffer();

private:
    QSize m_sizeInPixels;
    unsigned char* m_buffer; // pixel data in PixelBufferData format
};



#endif /* PIXELBUFFER_H_ */
