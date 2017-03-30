#include "filters/edge_filter.h"
#include "filter_registry.h"
#include "utils.h"
#include "grayscale_filter.h"

using FilterUtils::PixelUnion;
using FilterUtils::PixelInt;
using FilterUtils::reflect;

FilterSettings *EdgeFilterSettings::clone() {
    return new EdgeFilterSettings(*this);
}

EdgeFilter::EdgeFilter() :
    matrix({ 0.0, -1.0,  0.0,
            -1.0,  4.0, -1.0,
             0.0, -1.0, 0.0})
{
}

QImage EdgeFilter::applyFilter(QImage image, std::function<void(int)> updateProgress) {
    QImage filtered_image(image.size(), QImage::Format_RGBA8888);
    GrayscaleFilter grayscale_filter;
    image = grayscale_filter.applyFilter(image, [updateProgress] (int i) {
        updateProgress(30.0 * i / 100.0);
    });
    PixelUnion *pixels = (PixelUnion *) filtered_image.bits();
    PixelUnion *source_pixels = (PixelUnion *) image.bits();
    std::vector<float> matrix = this->getMatrix();
    int m = this->getMatrixSize();
    int threshold = this->settings.threshold;
    for (int j = 0; j < filtered_image.height(); ++j) {
        for (int i = 0; i < filtered_image.width(); ++i) {
            int sum = 0;
            for (int k = 0; k < m; ++k) {
                for (int l = 0; l < m; ++l) {
                    int x = reflect(image.width(), i + l - m / 2);
                    int y = reflect(image.height(), j + k - m / 2);
                    sum += matrix[k * m + l] * source_pixels[y * image.width() + x].rgba.r;
                }
            }
            if (sum > threshold) {
                sum = 255;
            } else {
                sum = 0;
            }
            pixels[j * filtered_image.width() + i].rgba.r = sum;
            pixels[j * filtered_image.width() + i].rgba.g = sum;
            pixels[j * filtered_image.width() + i].rgba.b = sum;
            pixels[j * filtered_image.width() + i].rgba.a = source_pixels[j * filtered_image.width() + i].rgba.a;
        }
        updateProgress(70 * j / filtered_image.height() + 30);
    }
    return filtered_image;
}

QIcon EdgeFilter::getIcon() {
    return QIcon(":/icons/edge.png");
}

QString EdgeFilter::getName() {
    return tr("Edge");
}

std::vector<float> &EdgeFilter::getMatrix() {
    return this->matrix;
}

int EdgeFilter::getMatrixSize() {
    return 3;
}

EdgeFilterSettings *EdgeFilter::getSettings() {
    return &this->settings;
}

void EdgeFilter::setSettings(FilterSettings *settings) {
    EdgeFilterSettings *w_settings = dynamic_cast<EdgeFilterSettings *>(settings);
    if (w_settings) {
        this->settings = *w_settings;
    }
}

namespace {
    bool reg = FilterRegistry::getInstance().registerFilter(new EdgeFilter);
}
