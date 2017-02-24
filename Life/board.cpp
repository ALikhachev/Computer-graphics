#include <QDateTime>

#include "board.h"
#include "utils.h"

using Utils::hexagonPoint;

Board::Board(QWidget *parent) :
    QWidget(parent),
    timer(this)
{
    resize(500, 400);
    image = QImage(this->size(), QImage::Format_RGB32);
    connect(&timer, SIGNAL(timeout()), this, SLOT(update()));
    timer.start(10);
}

void Board::fill(QRgb color) {
    // red = green = blue
    if ((uchar) color == (uchar) (color >> 8) &&
            (uchar) color == (uchar) (color >> 16)) {
        memset(image.bits(), (uchar) color, image.bytesPerLine() * image.height());
    } else {
        QRgb *pixels = reinterpret_cast<QRgb *>(image.bits());
        for (int j = 0; j < image.height(); ++j) {
            for (int i = 0; i < image.width(); ++i) {
                pixels[j * image.width() + i] = color;
            }
        }
    }
}

void Board::drawLine(QPoint from, QPoint to, QRgb color) {
    if (!tryDrawSimpleLine(from, to, color)) {
        clipLine(from, to);
        if (from.x() == to.x() || from.y() == to.y()) {
            return;
        }
        clipLine(to, from);
        if (from.x() == to.x() || from.y() == to.y()) {
            return;
        }
        drawLineBresenham(from, to, color);
    }
}

// Алгоритм Коэна — Сазерленда
void Board::clipLine(QPoint &from, QPoint &to) {
    if (from.x() < 0) {
        if (from.x() != to.x()) {
            int newY = from.y() + (to.y() - from.y()) * from.x() / (from.x() - to.x());
            from.setY(newY);
            from.setX(0);
        }
    }
    if (from.x() >= image.width()) {
        if (to.x() != from.x()) {
            int newY = from.y() + (to.y() - from.y()) * (image.width() - 1 - from.x()) / (to.x() - from.x());
            from.setY(newY);
            from.setX(image.width() - 1);
        }
    }
    if (from.y() < 0) {
        if (from.y() != to.y()) {
            int newX = from.x() + (to.x() - from.x()) * from.y() / (from.y() - to.y());
            from.setX(newX);
            from.setY(0);
        }
    }
    if (from.y() >= image.height()) {
        if (to.y() != from.y()) {
            int newX = from.x() + (to.x() - from.x()) * (image.height() - 1 - from.y()) / (to.y() - from.y());
            from.setX(newX);
            from.setY(image.height() - 1);
        }
    }
}

bool Board::tryDrawSimpleLine(QPoint &from, QPoint &to, QRgb color) {
    if (from.x() == to.x()) {
        drawVerticalLine(from, to, color);
        return true;
    } else if (from.y() == to.y()) {
        drawHorizontalLine(from, to, color);
        return true;
    } else {
        return false;
    }
}

void Board::drawLineBresenham(QPoint &from, QPoint &to, QRgb color) {
    if (abs(to.y() - from.y()) / abs(to.x() - from.x()) >= 1) {
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
    int x;
    int sign = to.y() > from.y() ? 1 : -1;
    QRgb *pixels = reinterpret_cast<QRgb *>(image.bits());
    for (x = from.x(); x <= to.x(); ++x) {
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
    int y;
    int sign = to.x() > from.x() ? 1 : -1;
    QRgb *pixels = reinterpret_cast<QRgb *>(image.bits());
    for (y = from.y(); y <= to.y(); ++y) {
        pixels[y * image.width() + x] = color;
        if (err > 0) {
            x += sign;
            err = err - dy;
        }
        err = err + dx;
    }
}

void Board::drawHorizontalLine(QPoint &from, QPoint &to, QRgb color) {
    if (from.y() < 0 || from.y() >= image.height()) {
        return;
    }
    if (from.x() > to.x()) {
        std::swap(from, to);
    }
    if (from.x() < 0) {
        from.setX(0);
    }
    if (to.x() >= image.width()) {
        to.setX(image.width() - 1);
    }
    if ((uchar) color == (uchar) (color >> 8) &&
            (uchar) color == (uchar) (color >> 16)) {
        uchar *line = image.bits() + from.y() * image.bytesPerLine();
        memset(line + from.x() * image.depth() / 8, color, 3 * (to.x() - from.x()));
    } else {
        QRgb *pixels = reinterpret_cast<QRgb *>(image.bits());
        for (int i = from.x(); i <= to.x(); ++i) {
            pixels[from.y() * image.width() + i] = color;
        }
    }
}

void Board::drawVerticalLine(QPoint &from, QPoint &to, QRgb color) {
    if (from.x() < 0 || from.x() >= image.width()) {
        return;
    }
    if (from.y() > to.y()) {
        std::swap(from, to);
    }
    if (from.y() < 0) {
        from.setY(0);
    }
    if (to.y() >= image.height()) {
        to.setY(image.height() - 1);
    }
    QRgb *pixels = reinterpret_cast<QRgb *>(image.bits());
    for (int j = from.y(); j <= to.y(); ++j) {
        pixels[j * image.width() + from.x()] = color;
    }
}

void Board::drawHexagon(QPoint start, uint size) {
    // width = sqrt(3)/2 * height
    start.setX(start.x() + sqrt(3) / 2 * size);
    // height = size * 2
    // vert = height * 3/4.
    start.setY(start.y() + size);
    drawLine(hexagonPoint(start, size, 0), hexagonPoint(start, size, 1), BlackColor);
    drawLine(hexagonPoint(start, size, 1), hexagonPoint(start, size, 2), BlackColor);
    drawLine(hexagonPoint(start, size, 2), hexagonPoint(start, size, 3), BlackColor);
    drawLine(hexagonPoint(start, size, 3), hexagonPoint(start, size, 4), BlackColor);
    drawLine(hexagonPoint(start, size, 4), hexagonPoint(start, size, 5), BlackColor);
    drawLine(hexagonPoint(start, size, 5), hexagonPoint(start, size, 0), BlackColor);
}

void Board::getSpans(int min_x, int max_x, int y, QRgb *pixels, QRgb oldValue, std::vector<SpanLine> &spans) {
    if (y < 0 || y >= image.height()) {
        return;
    }
    int x0 = - 1;
    // if left border is old value, go to find  new left border
    if (pixels[y * image.width() + min_x] == oldValue) {
        for (int i = min_x; i >= 0; --i) {
            if (pixels[y * image.width() + i] != oldValue) {
                x0 = i + 1;
                break;
            }
        }
        if (x0 == -1) {
            x0 = 0;
        }
    }
    for (int j = min_x + 1; j < image.width(); ++j) {
        if (pixels[y * image.width() + j] != oldValue) {
            if (x0 != -1) {
                spans.push_back({x0, j - 1, y});
                x0 = -1;
                if (j >= max_x) {
                    break;
                }
            }
        } else {
            if (x0 == -1) {
                if (j < max_x) {
                    x0 = j;
                } else {
                    break;
                }
            }
        }
    }
    if (x0 != -1) {
        spans.push_back({x0, image.width() - 1, y});
    }
}

void Board::spanFill(QPoint start, QRgb color) {
    if (start.y() < 0 || start.x() < 0 || start.y() >= image.height() || start.x() >= image.width()) {
        return;
    }
    QRgb *pixels = reinterpret_cast<QRgb *>(image.bits());
    QRgb oldValue = pixels[start.y() * image.width() + start.x()];
    int i, j;
    for (i = start.x() - 1; i >= 0; --i) {
        if (pixels[start.y() * image.width() + i] != oldValue) {
            break;
        }
    }
    for (j = start.x() + 1; j < image.width(); ++j) {
        if (pixels[start.y() * image.width() + j] != oldValue) {
            break;
        }
    }
    int start_min_x = i + 1;
    int start_max_x = j - 1;
    std::vector<SpanLine> spans;
    spans.push_back({start_min_x, start_max_x, start.y()});
    while (spans.size() > 0) {
        std::vector<SpanLine> newSpans;
        for (auto it = spans.begin(); it < spans.end(); ++it) {
            SpanLine &line = *it;
            getSpans(line.x0, line.x1, line.y + 1, pixels, oldValue, newSpans);
            getSpans(line.x0, line.x1, line.y - 1, pixels, oldValue, newSpans);
            drawLine(QPoint(line.x0, line.y), QPoint(line.x1, line.y), color);
        }
        spans = newSpans;
    }
}

void Board::paint() {
    fill(WhiteColor);
    /*drawLine(QPoint(300, 300), QPoint(300, 300), BlueColor);
    drawLine(QPoint(0, 0), QPoint(1024, 100), BlueColor);
    drawLine(QPoint(-100, 500), QPoint(50, 0), RedColor);
    drawLine(QPoint(-100, 500), QPoint(0, 90), RedColor);
    drawLine(QPoint(0, 50), QPoint(100, 10), BlackColor);
    drawLine(QPoint(-50, 50), QPoint(-10, 10), BlackColor);
    drawLine(QPoint(100, 100), QPoint(2000, 100), GreenColor);
    drawLine(QPoint(200, 100), QPoint(200, 200), GreenColor);*/
    static int offset;
    offset = QDateTime::currentMSecsSinceEpoch() / 10 % image.width();
    drawHexagon(QPoint(offset, 0), 50);
    spanFill(QPoint(offset + 30, 30), RedColor);
    drawHexagon(QPoint(offset + sqrt(3) * 50 + 1, 0), 50);
}

void Board::resizeEvent(QResizeEvent *) {
    image = QImage(this->size(), QImage::Format_RGB32);
}

void Board::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    paint();
    painter.drawImage(0, 0, image);
}
