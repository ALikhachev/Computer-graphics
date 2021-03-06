#ifndef UTILS_H
#define UTILS_H

#include <QPoint>
#include <QDebug>
#include <algorithm>

namespace IsolinesUtils {
    static inline std::vector<std::pair<QPoint, QPoint>> handleCell1(std::vector<std::pair<QPoint, double>> &vertices,
                                                              double value,
                                                              int bigger) {
        auto comp = bigger == 1 ?
                    [] (const std::pair<QPoint, double> &v1, const std::pair<QPoint, double> &v2) -> bool {
                        return v1.second > v2.second;
                    }
                    :
                    [] (const std::pair<QPoint, double> &v1, const std::pair<QPoint, double> &v2) -> bool {
                        return v1.second < v2.second;
                    };
        std::partial_sort(vertices.begin(), vertices.begin() + bigger, vertices.end(), comp);
        std::pair<QPoint, double> point = vertices[0];
        QPoint start(point.first.x(), 0);
        QPoint end(0, point.first.y());

        for (auto it = vertices.begin() + 1; it < vertices.end(); ++it) {
            if (point.first.x() == it->first.x()) {
                int offset = point.second > it->second ? point.first.y() : it->first.y();
                int sign = offset == std::max(point.first.y(), it->first.y()) ? -1 : 1;
                start.setY(sign * std::fabs(value - std::max(point.second, it->second)) * std::abs(point.first.y() - it->first.y()) / std::fabs(point.second - it->second)
                           + offset);
            }
            if (point.first.y() == it->first.y()) {
                int offset = point.second > it->second ? point.first.x() : it->first.x();
                int sign = offset == std::max(point.first.x(), it->first.x()) ? -1 : 1;
                end.setX(sign * std::fabs(value - std::max(point.second, it->second)) * std::abs(point.first.x() - it->first.x()) / std::fabs(point.second - it->second)
                         + offset);
            }
        }
        return std::vector<std::pair<QPoint, QPoint>>{std::make_pair(start, end)};
    }

    static inline std::vector<std::pair<QPoint, QPoint>> handleCell2(std::vector<std::pair<QPoint, double>> &vertices,
                                                                     double value,
                                                                     double middle_value) {
        std::partial_sort(vertices.begin(), vertices.begin() + 2, vertices.end(), [] (const std::pair<QPoint, double> &v1, const std::pair<QPoint, double> &v2) -> bool {
            return v1.second > v2.second;
        });
        auto &p1 = vertices[0];
        auto &p2 = vertices[1];
        auto &p3 = vertices[2];
        auto &p4 = vertices[3];
        if (p1.first.x() == p2.first.x()) {
            QPoint start(0, p1.first.y());
            QPoint end(0, p2.first.y());
            if (p1.first.y() != p3.first.y()) {
                std::swap(p3, p4);
            }
            int offset = p1.first.x();
            int sign = offset == std::max(p1.first.x(), p3.first.x()) ? -1 : 1;
            start.setX(offset + sign * std::fabs(value - p1.second) * std::abs(p1.first.x() - p3.first.x()) / std::fabs(p1.second - p3.second));
            offset = p2.first.x();
            sign = offset == std::max(p2.first.x(), p4.first.x()) ? -1 : 1;
            end.setX(offset + sign * std::fabs(value - p2.second) * std::abs(p2.first.x() - p4.first.x()) / std::fabs(p2.second - p4.second));
            return std::vector<std::pair<QPoint, QPoint>>{std::make_pair(start, end)};
        } else if (p1.first.y() == p2.first.y()) {
            QPoint start(p1.first.x(), 0);
            QPoint end(p2.first.x(), 0);
            if (p1.first.x() != p3.first.x()) {
                std::swap(p3, p4);
            }
            int offset = p1.first.y();
            int sign = offset == std::max(p1.first.y(), p3.first.y()) ? -1 : 1;
            start.setY(offset + sign * std::fabs(value - p1.second) * std::abs(p1.first.y() - p3.first.y()) / std::fabs(p1.second - p3.second));
            offset = p2.first.y();
            sign = offset == std::max(p2.first.y(), p4.first.y()) ? -1 : 1;
            end.setY(offset + sign * std::fabs(value - p2.second) * std::abs(p2.first.y() - p4.first.y()) / std::fabs(p2.second - p4.second));
            return std::vector<std::pair<QPoint, QPoint>>{std::make_pair(start, end)};
        } else if (middle_value > value){
            QPoint start1(p1.first.x(), 0);
            QPoint end1(0, p2.first.y());
            QPoint start2(p2.first.x(), 0);
            QPoint end2(0, p1.first.y());
            if (p1.first.x() != p3.first.x()) {
                std::swap(p3, p4);
            }
            int offset = p2.first.x();
            int sign = offset == std::max(p2.first.x(), p3.first.x()) ? -1 : 1;
            end1.setX(offset + sign * std::fabs(value - p2.second) * std::abs(p2.first.x() - p3.first.x()) / std::fabs(p2.second - p3.second));
            offset = p1.first.y();
            sign = offset == std::max(p1.first.y(), p3.first.y()) ? -1 : 1;
            start1.setY(offset + sign * std::fabs(value - p1.second) * std::abs(p1.first.y() - p3.first.y()) / std::fabs(p1.second - p3.second));
            offset = p1.first.x();
            sign = offset == std::max(p1.first.x(), p4.first.x()) ? -1 : 1;
            end2.setX(offset + sign * std::fabs(value - p1.second) * std::abs(p4.first.x() - p1.first.x()) / std::fabs(p1.second - p4.second));
            offset = p2.first.y();
            sign = offset == std::max(p2.first.y(), p4.first.y()) ? -1 : 1;
            start2.setY(offset + sign * std::fabs(value - p2.second) * std::abs(p2.first.y() - p4.first.y()) / std::fabs(p2.second - p4.second));
            return std::vector<std::pair<QPoint, QPoint>>{std::make_pair(start1, end1), std::make_pair(start2, end2)};
        } else {
            QPoint start1(p1.first.x(), 0);
            QPoint end1(0, p1.first.y());
            QPoint start2(p2.first.x(), 0);
            QPoint end2(0, p2.first.y());
            if (p1.first.x() != p3.first.x()) {
                std::swap(p3, p4);
            }
            int offset = p1.first.x();
            int sign = offset == std::max(p1.first.x(), p4.first.x()) ? -1 : 1;
            end1.setX(offset + sign * std::fabs(value - p1.second) * std::abs(p1.first.x() - p4.first.x()) / std::fabs(p1.second - p4.second));
            offset = p1.first.y();
            sign = offset == std::max(p1.first.y(), p3.first.y()) ? -1 : 1;
            start1.setY(offset + sign * std::fabs(value - p1.second) * std::abs(p1.first.y() - p3.first.y()) / std::fabs(p1.second - p3.second));
            offset = p2.first.x();
            sign = offset == std::max(p2.first.x(), p3.first.x()) ? -1 : 1;
            end2.setX(offset + sign * std::fabs(value - p2.second) * std::abs(p2.first.x() - p3.first.x()) / std::fabs(p2.second - p3.second));
            offset = p2.first.y();
            sign = offset == std::max(p2.first.y(), p4.first.y()) ? -1 : 1;
            start2.setY(offset + sign * std::fabs(value - p2.second) * std::abs(p2.first.y() - p4.first.y()) / std::fabs(p2.second - p4.second));
            return std::vector<std::pair<QPoint, QPoint>>{std::make_pair(start1, end1), std::make_pair(start2, end2)};
        }
    }

    inline std::vector<std::pair<QPoint, QPoint>> handleCell(std::vector<std::pair<QPoint, double>> &vertices,
                                                             double value, double middle_value) {
        std::vector<std::pair<QPoint, QPoint>> isolines;
        int bigger = 0;
        for (auto it = vertices.begin(); it < vertices.end(); ++it) {
            if (it->second > value) {
                ++bigger;
            }
        };
        if (bigger == 4 || bigger == 0) {
            return std::vector<std::pair<QPoint, QPoint>>();
        }
        if (bigger == 1 || bigger == 3) {
            return handleCell1(vertices, value, bigger);
        }
        if (bigger == 2) {
            return handleCell2(vertices, value, middle_value);
        }
        return std::vector<std::pair<QPoint, QPoint>>();
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
}

#endif // UTILS_H
