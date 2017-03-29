#include "gamma_correction_filter.h"
#include "filter_registry.h"

GammaCorrectionFilter::GammaCorrectionFilter() {

}

QImage GammaCorrectionFilter::applyFilter(QImage image, std::function<void(int)> updateProgress) {
    QImage filtered_image(image.size(), QImage::Format_RGBA8888);
    float gamma = this->settings.gamma;
    for (int j = 0; j < filtered_image.height(); ++j) {
        for (int i = 0; i < filtered_image.width(); ++i) {
            int index = j * image.bytesPerLine() + i * image.depth() / 8;
            for (int k = 0; k < 3; ++k) {
                filtered_image.bits()[index + k] = 255.0 * std::pow(image.bits()[index + k], gamma) / std::pow(255, gamma);
            }
            filtered_image.bits()[index + 3] = image.bits()[index + 3];
        }
        updateProgress(100 * j / filtered_image.height());
    }
    return filtered_image;
}

QIcon GammaCorrectionFilter::getIcon() {
    return QIcon(":/icons/gamma.png");
}

QString GammaCorrectionFilter::getName() {
    return tr("Gamma");
}

GammaCorrectionFilterSettings *GammaCorrectionFilter::getSettings() {
    return &this->settings;
}

namespace {
    bool reg = FilterRegistry::getInstance().registerFilter(new GammaCorrectionFilter);
}
