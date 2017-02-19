#include "board.h"

Board::Board(QWidget *parent) : QWidget(parent), image(this->size(), QImage::Format_RGB32)
{
    paint();
}

void Board::fill(QRgb color) {
    // red = green = blue
    if ((uchar) color == (uchar) (color >> 8) &&
            (uchar) color == (uchar) (color >> 16)) {
        memset(image.bits(), (uchar) color, image.bytesPerLine() * image.height());
    } else {
        QRgb *pixels = reinterpret_cast<QRgb *> (image.bits());
        for (size_t j = 0; j < image.height(); ++j) {
            for (size_t i = 0; i < image.width(); ++i) {
                pixels[j * image.width() + i] = color;
            }
        }
    }
}

void Board::paint() {
    fill(WhiteColor);
}

void Board::resizeEvent(QResizeEvent * event) {
    image = QImage(this->size(), QImage::Format_RGB32);
    paint();
}

void Board::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    painter.drawImage(0, 0, image);
}
