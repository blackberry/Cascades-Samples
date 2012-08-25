/*
 * draw.cpp
 *
 *  Created on: Jul 10, 2012
 *      Author: smacmartin
 */

// System includes
#include <unistd.h>

#include <QDebug>
#include <QImage>
#include <QPainter>
#include <QUrl>

#include "draw.h"

/*
 * Get the directory containing the images
 */
QString getAssetDir()
{
    // images are in assets/images.
    // This is under app of our initial working directory.
    // Since we don't change directories, it's safe to return a relative path.
    char cwd[PATH_MAX];
    getcwd(cwd, PATH_MAX);
    return QString(cwd) + "/app/native/assets";
}

/*
 * Convert an image url e.g. "asset:///images/Blue_20Nose_20Thumb.png"
 * into a full path that can be opened with routines that don't know asset://
 */
QString getImagePath(QUrl imageUrl)
{
    QString resourceText = imageUrl.toString();
    int index = resourceText.indexOf("/images/");
    return (index > 0)? getAssetDir() + resourceText.mid(index) : resourceText;
}

/*
 * Load an image from a QUrl.
 * We need to load the image from the corresponding path
 */
QImage loadImage(QUrl url)
{
    QString where = getImagePath(url);
    return QImage(where);
}

/*
 * Draw a circle in an image of the specified size.
 * If it's the same as the last request, just return the last image.
 */
QImage drawCircle(QSize size)
{
    static QImage image;
    if (image.isNull() || image.size() != size)
    {
        // Create an image of the appropriate size.
        // The underlying data is reference counted so is cleaned up as needed.
        image = QImage(size, QImage::Format_RGB32);
        image.fill(Qt::blue);

        // Pick an arbitrary size for the circle
        int centerX = size.width() / 2;
        int centerY = size.height() / 2;
        int radius = std::min(centerX, centerY) * 2 / 3;
        int diameter = radius * 2;

        // Draw the circle!
        QPainter painter(&image);
        painter.setPen(Qt::yellow);
        painter.drawEllipse(centerX-radius, centerY-radius, diameter, diameter);
    }
    return image;
}

/*
 * Draw a rectangle in an image of the specified size.
 * If it's the same as the last request, just return the last image.
 */
 QImage drawSquare(QSize size)
{
    static QImage image;
    if (image.isNull() || image.size() != size)
    {
        image = QImage(size, QImage::Format_RGB32);
        image.fill(Qt::blue);

        int centerX = size.width() / 2;
        int centerY = size.height() / 2;
        int w = size.width()  * 2 / 3;
        int h = size.height() * 2 / 3;
        QPainter painter(&image);
        painter.setPen(Qt::yellow);
        painter.drawRect(centerX - w/2, centerY - h/2, w, h);
    }
    return image;
}

 /*
  * Replicate an image into another image.
  * Scale down the replicant if necessary to fit.
  */
 void replicate(QImage& destination, QImage replicant, int numRows, int numCols)
 {
     if (numRows <= 0 || numCols <= 0)
         return; // illegal call

     QSize imageSize = destination.size();
     int numPixelsX = imageSize.width()  / numCols;
     int numPixelsY = imageSize.height() / numRows;
     QSize desiredReplicantSize(numPixelsX, numPixelsY);

     int extraPixelsX = imageSize.width()  - (numPixelsX * numCols);
     int extraPixelsY = imageSize.height() - (numPixelsY * numRows);

     int offsetX = extraPixelsX / 2;
     int offsetY = extraPixelsY / 2;

     // Scale if necessary
     if (replicant.size() != desiredReplicantSize)
     {
         replicant = replicant.scaled(desiredReplicantSize);
     }

     // Fill the extra pixels and overwrite the last contents
     // in case there is any transparency
     destination.fill(Qt::magenta);

     // Replicate
     QPainter painter(&destination);
     for (int rows = 0; rows < numRows; ++rows)
     {
         int y = offsetY + rows * numPixelsY;
         for (int cols = 0; cols < numCols; ++cols)
         {
             painter.drawImage(offsetX + cols * numPixelsX, y, replicant);
         }
     }
}
