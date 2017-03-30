#include "ordered_dithering_filter.h"
#include "filter_registry.h"

OrderedDitheringFilter::OrderedDitheringFilter()
{

}

QImage OrderedDitheringFilter::applyFilter(QImage image, std::function<void(int)> updateProgress) {
    return image;
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
