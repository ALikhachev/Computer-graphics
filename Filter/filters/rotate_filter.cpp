#include "rotate_filter.h"
#include "filter_registry.h"
#include "utils.h"

using FilterUtils::getBilinearInterpolatedPixel;
using FilterUtils::PixelUnion;

RotateFilter::RotateFilter()
{

}

const float PI_F = 3.14159265358979f;

QImage RotateFilter::applyFilter(QImage image, std::function<void(int)> updateProgress) {
    QImage filtered_image(image.size(), QImage::Format_RGBA8888);
    float angle_rad = (float) this->settings.angle / 180.0 * PI_F;

    PixelUnion *pixels = (PixelUnion *) filtered_image.bits();
    for (int j = 0; j < image.height(); ++j) {
        for (int i = 0; i < image.width(); ++i) {
            int x = i * std::cos(angle_rad) - j * std::sin(angle_rad);
            int y = i * std::sin(angle_rad) + j * std::cos(angle_rad);
            pixels[j * filtered_image.width() + i].packed = getBilinearInterpolatedPixel(image, (float) x / image.width(), (float) y / image.height());
        }
        updateProgress(100 * j / filtered_image.height());
    }
    return filtered_image;
}

QIcon RotateFilter::getIcon() {
    return QIcon(":/icons/rotate.png");
}

QString RotateFilter::getName() {
    return "Rotate";
}

RotateFilterSettings *RotateFilter::getSettings() {
    return &this->settings;
}

namespace {
    bool reg = FilterRegistry::getInstance().registerFilter(new RotateFilter);
}
