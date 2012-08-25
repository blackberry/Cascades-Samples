// System includes
#include <unistd.h>
// NDK includes
#include <bps/virtualkeyboard.h>

// Cascades includes
#include <bb/cascades/Application>
#include <bb/cascades/QmlDocument>
#include <bb/cascades/AbstractPane>
#include <bb/cascades/TextField>
#include <bb/cascades/Button>
#include <bb/cascades/ImageView>
#include <bb/cascades/Image>

// app includes
#include "app.h"
#include "PixelBuffer.h"
#include "draw.h"

using namespace bb::cascades;

// We want management of the underlying pointers.
// We could either use QSharedPointer everywhere,
// or use a subclass RawImages that owns and manages the images.
// The former adds some syntax but it's easier to get the
// memory management right.
QHash<QUrl, QImage> rawImages;  // Hold onto the underlying images we use

namespace
{
QSize workingImageSize(700, 500);

QImage& getWorkingImage()
{
    static QImage workingImage; // ensures memory cleanup on exit

    if (workingImage.isNull())
    {
        workingImage = QImage(workingImageSize, QImage::Format_RGB32);
    }
    return workingImage;
}

QImage getRawImage(QUrl url)
{
    QImage image = rawImages.value(url);
    if (image.isNull())
    {
        image = loadImage(url);
        if (!image.isNull())
        {
            rawImages[url] = image;
        }
    }
    return image;
}

} // namespace

App::App()
: m_numRows(1)
, m_numCols(1)
, m_drawType(kCircle)
, m_finalImage(0)
{
    QmlDocument *qml = QmlDocument::create("main.qml");
    qml->setContextProperty("cs", this);

    AbstractPane *root = qml->createRootNode<AbstractPane>();
    Application::setScene(root);

    if (root != 0)
    {
        TextField* textField = root->findChild<TextField*>("numRows");
        connect(textField, SIGNAL(touch(bb::cascades::TouchEvent*)), this, SLOT(onIntFieldTouched(bb::cascades::TouchEvent*)));
        connect(textField, SIGNAL(textChanged(QString)), this, SLOT(onNumRowsChanged(QString)));

        textField = root->findChild<TextField*>("numCols");
        connect(textField, SIGNAL(touch(bb::cascades::TouchEvent*)), this, SLOT(onIntFieldTouched(bb::cascades::TouchEvent*)));
        connect(textField, SIGNAL(textChanged(QString)), this, SLOT(onNumColsChanged(QString)));

        // Four hard-coded buttons.
        // Three select an image from the asset...
        Button* button = root->findChild<Button*>("img1");
        connect(button, SIGNAL(clicked()), this, SLOT(onImageSelected()));
        button = root->findChild<Button*>("img2");
        connect(button, SIGNAL(clicked()), this, SLOT(onImageSelected()));
        button = root->findChild<Button*>("img3");
        connect(button, SIGNAL(clicked()), this, SLOT(onImageSelected()));

        // ...and two specify images to draw on demand
        button = root->findChild<Button*>("circle");
        connect(button, SIGNAL(clicked()), this, SLOT(onCircleSelected()));
        button = root->findChild<Button*>("square");
        connect(button, SIGNAL(clicked()), this, SLOT(onSquareSelected()));
        m_finalImage = root->findChild<ImageView*>("finalImage");

        // Set the image on the circle and the square
        PixelBuffer pixelBuffer;
        button = root->findChild<Button*>("circle");
        pixelBuffer = drawCircle(QSize(50,50));
        button->setImage(pixelBuffer.getBuffer());

        button = root->findChild<Button*>("square");
        pixelBuffer = drawSquare(QSize(50,50));
        button->setImage(pixelBuffer.getBuffer());
    }
}

void App::onIntFieldTouched(TouchEvent* event)
{
    if (event->isDown())
    {
        // Show the keyboard as number
        virtualkeyboard_hide();
        virtualkeyboard_show();
        virtualkeyboard_change_options(VIRTUALKEYBOARD_LAYOUT_NUM_PUNC, VIRTUALKEYBOARD_ENTER_DEFAULT);
    }
}

// Slot invoked whenever the # rows changes.
// Update the number of rows to display
// and rebuild the image.
void App::onNumRowsChanged(QString text)
{
    int value = text.toInt();
    if (value <= 0)
    {
        value = 1;
    }
    if (m_numRows != value)
    {
        m_numRows = value;
        updateImage();
    }
}

// Slot invoked whenever the # columns changes.
// Update the number of columns to display
// and rebuild the image.
void App::onNumColsChanged(QString text)
{
    int value = text.toInt();
    if (value <= 0)
    {
        value = 1;
    }
    if (m_numCols != value)
    {
        m_numCols = value;
        updateImage();
    }
}

/*
 * Slot to handle selecting repeated stamp images
 */
void App::onImageSelected()
{
    Button* button = dynamic_cast<Button*>(sender());
    if (button)
    {
        QUrl imageSource = button->imageSource();
        QImage image = getRawImage(imageSource);
        if (!image.isNull())
        {
            setCurrentImage(image);
        }
    }
}

/*
 * Slot to handle selecting repeated circles
 */
void App::onCircleSelected()
{
    // We don't want to scale a circle
    // Instead, we want to draw a circle of particular size as needed
    m_drawType = kCircle;
    updateImage();
}

/*
 * Slot to handle selecting repeated squares
 */
void App::onSquareSelected()
{
    m_drawType = kSquare;
    updateImage();
}

/*
 * Replace the selected image with the specified one.
 */
void App::setCurrentImage(const QImage& image)
{
    if (m_drawType != kImage || m_currentRawImage.constBits() != image.constBits())
    {
        m_drawType = kImage;
        m_currentRawImage = image;
        updateImage();
    }
}

/*
 * Return the size we need for the image that is to be replicated
 */
QSize App::getDesiredReplicantSize() const
{
    int numRows = (m_numRows < 0)? 1 : m_numRows;
    int numCols = (m_numCols < 0)? 1 : m_numCols;
    int numPixelsX = workingImageSize.width()  / numCols;
    int numPixelsY = workingImageSize.height() / numRows;
    return QSize(numPixelsX, numPixelsY);

}

/*
 * Select the image we want to replicate.
 * We do not need to scale it now, but if we are drawing
 * the image, draw into the desired size to avoid artifacts
 * created by scaling
 */
QImage App::selectReplicant() const
{
    QImage replicant;
    switch (m_drawType)
    {
    case kImage:
        if (!m_currentRawImage.isNull())
        {
            replicant = m_currentRawImage;
            break;
        }
        replicant = drawCircle(getDesiredReplicantSize());
        break;

    case kCircle:
        replicant = drawCircle(getDesiredReplicantSize());
        break;

    case kSquare:
        replicant = drawSquare(getDesiredReplicantSize());
        break;
    }
    return replicant;
}

/*
 * Update the ImageView with the replicated stamps or images
 */
void App::updateImage()
{
    if (m_finalImage == 0)
        return; // defensive

    QImage replicant = selectReplicant();
    if (replicant.isNull())
        return; // Nothing selected to draw

    QImage& qImage = getWorkingImage();
    replicate(qImage, replicant, m_numRows, m_numCols);
    replaceImageInView(qImage);
}

/*
 * Transfer the image we constructed to the view
 * This is done using a helper object that converts
 * the pixel data from QImage format to cascades::Image format.1234
 */
void App::replaceImageInView(const QImage& qImage)
{
    if (m_finalImage == 0)
        return; // defensive

    static PixelBuffer pixels;
    pixels = qImage;

    m_finalImage->setImage(pixels.getBuffer());
}


