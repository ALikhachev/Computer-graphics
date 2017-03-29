#include "convolution_filter.h"
#include "filter_registry.h"
#include "utils.h"

#include <QDebug>

using FilterUtils::PixelUnion;
using FilterUtils::PixelFloat;
using FilterUtils::reflect;

QImage ConvolutionFilter::applyFilter(QImage image, std::function<void(int)> updateProgress) {
    QImage filtered_image(image.size(), QImage::Format_RGBA8888);
    PixelUnion *pixels = (PixelUnion *) filtered_image.bits();
    PixelUnion *source_pixels = (PixelUnion *) image.bits();
    std::vector<float> matrix = this->getMatrix();
    int m = this->getMatrixSize();
    for (int j = 0; j < filtered_image.height(); ++j) {
        for (int i = 0; i < filtered_image.width(); ++i) {
            PixelFloat sum;
            for (int k = 0; k < m; ++k) {
                for (int l = 0; l < m; ++l) {
                    int x = reflect(image.width(), i + l - m / 2);
                    int y = reflect(image.height(), j + k - m / 2);
                    for (int i1 = 0; i1 < 4; ++i1) {
                        ((float *) &(sum.r))[i1] += matrix[k * m + l] * ((uchar *) &(source_pixels[y * image.width() + x].rgba.r))[i1];
                    }
                }
            }
            for (int i1 = 0; i1 < 4; ++i1) {
                int v = (int) ((float *) &(sum.r))[i1];
                if (v < 0) {
                    v = 0;
                }
                if (v > 255) {
                    v = 255;
                }
                ((uchar *) &(pixels[j * filtered_image.width() + i].rgba.r))[i1] = v;
            }
        }
        updateProgress(100 * j / filtered_image.height());
    }
    return filtered_image;
}
