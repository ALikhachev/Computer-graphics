#include "filters/watercolor_filter.h"
#include "filter_registry.h"
#include "utils.h"

using FilterUtils::PixelUnion;
using FilterUtils::reflect;

WatercolorFilter::WatercolorFilter() :
    matrix({ 0.0, -1.0,  0.0,
            -1.0,  5.0, -1.0,
             0.0, -1.0,  0.0})
{
}

QImage WatercolorFilter::applyFilter(QImage image, std::function<void(int)> updateProgress) {
    QImage filtered_image(image.size(), QImage::Format_RGBA8888);
    std::vector<int> neighbors[4];
    PixelUnion *source_pixels = (PixelUnion *) image.bits();
    PixelUnion *pixels = (PixelUnion *) filtered_image.bits();
    for (int j = 0; j < filtered_image.height(); ++j) {
        for (int i = 0; i < filtered_image.width(); ++i) {
            for (int k = -5; k < 5; ++k) {
                for (int l = -5; l < 5; ++l) {
                    int x = reflect(image.width(), i + l);
                    int y = reflect(image.height(), j + k);
                    for (int i1 = 0; i1 < 4; ++i1) {
                        neighbors[i1].push_back(((uchar *) &(source_pixels[y * image.width() + x].rgba.r))[i1]);
                    }
                }
            }
            for (int i1 = 0; i1 < 4; ++i1) {
                std::sort(neighbors[i1].begin(), neighbors[i1].end());
                ((uchar *) &(pixels[j * filtered_image.width() + i].rgba.r))[i1] = neighbors[i1][neighbors[i1].size() / 2];
                neighbors[i1].clear();
            }
        }
        updateProgress(50 * j / filtered_image.height());
    }
    return ConvolutionFilter::applyFilter(filtered_image, [updateProgress] (int progress) {
        updateProgress(progress * 0.5 + 50);
    });
}

std::vector<float> &WatercolorFilter::getMatrix() {
    return matrix;
}

int WatercolorFilter::getMatrixSize() {
    return 3;
}

QIcon WatercolorFilter::getIcon() {
    return QIcon(":/icons/watercolor.png");
}

QString WatercolorFilter::getName() {
    return tr("Watercolor");
}

WatercolorFilterSettings *WatercolorFilter::getSettings() {
    return &this->settings;
}

namespace {
    bool reg = FilterRegistry::getInstance().registerFilter(new WatercolorFilter);
}
