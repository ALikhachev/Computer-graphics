#ifndef QCANVAS_H
#define QCANVAS_H

#include <QImage>
#include <QPainter>
#include <QWidget>
#include <QPaintEvent>
#include <QSharedPointer>

class QCanvas : public QWidget
{
    Q_OBJECT
public:
    explicit QCanvas(QWidget *parent = 0);

protected:
    void paintEvent(QPaintEvent *) override;
    void resizeEvent(QResizeEvent *) override;

signals:

public slots:

private:
    void drawLine(uint x_from, uint y_from, uint x_to, uint y_to);
    void bresenham(uint x_from, uint y_from, uint x_to, uint y_to);

    QImage image;
};

#endif // QCANVAS_H
