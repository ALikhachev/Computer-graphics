#include "filter.h"

void FilterWorker::doFilter(Filter *f, QImage image) {
    emit resultReady(f->applyFilter(image));
}

void Filter::request() {
    emit requested(this);
}

GrayscaleFilter::GrayscaleFilter() {

}

QImage GrayscaleFilter::applyFilter(QImage image) {
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
    }
    return filtered_image;
}

QIcon GrayscaleFilter::getIcon() {
    return QIcon(":/icons/grayscale.png");
}

QString GrayscaleFilter::getName() {
    return tr("Grayscale filter");
}

NegativeFilter::NegativeFilter() {

}
#include <QDebug>
QImage NegativeFilter::applyFilter(QImage image) {
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
    qDebug() << image.size();
    return filtered_image;
}

QIcon NegativeFilter::getIcon() {
    return QIcon(":/icons/negative.png");
}

QString NegativeFilter::getName() {
    return tr("Negative filter");
}
