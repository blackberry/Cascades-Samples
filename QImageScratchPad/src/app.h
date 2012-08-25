#ifndef APP_H
#define APP_H

#include <QtCore/QObject>
#include <QtCore/QMetaType>
#include <QtCore/QSharedPointer>

#include <bb/cascades/Event>
#include <bb/cascades/UiObject>
#include <bb/cascades/Control>

namespace bb
{
namespace cascades
{
    class ImageView;
}
}

class App : public QObject
{
    Q_OBJECT

public:
    App();

private slots:
    void onIntFieldTouched(bb::cascades::TouchEvent*);
    void onNumRowsChanged(QString);
    void onNumColsChanged(QString);
    void onImageSelected();
    void onCircleSelected();
    void onSquareSelected();

private:
    void setCurrentImage(const QImage& image);
    void updateImage();
    void replaceImageInView(const QImage& qImage);
    QSize getDesiredReplicantSize() const;
    QImage selectReplicant() const;

private:
    int m_numRows;
    int m_numCols;
    QImage m_currentRawImage; // The current underlying image

    enum
    {
        kImage,
        kCircle,
        kSquare
    } m_drawType;

    bb::cascades::ImageView* m_finalImage; // The imageView to draw into
};

#endif // ifndef APP_H
