#include "negative_filter.h"
#include "filter_registry.h"

NegativeFilter::NegativeFilter() {

}

QImage NegativeFilter::applyFilter(QImage image, std::function<void(int)> updateProgress) {
    QImage filtered_image(image.size(), QImage::Format_RGBA8888);
    for (int j = 0; j < filtered_image.height(); ++j) {
        for (int i = 0; i < filtered_image.width(); ++i) {
            int index = j * image.bytesPerLine() + i * image.depth() / 8;
            filtered_image.bits()[index] = 0xFF - image.bits()[index];
            filtered_image.bits()[index + 1] = 0xFF - image.bits()[index + 1];
            filtered_image.bits()[index + 2] = 0xFF - image.bits()[index + 2];
            filtered_image.bits()[index + 3] = image.bits()[index + 3];
        }
    }
    return filtered_image;
}

QIcon NegativeFilter::getIcon() {
    return QIcon(":/icons/negative.png");
}

QString NegativeFilter::getName() {
    return tr("Negative");
}

namespace {
    bool reg = FilterRegistry::getInstance().registerFilter(new NegativeFilter);
}
