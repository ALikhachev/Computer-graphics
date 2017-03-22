#include <QImage>
#include <QPainter>

#include "filter_zone.h"

FilterZone::FilterZone(QWidget *parent) : QWidget(parent)
{
    this->setFixedSize(350, 350);
}

void FilterZone::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    QImage image(this->size(), QImage::Format_RGB32);
    image.fill(qRgb(120, 0, 0));
    painter.drawImage(0, 0, image);
}
