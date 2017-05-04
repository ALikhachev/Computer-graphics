#ifndef DRAWING_H
#define DRAWING_H

#include <QPoint>
#include <QColor>
#include <QVector3D>
#include "wire_object.h"
#include "line3d.h"

namespace Drawing {
    static inline void fill(QImage &image, QRgb color) {
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

    static inline void drawLineBresenhamY(QImage &image, QPoint &from, QPoint &to, QRgb color) {
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
                err = err - 2 * dy;
            }
            err = err + 2 * dx;
        }
    }

    static inline void drawLineBresenham(QImage &image, QPoint &from, QPoint &to, QRgb color) {
        if (abs(to.y() - from.y()) >= abs(to.x() - from.x())) {
            drawLineBresenhamY(image, from, to, color);
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
                err = err - 2 * dx;
            }
            err = err + 2 * dy;
        }
    }

    static inline void drawHorizontalLine(QImage &image, QPoint &from, QPoint &to, QRgb color) {
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
        if (from.x() >= image.width()) {
            return;
        }
        if ((uchar) color == (uchar) (color >> 8) &&
                (uchar) color == (uchar) (color >> 16)) {
            uchar *line = image.bits() + from.y() * image.bytesPerLine();
            memset(line + from.x() * image.depth() / 8, color, image.depth() / 8 * (to.x() - from.x()));
        } else {
            QRgb *pixels = reinterpret_cast<QRgb *>(image.bits());
            for (int i = from.x(); i <= to.x(); ++i) {
                pixels[from.y() * image.width() + i] = color;
            }
        }
    }

    static inline void drawVerticalLine(QImage &image, QPoint &from, QPoint &to, QRgb color) {
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
        if (from.y() >= image.height()) {
            return;
        }
        QRgb *pixels = reinterpret_cast<QRgb *>(image.bits());
        for (int j = from.y(); j <= to.y(); ++j) {
            pixels[j * image.width() + from.x()] = color;
        }
    }

    static inline bool tryDrawSimpleLine(QImage &image, QPoint &from, QPoint &to, QRgb color) {
        if (from.x() == to.x()) {
           drawVerticalLine(image, from, to, color);
           return true;
       } else if (from.y() == to.y()) {
           drawHorizontalLine(image, from, to, color);
           return true;
       } else {
           return false;
       }
    }

    static inline void clipLine(QImage &image, QPoint &from, QPoint &to) {
        bool b[4] = {
            from.x() < 0,
            from.x() >= image.width(),
            from.y() < 0,
            from.y() >= image.height()
        };
        if (b[0]) {
            if (from.x() != to.x()) {
                int newY = from.y() + (to.y() - from.y()) * from.x() / (from.x() - to.x());
                from.setY(newY);
                from.setX(0);
            }
        }
        if (b[1]) {
            if (to.x() != from.x()) {
                int newY = from.y() + (to.y() - from.y()) * (image.width() - 1 - from.x()) / (to.x() - from.x());
                from.setY(newY);
                from.setX(image.width() - 1);
            }
        }
        if (b[2]) {
            if (from.y() != to.y()) {
                int newX = from.x() + (to.x() - from.x()) * from.y() / (from.y() - to.y());
                from.setX(newX);
                from.setY(0);
            }
        }
        if (b[3]) {
            if (to.y() != from.y()) {
                int newX = from.x() + (to.x() - from.x()) * (image.height() - 1 - from.y()) / (to.y() - from.y());
                from.setX(newX);
                from.setY(image.height() - 1);
            }
        }
    }

    inline void drawLine(QImage &image, QPoint from, QPoint to, QRgb color) {
        if (!tryDrawSimpleLine(image, from, to, color)) {
            clipLine(image, from, to);
            if (from.x() == to.x() || from.y() == to.y()) {
                return;
            }
            clipLine(image, to, from);
            if (from.x() == to.x() || from.y() == to.y()) {
                return;
            }
            clipLine(image, from, to);
            if (from.x() == to.x() || from.y() == to.y()) {
                return;
            }
            clipLine(image, to, from);
            if (from.x() == to.x() || from.y() == to.y()) {
                return;
            }
            drawLineBresenham(image, from, to, color);
        }
    }

    inline void setPixel(QImage &image, int x, int y, QRgb color) {
        if (x < 0 || x >= image.width() || y < 0 || y >= image.height()) return;
        QRgb *pixels = reinterpret_cast<QRgb *>(image.bits());
        pixels[y * image.width() + x] = color;
    }

    inline void setPixelUnsafe(QImage &image, int x, int y, QRgb color) {
        QRgb *pixels = reinterpret_cast<QRgb *>(image.bits());
        pixels[y * image.width() + x] = color;
    }

    inline void drawCircleUnsafe(QImage &image, QPoint center, int radius, QRgb color) {
        int x = 0;
        int y = radius;
        int delta = 1 - 2 * radius;
        int error = 0;
        while (y >= 0) {
            setPixelUnsafe(image, center.x() + x, center.y() + y, color);
            setPixelUnsafe(image, center.x() + x, center.y() - y, color);
            setPixelUnsafe(image, center.x() - x, center.y() + y, color);
            setPixelUnsafe(image, center.x() - x, center.y() - y, color);
            error = 2 * (delta + y) - 1;
            if ((delta < 0) && (error <= 0)) {
                delta += 2 * ++x + 1;
                continue;
            }
            error = 2 * (delta - x) - 1;
            if ((delta > 0) && (error > 0)) {
                delta += 1 - 2 * --y;
                continue;
            }
            x++;
            delta += 2 * (x - y);
            --y;
        }
    }

    inline void drawCircle(QImage &image, QPoint center, int radius, QRgb color) {
        if (center.x() > radius + 1 && center.x() < image.width() - 1 - radius - 1
                && center.y() > radius + 1 && center.y() < image.height() - 1 - radius - 1) {
            drawCircleUnsafe(image, center, radius, color);
        }
        int x = 0;
        int y = radius;
        int delta = 1 - 2 * radius;
        int error = 0;
        while (y >= 0) {
            setPixel(image, center.x() + x, center.y() + y, color);
            setPixel(image, center.x() + x, center.y() - y, color);
            setPixel(image, center.x() - x, center.y() + y, color);
            setPixel(image, center.x() - x, center.y() - y, color);
            error = 2 * (delta + y) - 1;
            if ((delta < 0) && (error <= 0)) {
                delta += 2 * ++x + 1;
                continue;
            }
            error = 2 * (delta - x) - 1;
            if ((delta > 0) && (error > 0)) {
                delta += 1 - 2 * --y;
                continue;
            }
            x++;
            delta += 2 * (x - y);
            --y;
        }
    }

    inline void drawLine3D(QImage &image, const QVector3D &from, const QVector3D &to, QColor color)
    {
        QPoint from2D(qRound(from.x() + 150), qRound(image.height() - 150 - 1 - from.y()));
        QPoint to2D(qRound(to.x() + 150), qRound(image.height() - 150 - 1 - to.y()));
        drawLine(image, from2D, to2D, color.rgb());
    }
}

#endif // DRAWING_H
