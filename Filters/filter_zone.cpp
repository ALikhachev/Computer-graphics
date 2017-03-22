#include <QImage>
#include <QPainter>
#include <QDebug>

#include "filter_zone.h"

FilterZone::FilterZone(QWidget *parent) : QWidget(parent)
{
    this->setFixedSize(352, 352);
}

void FilterZone::setImage(QImage &image) {
    this->image = (image.width() > 350 || image.height() > 350) ?
                      image.scaled(350, 350, Qt::KeepAspectRatio) :
                      image;
    qDebug() << this->image.size() << "\n";
}

void FilterZone::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    painter.drawImage(0, 0, this->image);
}

void FilterZone::offsetImage() {
    QImage full_image;
    int depth_bytes = this->image.depth() / 8;
    int offset = depth_bytes;
    for (int j = 0; j < 350 * depth_bytes; ++j) {
        for (int i = 0; i < 350 * depth_bytes; ++i) {
            full_image.bits()[j * 350 * depth_bytes + i + offset] = this->image.bits()[j * 350 * depth_bytes + i];
        }
    }
    this->image = full_image;
}
