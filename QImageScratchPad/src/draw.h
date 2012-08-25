/*
 * draw.h
 *
 *  Created on: Jul 10, 2012
 *      Author: smacmartin
 */

#ifndef DRAW_H_
#define DRAW_H_

class QImage;

QImage loadImage(QUrl url);
QImage drawCircle(QSize size);
QImage drawSquare(QSize size);

void replicate(QImage& destination, QImage replicant, int numRows, int numCols);

#endif /* DRAW_H_ */
