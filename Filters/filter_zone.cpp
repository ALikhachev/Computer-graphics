#include <QImage>
#include <QPainter>
#include <QDebug>

#include "filter_zone.h"

FilterZone::FilterZone(QWidget *parent) : QWidget(parent),
    image(352, 352, QImage::Format_RGB32),
    depth_bytes(4)
{
    this->setFixedSize(352, 352);
    emptyImage(this->image);
    this->drawBorder();
}

void FilterZone::setImage(QImage &image) {
    this->image = (image.width() > 350 || image.height() > 350) ?
                      image.scaled(350, 350, Qt::KeepAspectRatio) :
                      image;
    this->depth_bytes = this->image.depth() / 8;
    this->offsetImage();
    this->drawBorder();
}

void FilterZone::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    painter.drawImage(0, 0, this->image);
}

void FilterZone::offsetImage() {
    QImage full_image(352, 352, this->image.format());
    int offset = this->depth_bytes;
    emptyImage(full_image);
    for (int j = 0; j < this->image.height(); ++j) {
        for (int i = 0; i < this->image.width() * this->depth_bytes; ++i) {
            full_image.bits()[(j + 1) * full_image.bytesPerLine() + i + offset] = this->image.bits()[j * this->image.bytesPerLine() + i];
        }
    }
    this->image = full_image;
}

void FilterZone::drawBorder() {
    int dash_length = this->image.width() / DashPeriods;
    for (int i = 0; i < DashPeriods; i += 2) {
        // top border
        memset(this->image.bits() + i * dash_length * this->depth_bytes, 0, dash_length * this->depth_bytes);
        // right border
        for (int j = i * dash_length; j < (i + 1) * dash_length; ++j) {
            memset(this->image.bits() + j * this->image.bytesPerLine() + (this->image.width() - 1) * this->depth_bytes, 0, this->depth_bytes);
        }
    }
    for (int i = 1; i < DashPeriods; i += 2) {
        // bottom border
        memset(this->image.bits() + (this->image.height() - 1) * this->image.bytesPerLine() + i * dash_length * this->depth_bytes, 0, dash_length * this->depth_bytes);
        // left border
        for (int j = i * dash_length; j < (i + 1) * dash_length; ++j) {
            memset(this->image.bits() + j * this->image.bytesPerLine(), 0, this->depth_bytes);
        }
    }
}

void FilterZone::emptyImage(QImage &image) {
    memset(image.bits(), 0xFF, image.height() * image.bytesPerLine());
}
