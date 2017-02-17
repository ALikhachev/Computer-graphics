#include "qcanvas.h"

QCanvas::QCanvas(QWidget *parent) : QWidget(parent), image(this->size(), QImage::Format_RGB32)
{
}

void QCanvas::drawLine(uint x_from, uint y_from, uint x_to, uint y_to) {
}

void QCanvas::bresenham(uint x_from, uint y_from, uint x_to, uint y_to) {
}

#define min(x, y) (x) > (y) ? (y) : (x)
#define max(x, y) (x) > (y) ? (x) : (y)

void QCanvas::resizeEvent(QResizeEvent * event) {
    image = QImage(this->size(), QImage::Format_RGB32);
}

void QCanvas::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    image.fill(qRgb(255, 255, 255));
    uchar* bits = image.bits();
    int y_max = max(0, min(50, image.height() - 50));
    int x_max = max(0, min(50, image.width() - 50));
    for (int j = 0; j < y_max; ++j) {
        uchar *line = bits + (50 + j) * image.bytesPerLine();
        memset(line + (50 * image.depth() / 8), 0, image.depth() / 8 * x_max);
    }
    painter.drawImage(0, 0, image);
}
