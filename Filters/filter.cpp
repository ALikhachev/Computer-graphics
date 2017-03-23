#include "filter.h"

void Filter::request() {
    emit requested();
}

GrayscaleFilter::GrayscaleFilter() {

}

QImage GrayscaleFilter::applyFilter(QImage image) {
    QImage filtered_image(image.size(), QImage::Format_Grayscale8);
    for (int j = 0; j < filtered_image.height(); ++j) {
        for (int i = 0; i < filtered_image.width(); ++i) {
            int src_index = j * image.bytesPerLine() + i * image.depth() / 8;
            filtered_image.bits()[j * filtered_image.bytesPerLine() + i] =
                    0.2126 * (double)image.bits()[src_index] + 0.7152 * (double)image.bits()[src_index + 1] + 0.0722 * (double)image.bits()[src_index + 2];
        }
    }
    return filtered_image;
}

FilterParametersWidget *GrayscaleFilter::getParametersWidget(QWidget *parent) {
    return NULL;
}

QIcon GrayscaleFilter::getIcon() {
    return QIcon(":/icons/grayscale.png");
}

QString GrayscaleFilter::getName() {
    return tr("Grayscale filter");
}
