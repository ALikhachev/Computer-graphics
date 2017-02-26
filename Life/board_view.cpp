#include <QDateTime>

#include "board_view.h"

BoardView::BoardView(Board *board, QWidget *parent) :
    QWidget(parent),
    timer(this),
    board(board),
    hex_qrheight(board->getSettings()->cellSize / 2),
    hex_semiwidth(board->getSettings()->cellSize * sqrt(3) / 2),
    top_coeff((double)hex_qrheight/(double)hex_semiwidth)
{
    resize(hex_semiwidth * 2 * board->getWidth() + 1, board->getHeight() * hex_qrheight * 3 + hex_qrheight);
    image = QImage(this->size(), QImage::Format_RGB32);
    connect(&timer, SIGNAL(timeout()), this, SLOT(update()));
    timer.start(33);
    drawHexagonOutlines();
    board->subscribe(this);
}

void BoardView::fill(QRgb color) {
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

void BoardView::drawLine(QPoint from, QPoint to, QRgb color) {
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
void BoardView::clipLine(QPoint &from, QPoint &to) {
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

bool BoardView::tryDrawSimpleLine(QPoint &from, QPoint &to, QRgb color) {
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

void BoardView::drawLineBresenham(QPoint &from, QPoint &to, QRgb color) {
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

void BoardView::drawLineBresenhamY(QPoint &from, QPoint &to, QRgb color) {
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

void BoardView::drawHorizontalLine(QPoint &from, QPoint &to, QRgb color) {
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

void BoardView::drawVerticalLine(QPoint &from, QPoint &to, QRgb color) {
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

void BoardView::drawHexagon(QPoint start) {
    QPoint vertices[6] = {
        QPoint(start.x(), start.y() + hex_qrheight),
        QPoint(start.x() + hex_semiwidth, start.y()),
        QPoint(start.x() + hex_semiwidth * 2, start.y() + hex_qrheight),
        QPoint(start.x() + hex_semiwidth * 2, start.y() + hex_qrheight * 3),
        QPoint(start.x() + hex_semiwidth, start.y() + hex_qrheight * 4),
        QPoint(start.x(), start.y() + hex_qrheight * 3),
    };
    drawLine(vertices[0], vertices[1], BlackColor);
    drawLine(vertices[1], vertices[2], BlackColor);
    drawLine(vertices[2], vertices[3], BlackColor);
    drawLine(vertices[3], vertices[4], BlackColor);
    drawLine(vertices[4], vertices[5], BlackColor);
    drawLine(vertices[0], vertices[5], BlackColor);
}

void BoardView::getSpans(int min_x, int max_x, int y, QRgb *pixels, QRgb oldValue, std::vector<SpanLine> &spans) {
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

void BoardView::spanFill(QPoint start, QRgb color) {
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

void BoardView::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    painter.drawImage(0, 0, image);
}

void BoardView::mousePressEvent(QMouseEvent * event) {
    int x = event->x();
    int y = event->y();
    int row = static_cast<int>(y / (hex_qrheight * 3));
    bool rowIsOdd = row % 2 == 1;
    int column = rowIsOdd ? static_cast<int>((x - hex_semiwidth) / (hex_semiwidth * 2)) : static_cast<int>(x / (hex_semiwidth * 2));
    double relY = y - (row * hex_qrheight * 3);
    double relX;

    if (rowIsOdd) {
        relX = (x - (column * hex_semiwidth * 2)) - hex_semiwidth;
    } else {
        relX = x - (column * hex_semiwidth * 2);
    }

    if (relY < (-top_coeff * relX) + hex_qrheight) {
        // left triangle
        row--;
        if (!rowIsOdd) {
            column--;
        }
    }
    else if (relY < (top_coeff * relX) - hex_qrheight) {
        // right triangle
        row--;
        if (rowIsOdd) {
           column++;
        }
    }
    board->invertCell(column, row);
}

void BoardView::drawHexagonOutlines() {
    fill(WhiteColor);
    std::vector<Cell> &state = board->getState();
    int horizontal_offset = hex_semiwidth * 2;
    int vertical_offset = hex_qrheight * 3;
    for (quint32 j = 0; j < board->getHeight(); ++j) {
        for (quint32 i = 0; i < board->getWidth(); ++i) {
            if (j % 2 == 1) {
                drawHexagon(QPoint(horizontal_offset / 2 + horizontal_offset * i, vertical_offset * j));
                if (state[j * board->getWidth() + i].alive) {
                    spanFill(QPoint(horizontal_offset / 2 + horizontal_offset * i + 1, vertical_offset * j + hex_qrheight + 1), qRgb(16, 202, 90));
                }
                if (i == board->getWidth() - 2) {
                    break;
                }
            } else {
                drawHexagon(QPoint(horizontal_offset * i, vertical_offset * j));
                if (state[j * board->getWidth() + i].alive) {
                    spanFill(QPoint(horizontal_offset * i + 1, vertical_offset * j + hex_qrheight + 1), qRgb(16, 202, 90));
                }
            }
        }
    }
}

void BoardView::onCellStateChanged(quint32 x, quint32 y, const Cell &cell) {
    if (cell.alive) {
        spanFill(QPoint((y & 1) * hex_semiwidth + hex_semiwidth * 2 * x + 1, hex_qrheight * 3 * y + hex_qrheight + 1), qRgb(16, 202, 90));
    } else {
        spanFill(QPoint((y & 1) * hex_semiwidth + hex_semiwidth * 2 * x + 1, hex_qrheight * 3 * y + hex_qrheight + 1), WhiteColor);
    }
}
