#include "rotate_filter.h"
#include "filter_registry.h"
#include "utils.h"

using FilterUtils::getBilinearInterpolatedPixel;
using FilterUtils::PixelUnion;

FilterSettings *RotateFilterSettings::clone() {
    return new RotateFilterSettings(*this);
}

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
            int x = (i - image.width() / 2) * std::cos(angle_rad) - (j - image.height() / 2) * std::sin(angle_rad);
            int y = (i - image.width() / 2) * std::sin(angle_rad) + (j - image.height() / 2) * std::cos(angle_rad);
            pixels[j * filtered_image.width() + i].packed =
                    getBilinearInterpolatedPixel(image, (float) x / image.width(), (float) y / image.height(),
                                                 image.width() / 2, image.height() / 2);
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

void RotateFilter::setSettings(FilterSettings *settings) {
    RotateFilterSettings *w_settings = dynamic_cast<RotateFilterSettings *>(settings);
    if (w_settings) {
        this->settings = *w_settings;
    }
}

namespace {
    bool reg = FilterRegistry::getInstance().registerFilter(new RotateFilter);
}
