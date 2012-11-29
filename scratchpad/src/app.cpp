/* Copyright (c) 2012 Stuart MacMartin.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
* PARTICULAR PURPOSE AND NONINFRINGEMENT.
* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR
* OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include "app.h"
#include "draw.h"

#include <bb/cascades/AbstractPane>
#include <bb/cascades/Application>
#include <bb/cascades/QmlDocument>
#include <bb/cascades/Image>
#include <bb/ImageData>

using namespace bb::cascades;

namespace
{
QSize workingImageSize(700, 500);
}
//! [0]
App::App()
    : m_rows(1)
    , m_columns(1)
{
    QmlDocument *qml = QmlDocument::create("asset:///main.qml");
    qml->setContextProperty("_scratchpad", this);

    AbstractPane *root = qml->createRootObject<AbstractPane>();
    Application::instance()->setScene(root);
}
//! [0]
//! [1]
void App::setObject(const QString &object)
{
    if (m_object == object)
        return;

    m_object = object;
    emit objectChanged();

    updateImage();
}
//! [1]

QString App::object() const
{
    return m_object;
}
//! [2]
void App::setRows(const QString &text)
{
    bool ok = false;
    int rows = text.toInt(&ok);
    if (!ok || (rows <= 0))
        rows = 1;

    if (m_rows == rows)
        return;

    m_rows = rows;
    emit rowsChanged();

    updateImage();
}
//! [2]

QString App::rows() const
{
    return QString::number(m_rows);
}
//! [3]
void App::setColumns(const QString &text)
{
    bool ok = false;
    int columns = text.toInt(&ok);
    if (!ok || (columns <= 0))
        columns = 1;

    if (m_columns == columns)
        return;

    m_columns = columns;
    emit columnsChanged();

    updateImage();
}
//! [3]
QString App::columns() const
{
    return QString::number(m_columns);
}

bb::cascades::Image App::image() const
{
    return m_image;
}

/*
 * Return the size we need for the image that is to be replicated
 */
//! [4]
QSize App::desiredReplicantSize() const
{
    const int numPixelsX = workingImageSize.width() / m_columns;
    const int numPixelsY = workingImageSize.height() / m_rows;
    return QSize(numPixelsX, numPixelsY);
}
//! [4]
/*
 * Update the ImageView with the replicated stamps or images
 */
//! [5]
void App::updateImage()
{
    QImage replicant;

    if (m_object == QLatin1String("circle"))
        replicant = Draw::drawCircle(desiredReplicantSize());
    else if (m_object == QLatin1String("square"))
        replicant = Draw::drawSquare(desiredReplicantSize());
    else {
        // Load from file
        replicant.load(QString::fromLatin1("app/native/assets/%1").arg(m_object));
        replicant = replicant.scaled(desiredReplicantSize());
    }

    const QImage finalImage = Draw::replicate(workingImageSize, replicant, m_rows, m_columns).rgbSwapped();

    const bb::ImageData imageData = bb::ImageData::fromPixels(finalImage.bits(), bb::PixelFormat::RGBX, finalImage.width(), finalImage.height(), finalImage.bytesPerLine());

    m_image = bb::cascades::Image(imageData);
    emit imageChanged();
}
//! [5]
