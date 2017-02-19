#ifndef QCANVAS_H
#define QCANVAS_H

#include <QImage>
#include <QPainter>
#include <QWidget>
#include <QPaintEvent>
#include <QSharedPointer>

class Board : public QWidget
{
    Q_OBJECT
public:
    explicit Board(QWidget *parent = 0);

protected:
    void paintEvent(QPaintEvent *) override;
    void resizeEvent(QResizeEvent *) override;

signals:

public slots:

private:
    void paint();
    void fill(QRgb color);

    static const QRgb RedColor = qRgb(255, 0, 0);
    static const QRgb GreenColor = qRgb(0, 255, 0);
    static const QRgb BlueColor = qRgb(0, 0, 255);
    static const QRgb WhiteColor = qRgb(255, 255, 255);
    static const QRgb BlackColor = qRgb(0, 0, 0);


    QImage image;
};

#endif // QCANVAS_H
