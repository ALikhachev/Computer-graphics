#include "floyd_steinberg_filter.h"
#include "filter_registry.h"
#include "utils.h"

using FilterUtils::PixelUnion;
using FilterUtils::validateColor;

FilterSettings *FloydSteinbergFilterSettings::clone() {
    return new FloydSteinbergFilterSettings(*this);
}

FloydSteinbergFilter::FloydSteinbergFilter()
{

}

QImage FloydSteinbergFilter::applyFilter(QImage image, std::function<void(int)> updateProgress) {
    PixelUnion *pixels = (PixelUnion *) image.bits();
    FloydSteinbergFilterSettings settings = this->settings;
    for (int j = 0; j < image.height() - 1; ++j) {
        for (int i = 1; i < image.width() - 1; ++i) {
            for (int k = 0; k < 3; ++k) {
                uchar old_pixel = ((uchar *) &pixels[j * image.width() + i].rgba.r)[k];
                uchar new_pixel = getClosestColor(old_pixel, ((int *) &settings.red)[k]);
                ((uchar *) &pixels[j * image.width() + i].rgba.r)[k] = new_pixel;
                int error = old_pixel - new_pixel;
                int new_color_1_0 = ((uchar *) &pixels[ j      * image.width() + i + 1].rgba.r)[k] + error * 7.0/16.0;
                int new_color_m1_1 = ((uchar *) &pixels[(j + 1) * image.width() + i - 1].rgba.r)[k] + error * 3.0/16.0;
                int new_color_0_1 = ((uchar *) &pixels[(j + 1) * image.width() + i    ].rgba.r)[k] + error * 5.0/16.0;
                int new_color_1_1 = ((uchar *) &pixels[(j + 1) * image.width() + i + 1].rgba.r)[k] + error * 1.0/16.0;
                ((uchar *) &pixels[ j      * image.width() + i + 1].rgba.r)[k] = validateColor(new_color_1_0);
                ((uchar *) &pixels[(j + 1) * image.width() + i - 1].rgba.r)[k] = validateColor(new_color_m1_1);
                ((uchar *) &pixels[(j + 1) * image.width() + i    ].rgba.r)[k] = validateColor(new_color_0_1);
                ((uchar *) &pixels[(j + 1) * image.width() + i + 1].rgba.r)[k] = validateColor(new_color_1_1);
            }
        }
        updateProgress(100 * j / image.height());
    }
    return image;
}

uchar FloydSteinbergFilter::getClosestColor(uchar color, int shades) {
    float period = 255.0 / (float) (shades - 1);
    return period * (int) ((color + period / 2) / period);
}

QIcon FloydSteinbergFilter::getIcon() {
    return QIcon(":/icons/floyd_steinberg.png");
}

QString FloydSteinbergFilter::getName() {
    return "Floyd-steinberg";
}

FloydSteinbergFilterSettings *FloydSteinbergFilter::getSettings() {
    return &this->settings;
}

void FloydSteinbergFilter::setSettings(FilterSettings *settings) {
    FloydSteinbergFilterSettings *w_settings = dynamic_cast<FloydSteinbergFilterSettings *>(settings);
    if (w_settings) {
        this->settings = *w_settings;
    }
}

namespace {
    bool reg = FilterRegistry::getInstance().registerFilter(new FloydSteinbergFilter);
}
