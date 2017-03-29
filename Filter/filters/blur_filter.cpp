#include "blur_filter.h"
#include "filter_registry.h"

BlurFilter::BlurFilter() :
    matrices({{0,       1.0/6.0, 0,
               1.0/6.0, 2.0/6.0, 1.0/6.0,
               0,       1.0/6.0, 0},

              {1.0/9.0, 1.0/9.0, 1.0/9.0,
               1.0/9.0, 1.0/9.0, 1.0/9.0,
               1.0/9.0, 1.0/9.0, 1.0/9.0},

              {1.0/16.0, 2.0/16.0, 1.0/16.0,
               2.0/16.0, 4.0/16.0, 2.0/16.0,
               1.0/16.0, 2.0/16.0, 1.0/16.0},

              {1.0/9.0, 1.0/9.0, 1.0/9.0,
               1.0/9.0, 1.0/9.0, 1.0/9.0,
               1.0/9.0, 1.0/9.0, 1.0/9.0},

              {1.0/256.0, 4.0/256.0,  6.0/256.0,  4.0/256.0,  1.0/256.0,
               4.0/256.0, 16.0/256.0, 24.0/256.0, 16.0/256.0, 4.0/256.0,
               6.0/256.0, 24.0/256.0, 36.0/256.0, 24.0/256.0, 6.0/256.0,
               4.0/256.0, 16.0/256.0, 24.0/256.0, 16.0/256.0, 4.0/256.0,
               1.0/256.0, 4.0/256.0,  6.0/256.0,  4.0/256.0,  1.0/256.0,}})
{
}

QIcon BlurFilter::getIcon() {
    return QIcon(":/icons/blur.png");
}

QString BlurFilter::getName() {
    return tr("Blur");
}

std::vector<float> &BlurFilter::getMatrix() {
    return this->matrices[this->settings.matrix_index];
}

int BlurFilter::getMatrixSize() {
    if (this->settings.matrix_index == 5) {
        return 5;
    }
    return 3;
}

BlurFilterSettings *BlurFilter::getSettings() {
    return &this->settings;
}

namespace {
    bool reg = FilterRegistry::getInstance().registerFilter(new BlurFilter);
}
