#include <QImage>
#include <QPainter>
#include <QDebug>

#include "filter_zone.h"

FilterZone::FilterZone(QWidget *parent) : QWidget(parent)
{
    this->setFixedSize(350, 350);
}

void FilterZone::setImage(QImage &image) {
    this->image = (image.width() > this->width() || image.height() > this->height()) ?
                      image.scaled(this->size(), Qt::KeepAspectRatio) :
                      image;
    qDebug() << this->image.size() << "\n";
}

void FilterZone::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    painter.drawImage(0, 0, this->image);
}
