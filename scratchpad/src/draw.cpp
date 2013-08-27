/* Copyright (c) 2012, 2013  Stuart MacMartin.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
* PARTICULAR PURPOSE AND NONINFRINGEMENT.
* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR
* OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include <QImage>
#include <QPainter>

#include "draw.h"

/*
 * Draw a circle in an image of the specified size.
 */
//! [0]
QImage Draw::drawCircle(const QSize &size)
{
    // Create an image of the appropriate size.
    // The underlying data is reference counted so is cleaned up as needed.
    QImage image(size, QImage::Format_RGB32);
    image.fill(Qt::black);

    // Pick an arbitrary size for the circle
    const int centerX = size.width() / 2;
    const int centerY = size.height() / 2;
    const int radius = std::min(centerX, centerY) * 2 / 3;
    const int diameter = radius * 2;

    // Draw the circle!
    QPainter painter(&image);
    painter.setPen(Qt::white);
    painter.drawEllipse(centerX-radius, centerY-radius, diameter, diameter);

    return image;
}
//! [0]
/*
 * Draw a rectangle in an image of the specified size.
 */
//! [1]
QImage Draw::drawSquare(const QSize &size)
{
    QImage image(size, QImage::Format_RGB32);
    image.fill(Qt::black);

    // Pick an arbitrary size for the square
    const int centerX = size.width() / 2;
    const int centerY = size.height() / 2;
    const int w = size.width()  * 2 / 3;
    const int h = size.height() * 2 / 3;

    // Draw the square!
    QPainter painter(&image);
    painter.setPen(Qt::white);
    painter.drawRect(centerX - w/2, centerY - h/2, w, h);

    return image;
}
//! [1]
/*
 * Replicate an image into another image.
 * Scale down the replicant if necessary to fit.
 */
//! [2]
QImage Draw::replicate(const QSize &destinationSize, const QImage &replicant, int numRows, int numCols)
{
    if (numRows <= 0 || numCols <= 0)
        return QImage(); // illegal call

    QImage destination(destinationSize, QImage::Format_RGB32);

    const int numPixelsX = destinationSize.width()  / numCols;
    const int numPixelsY = destinationSize.height() / numRows;

    const int extraPixelsX = destination.width()  - (numPixelsX * numCols);
    const int extraPixelsY = destination.height() - (numPixelsY * numRows);

    const int offsetX = extraPixelsX / 2;
    const int offsetY = extraPixelsY / 2;

    // Fill the extra pixels and overwrite the last contents
    // in case there is any transparency
    destination.fill(Qt::black);

    // Replicate
    QPainter painter(&destination);
    for (int rows = 0; rows < numRows; ++rows) {
        const int y = offsetY + rows * numPixelsY;
        for (int cols = 0; cols < numCols; ++cols) {
            painter.drawImage(offsetX + cols * numPixelsX, y, replicant);
        }
    }

    return destination;
}
//! [2]
