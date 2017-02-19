#include "board.h"

Board::Board(QWidget *parent) : QWidget(parent), image(this->size(), QImage::Format_RGB32)
{
}

void Board::fill(QRgb color) {
    // red = green = blue
    if ((uchar) color == (uchar) (color >> 8) &&
            (uchar) color == (uchar) (color >> 16)) {
        memset(image.bits(), (uchar) color, image.bytesPerLine() * image.height());
    } else {
        QRgb *pixels = reinterpret_cast<QRgb *>(image.bits());
        for (uint j = 0; j < image.height(); ++j) {
            for (uint i = 0; i < image.width(); ++i) {
                pixels[j * image.width() + i] = color;
            }
        }
    }
}

void Board::drawLine(QPoint from, QPoint to, QRgb color) {
    if (from.x() == to.x()) {
        drawVerticalLine(from, to, color);
    } else if (from.y() == to.y()) {
        drawHorizontalLine(from, to, color);
    } else {
        drawLineBresenham(from, to, color);
    }
}

void Board::drawLineBresenham(QPoint &from, QPoint &to, QRgb color) {
    if (abs(to.y() - from.y()) / abs(to.x() - from.x()) > 1) {
        drawLineBresenhamY(from, to, color);
        return;
    }
    if (from.x() > to.x()) {
        std::swap(from, to);
    }
    uint dx = to.x() - from.x();
    uint dy = abs(to.y() - from.y());
    int err = 2 * dy - dx;
    uint y = from.y();
    uint x;
    int sign = to.y() > from.y() ? 1 : -1;
    QRgb *pixels = reinterpret_cast<QRgb *>(image.bits());
    for (x = from.x(); x < to.x(); ++x) {
        pixels[y * image.width() + x] = color;
        if (err > 0) {
            y += sign;
            err = err - dx;
        }
        err = err + dy;
    }
}

void Board::drawLineBresenhamY(QPoint &from, QPoint &to, QRgb color) {
    if (from.y() > to.y()) {
        std::swap(from, to);
    }
    uint dx = abs(to.x() - from.x());
    uint dy = to.y() - from.y();
    int err = 2 * dx - dy;
    uint x = from.x();
    uint y;
    int sign = to.x() > from.x() ? 1 : -1;
    QRgb *pixels = reinterpret_cast<QRgb *>(image.bits());
    for (y = from.y(); y < to.y(); ++y) {
        pixels[y * image.width() + x] = color;
        if (err > 0) {
            x += sign;
            err = err - dy;
        }
        err = err + dx;
    }
}

void Board::drawHorizontalLine(QPoint &from, QPoint &to, QRgb color) {
    if (from.x() > to.x()) {
        std::swap(from, to);
    }
    if ((uchar) color == (uchar) (color >> 8) &&
            (uchar) color == (uchar) (color >> 16)) {
        uchar *line = image.bits() + from.y() * image.bytesPerLine();
        memset(line + from.x() * image.depth() / 8, color, 3 * (to.x() - from.x()));
    } else {
        QRgb *pixels = reinterpret_cast<QRgb *>(image.bits());
        for (uint i = from.x(); i < to.x(); ++i) {
            pixels[from.y() * image.width() + i] = color;
        }
    }
}

void Board::drawVerticalLine(QPoint &from, QPoint &to, QRgb color) {
    if (from.y() > to.y()) {
        std::swap(from, to);
    }
    QRgb *pixels = reinterpret_cast<QRgb *>(image.bits());
    for (uint j = from.y(); j < to.y(); ++j) {
        pixels[j * image.width() + from.x()] = color;
    }
}

void Board::paint() {
    fill(WhiteColor);
    drawLine(QPoint(0, 50), QPoint(50, 0), RedColor);
    drawLine(QPoint(0, 50), QPoint(100, 10), BlackColor);
    drawLine(QPoint(0, 50), QPoint(10, 3), BlackColor);
    drawLine(QPoint(100, 100), QPoint(200, 100), GreenColor);
    drawLine(QPoint(200, 100), QPoint(200, 200), GreenColor);
}

void Board::resizeEvent(QResizeEvent * event) {
    image = QImage(this->size(), QImage::Format_RGB32);
    paint();
}

void Board::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    painter.drawImage(0, 0, image);
}
