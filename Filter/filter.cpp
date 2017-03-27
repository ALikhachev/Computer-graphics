#include "filter.h"

FilterWorker::FilterWorker(Filter *filter, QImage image) :
  f(filter),
  image(image)
{
}

void FilterWorker::run() {
    emit imageReady(this->f->applyFilter(image));
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
    return filtered_image;
}

QIcon NegativeFilter::getIcon() {
    return QIcon(":/icons/negative.png");
}

QString NegativeFilter::getName() {
    return tr("Negative filter");
}

BlurFilter::BlurFilter() {

}

QImage BlurFilter::applyFilter(QImage image) {
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
    return tr("Blur filter");
}
