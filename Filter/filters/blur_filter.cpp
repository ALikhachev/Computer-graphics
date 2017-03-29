#include "blur_filter.h"
#include "filter_registry.h"

BlurFilter::BlurFilter() {

}

QImage BlurFilter::applyFilter(QImage image, std::function<void(int)> updateProgress) {
    QImage filtered_image(image.size(), QImage::Format_RGBA8888);
    for (int j = 1; j < filtered_image.height() - 1; ++j) {
        for (int i = 1; i < filtered_image.width() - 1; ++i) {
            int index = j * image.bytesPerLine() + i * image.depth() / 8;
            int r = 0, g = 0, b = 0, a = 0;
            for (int k = -1; k < 2; k += 2) {
                r += image.bits()[(j + k) * image.bytesPerLine() + i * image.depth() / 8];
                g += image.bits()[(j + k) * image.bytesPerLine() + i * image.depth() / 8 + 1];
                b += image.bits()[(j + k) * image.bytesPerLine() + i * image.depth() / 8 + 2];
                a += image.bits()[(j + k) * image.bytesPerLine() + i * image.depth() / 8 + 3];
            }
            for (int k = -1; k < 2; k += 2) {
                r += image.bits()[j * image.bytesPerLine() + (i + k) * image.depth() / 8];
                g += image.bits()[j * image.bytesPerLine() + (i + k) * image.depth() / 8 + 1];
                b += image.bits()[j * image.bytesPerLine() + (i + k) * image.depth() / 8 + 2];
                a += image.bits()[j * image.bytesPerLine() + (i + k) * image.depth() / 8 + 3];
            }
            r += 2 * image.bits()[j * image.bytesPerLine() + i * image.depth() / 8];
            g += 2 * image.bits()[j * image.bytesPerLine() + i * image.depth() / 8 + 1];
            b += 2 * image.bits()[j * image.bytesPerLine() + i * image.depth() / 8 + 2];
            a += 2 * image.bits()[j * image.bytesPerLine() + i * image.depth() / 8 + 3];
            r /= 6;
            g /= 6;
            b /= 6;
            a /= 6;
            filtered_image.bits()[index] = (uchar) r;
            filtered_image.bits()[index + 1] = (uchar) g;
            filtered_image.bits()[index + 2] = (uchar) b;
            filtered_image.bits()[index + 3] = (uchar) a;
        }
    }
    return filtered_image;
}

QIcon BlurFilter::getIcon() {
    return QIcon(":/icons/blur.png");
}

QString BlurFilter::getName() {
    return tr("Blur");
}

namespace {
    bool reg = FilterRegistry::getInstance().registerFilter(new BlurFilter);
}
