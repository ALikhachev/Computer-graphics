#include <QImage>
#include <QPainter>
#include <QDebug>

#include "filter_zone.h"

FilterZone::FilterZone(QWidget *parent) : QWidget(parent),
    image(352, 352, QImage::Format_RGB32),
    depth_bytes(4)
{
    this->setFixedSize(352, 352);
    this->clear();
}

void FilterZone::clear() {
    emptyImage(this->image);
    this->update();
}

void FilterZone::setImage(QImage image) {
    this->image = (image.width() > 350 || image.height() > 350) ?
                      image.scaled(350, 350, Qt::KeepAspectRatio) :
                      image;
    this->depth_bytes = this->image.depth() / 8;
    this->update();
}

QImage FilterZone::getImage() {
    return this->image;
}

void FilterZone::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    painter.drawImage(1, 1, this->image);
    QPen pen(Qt::black, 1, Qt::DashLine);
    painter.setPen(pen);
    painter.drawRect(0, 0, 351, 351);
}

void FilterZone::emptyImage(QImage &image) {
    memset(image.bits(), 0xFF, image.height() * image.bytesPerLine());
}
