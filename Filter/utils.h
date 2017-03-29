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

    union PixelUnion {
        uint32_t packed;
        struct {
            uchar r;
            uchar g;
            uchar b;
            uchar a;
        } rgba;
    };

    uint32_t inline getBilinearInterpolatedPixel(QImage &image, float x_norm, float y_norm) {
        PixelUnion rgba_union;
        int x = x_norm * image.width();
        int y = y_norm * image.height();
        float x_diff = x_norm * image.width() - x;
        float y_diff = y_norm * image.height() - y;
        uchar *channel = &rgba_union.rgba.r;
        for (int i = 0; i < 4; ++i) {
            channel[i] = image.bits()[y       * image.bytesPerLine() + x       * FilterUtils::RgbaDepth + i] * (1.0 - x_diff) * (1.0 - y_diff)
                       + (x >= image.width() ?  0 :
                         image.bits()[y       * image.bytesPerLine() + (x + 1) * FilterUtils::RgbaDepth + i] * x_diff * (1.0 - y_diff))
                       + (y >= image.height() ? 0 :
                         image.bits()[(y + 1) * image.bytesPerLine() + x       * FilterUtils::RgbaDepth + i] * y_diff * (1.0 - x_diff))
                       + (y >= image.height() || x >= image.width() ? 0 :
                         image.bits()[(y + 1) * image.bytesPerLine() + (x + 1) * FilterUtils::RgbaDepth + i] * x_diff * y_diff);
        }
        return rgba_union.packed;
    }

    QImage inline scaleImage(QImage &image, float scale_factor) {
        QImage scaled(scale_factor > 1 ? image.width() : image.width() * scale_factor,
                      scale_factor > 1 ? image.height() : image.height() * scale_factor,
                      QImage::Format_RGBA8888);
        uint32_t *bits = (uint32_t *) scaled.bits();
        int x_offset = scale_factor > 1 ? (image.width() - image.width() / scale_factor) / 2 : 0;
        int y_offset = scale_factor > 1 ? (image.height() - image.height() / scale_factor) / 2 : 0;
        for (int j = 0; j < scaled.height(); ++j) {
            for (int i = 0; i < scaled.width(); ++i) {
                float x_norm = (i + x_offset * scale_factor) / (image.width() * scale_factor);
                float y_norm = (j + y_offset * scale_factor) / (image.height() * scale_factor);
                bits[j * scaled.width() + i] = getBilinearInterpolatedPixel(image, x_norm, y_norm);
            }
        }
        return scaled;
    }

    QImage inline scaleImageToFit(QImage &image, int width, int height) {
        if (image.width() < width && image.height() < height) {
            return image;
        }
        float x_factor = (float) width / (float) image.width();
        float y_factor = (float) height / (float) image.height();
        return scaleImage(image, std::min(x_factor, y_factor));
    }
}

#endif // UTILS_H
