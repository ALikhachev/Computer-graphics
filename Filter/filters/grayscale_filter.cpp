#include "grayscale_filter.h"
#include "filter_registry.h"

GrayscaleFilter::GrayscaleFilter() {

}

QImage GrayscaleFilter::applyFilter(QImage image, std::function<void(int)> updateProgress) {
    QImage filtered_image(image.size(), QImage::Format_RGBA8888);
    for (int j = 0; j < filtered_image.height(); ++j) {
        for (int i = 0; i < filtered_image.width(); ++i) {
            int index = j * image.bytesPerLine() + i * image.depth() / 8;
            filtered_image.bits()[index] =
                    0.2126 * (double)image.bits()[index] + 0.7152 * (double)image.bits()[index + 1] + 0.0722 * (double)image.bits()[index + 2];
            filtered_image.bits()[index + 1] =
                    0.2126 * (double)image.bits()[index] + 0.7152 * (double)image.bits()[index + 1] + 0.0722 * (double)image.bits()[index + 2];
            filtered_image.bits()[index + 2] =
                    0.2126 * (double)image.bits()[index] + 0.7152 * (double)image.bits()[index + 1] + 0.0722 * (double)image.bits()[index + 2];
            filtered_image.bits()[index + 3] = image.bits()[index + 3];
        }
        updateProgress(100 * j / filtered_image.height());
    }
    return filtered_image;
}

QIcon GrayscaleFilter::getIcon() {
    return QIcon(":/icons/grayscale.png");
}

QString GrayscaleFilter::getName() {
    return tr("Grayscale");
}

namespace {
    bool reg = FilterRegistry::getInstance().registerFilter(new GrayscaleFilter);
}
