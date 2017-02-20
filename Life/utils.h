#ifndef UTILS_H
#define UTILS_H

#include <cmath>

namespace Utils {
    inline QPoint hexagonPoint(QPoint center, uint size, uint i) {
        int angle_deg = 60 * i   + 30;
        double angle_rad = M_PI / 180 * angle_deg;
        return QPoint(center.x() + size * cos(angle_rad),
                     center.y() + size * sin(angle_rad));
    }
}

#endif // UTILS_H
