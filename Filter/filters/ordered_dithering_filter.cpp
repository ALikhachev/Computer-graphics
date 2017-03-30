#include "ordered_dithering_filter.h"
#include "filter_registry.h"
#include "utils.h"

using FilterUtils::PixelUnion;

FilterSettings *OrderedDitheringFilterSettings::clone() {
    return new OrderedDitheringFilterSettings(*this);
}

OrderedDitheringFilter::OrderedDitheringFilter()
{

}

QImage OrderedDitheringFilter::applyFilter(QImage image, std::function<void(int)> updateProgress) {
    QImage filtered_image(image.size(), QImage::Format_RGBA8888);
    PixelUnion *pixels = (PixelUnion *) filtered_image.bits();
    PixelUnion *source_pixels = (PixelUnion *) image.bits();
    int matrix_size = this->settings.matrix_size;
    std::vector<float> matrix = buildMatrix(matrix_size);
    for (int j = 0; j < filtered_image.height(); ++j) {
        for (int i = 0; i < filtered_image.width(); ++i) {
            int matrix_value = matrix[(j % matrix_size) * matrix_size + (i % matrix_size)] * 255;
            for (int k = 0; k < 3; ++k) {
                uchar pixel_value = ((uchar *)(&source_pixels[j * image.width() + i].rgba.r))[k];
                ((uchar *)(&pixels[j * image.width() + i].rgba.r))[k] =
                        pixel_value > matrix_value ? 255 : 0;
            }
            pixels[j * filtered_image.width() + i].rgba.a = source_pixels[j * filtered_image.width() + i].rgba.a;
        }
        updateProgress(100 * j / filtered_image.height());
    }
    return filtered_image;
}

QIcon OrderedDitheringFilter::getIcon() {
    return QIcon(":/icons/ordered_dithering.png");
}

QString OrderedDitheringFilter::getName() {
    return "Ordered dithering";
}

OrderedDitheringFilterSettings *OrderedDitheringFilter::getSettings() {
    return &this->settings;
}

void OrderedDitheringFilter::setSettings(FilterSettings *settings) {
    OrderedDitheringFilterSettings *w_settings = dynamic_cast<OrderedDitheringFilterSettings *>(settings);
    if (w_settings) {
        this->settings = *w_settings;
    }
}

std::vector<float> OrderedDitheringFilter::buildMatrix(int size) {
    int generated_size = 2;
    std::vector<int> matrix({
        0, 2,
        3, 1
    });
    while (generated_size < size) {
        std::vector<int> matrix2n(generated_size * 2 * generated_size * 2);
        for (int j = 0; j < generated_size; ++j) {
            for (int i = 0; i < generated_size; ++i) {
                matrix2n[j * generated_size * 2 + i] = 4 * matrix[j * generated_size + i];
            }
        }
        for (int j = 0; j < generated_size; ++j) {
            for (int i = 0; i < generated_size; ++i) {
                matrix2n[j * generated_size * 2 + generated_size + i] = 4 * matrix[j * generated_size + i] + 2;
            }
        }
        for (int j = 0; j < generated_size; ++j) {
            for (int i = 0; i < generated_size; ++i) {
                matrix2n[(j + generated_size) * generated_size * 2 + i] = 4 * matrix[j * generated_size + i] + 3;
            }
        }
        for (int j = 0; j < generated_size; ++j) {
            for (int i = 0; i < generated_size; ++i) {
                matrix2n[(j + generated_size) * generated_size * 2 + generated_size + i] = 4 * matrix[j * generated_size + i] + 1;
            }
        }
        matrix = matrix2n;
        generated_size <<= 1;
    }
    std::vector<float> f_matrix(size * size);
    for (int i = 0; i < size * size; ++i) {
        f_matrix[i] = (float) matrix[i] / (float) (size * size);
    }
    return f_matrix;
}

namespace {
    bool reg = FilterRegistry::getInstance().registerFilter(new OrderedDitheringFilter);
}
