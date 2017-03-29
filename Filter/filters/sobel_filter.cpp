#include "sobel_filter.h"
#include "filter_registry.h"
#include "utils.h"
#include "filters/grayscale_filter.h"

using FilterUtils::PixelUnion;
using FilterUtils::PixelInt;
using FilterUtils::reflect;

SobelFilter::SobelFilter()
{
}

QImage SobelFilter::applyFilter(QImage image, std::function<void(int)> updateProgress) {
    QImage filtered_image(image.size(), QImage::Format_RGBA8888);
    GrayscaleFilter grayscale_filter;
    image = grayscale_filter.applyFilter(image, [updateProgress] (int i) {
        updateProgress(30.0 * i / 100.0);
    });
    PixelUnion *pixels = (PixelUnion *) filtered_image.bits();
    PixelUnion *source_pixels = (PixelUnion *) image.bits();
    int threshold = this->settings.threshold;
    int m = 3;
    std::vector<int> matrix_x({
        -1, 0, 1,
        -2, 0, 2,
        -1, 0, 1
    });
    std::vector<int> matrix_y({
        -1, -2, -1,
         0,  0,  0,
         1,  2,  1
    });
    for (int j = 0; j < filtered_image.height(); ++j) {
        for (int i = 0; i < filtered_image.width(); ++i) {
            int gx = 0;
            int gy = 0;
            for (int k = 0; k < m; ++k) {
                for (int l = 0; l < m; ++l) {
                    int x = reflect(image.width(), i + l - m / 2);
                    int y = reflect(image.height(), j + k - m / 2);
                    gx += matrix_x[k * m + l] * source_pixels[y * image.width() + x].rgba.r;
                    gy += matrix_y[k * m + l] * source_pixels[y * image.width() + x].rgba.r;
                }
            }
            int v = std::sqrt(gx * gx + gy * gy);
            if (v > threshold) {
                v = 255;
            } else {
                v = 0;
            }
            pixels[j * filtered_image.width() + i].rgba.r = v;
            pixels[j * filtered_image.width() + i].rgba.g = v;
            pixels[j * filtered_image.width() + i].rgba.b = v;
            pixels[j * filtered_image.width() + i].rgba.a = source_pixels[j * filtered_image.width() + i].rgba.a;
        }
        updateProgress(70 * j / filtered_image.height() + 30);
    }
    return filtered_image;
}

QIcon SobelFilter::getIcon() {
    return QIcon(":/icons/sobel.png");
}

QString SobelFilter::getName() {
    return tr("Sobel");
}

SobelFilterSettings *SobelFilter::getSettings() {
    return &this->settings;
}

namespace {
    bool reg = FilterRegistry::getInstance().registerFilter(new SobelFilter);
}
