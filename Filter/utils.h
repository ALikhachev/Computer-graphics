#ifndef UTILS_H
#define UTILS_H

#include <QImage>

namespace FilterUtils {
    const int RgbaDepth = 4;
    const int DashLength = 3;

    void inline emptyImage(QImage &image) {
        memset(image.bits(), 0xFF, image.height() * image.bytesPerLine());
    }

    void inline drawDashedRect(QImage &image, int x, int y, int width, int height) {
        int w_periods = width / DashLength;
        int h_periods = height / DashLength;
        for (int i = 0; i < w_periods; i += 2) {
            // top border
            memset(image.bits() + y * image.bytesPerLine() + x * RgbaDepth + i * DashLength * RgbaDepth, 0, DashLength * RgbaDepth);
            // bottom border
            memset(image.bits() + (y + height - 1) * image.bytesPerLine() + x * RgbaDepth + i * DashLength * RgbaDepth, 0, DashLength * RgbaDepth);
            for (int j = i * DashLength; j < (i + 1) * DashLength; ++j) {
                image.bits()[y * image.bytesPerLine() + (x + j) * RgbaDepth + 3] = 0xFF; // top
                image.bits()[(y + height - 1) * image.bytesPerLine() + (x + j) * RgbaDepth + 3] = 0xFF; // bottom
            }
        }
        for (int i = 0; i < h_periods; i += 2) {
            for (int j = i * DashLength; j < (i + 1) * DashLength; ++j) {
                // right border
                memset(image.bits() + (y + j) * image.bytesPerLine() + (x + width - 1) * RgbaDepth, 0, RgbaDepth - 1);
                image.bits()[(y + j) * image.bytesPerLine() + (x + width - 1) * RgbaDepth + 3] = 0xFF;
                // left border
                memset(image.bits() + (y + j) * image.bytesPerLine() + x * RgbaDepth, 0, RgbaDepth - 1);
                image.bits()[(y + j) * image.bytesPerLine() + x * RgbaDepth + 3] = 0xFF;
            }
        }
    }
}

#endif // UTILS_H
