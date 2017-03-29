#include "convolution_filter.h"
#include "filter_registry.h"
#include "utils.h"

using FilterUtils::PixelUnion;
using FilterUtils::reflect;

QImage ConvolutionFilter::applyFilter(QImage image, std::function<void(int)> updateProgress) {
    QImage filtered_image(image.size(), QImage::Format_RGBA8888);
    PixelUnion *pixels = (PixelUnion *) filtered_image.bits();
    std::vector<float> matrix = this->getMatrix();
    for (int j = 0; j < filtered_image.height(); ++j) {
        for (int i = 0; i < filtered_image.width(); ++i) {
            int m = this->getMatrixSize();
            float sum = 0;
            for (int k = 0; k < m; ++k) {
                for (int l = 0; l < m; ++l) {
                    int x = reflect(filtered_image.width(), i);
                    int y = reflect(filtered_image.height(), j);
                    sum += matrix[k * m + l] * y + x;
                }
            }
            pixels[j * filtered_image.width() + i] = sum;
        }
        updateProgress(100 * j / filtered_image.height());
    }
    return filtered_image;
}
