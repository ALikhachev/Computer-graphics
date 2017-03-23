#include <QImage>
#include <QPainter>
#include <QDebug>

#include "filter_zone.h"
#include "utils.h"

using FilterUtils::emptyImage;

FilterZone::FilterZone(QWidget *parent) : QWidget(parent),
    image(350, 350, QImage::Format_RGB32)
{
    this->setFixedSize(352, 352);
    this->clear();
}

void FilterZone::clear() {
    emptyImage(this->image);
    this->update();
}

void FilterZone::setImage(QImage image) {
    this->image = image;
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

SourceZone::SourceZone(QWidget *parent) : QWidget(parent),
    selection({0, 0, 0, 0, true}),
    source_image(350, 350, QImage::Format_RGB32),
    image(350, 350, QImage::Format_RGB32)
{
    this->setFixedSize(352, 352);
    this->clear();
}

void SourceZone::clear() {
    emptyImage(this->image);
    emptyImage(this->source_image);
    this->selection.empty = true;
    this->update();
}

void SourceZone::setSourceImage(QImage image) {
    this->selection.empty = true;
    this->source_image = image;
    this->image = (image.width() > 350 || image.height() > 350) ?
                      image.scaled(350, 350, Qt::KeepAspectRatio) :
                      image;
    this->update();
}

void SourceZone::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    painter.drawImage(1, 1, this->image);
    QPen pen(Qt::black, 1, Qt::DashLine);
    painter.setPen(pen);
    painter.drawRect(0, 0, 351, 351); // border

    if (this->selection.empty) {
        return;
    }
    QVector<qreal> pattern = {5, 5};
    pen.setDashPattern(pattern);
    painter.setPen(pen);
    painter.drawRect(this->selection.x + 1, this->selection.y + 1, this->selection.width, this->selection.height); // selection
}

void SourceZone::mousePressEvent(QMouseEvent *event) {
    int x = event->x() - 175 * this->image.width() / this->source_image.width();
    int y = event->y() - 175 * this->image.height() / this->source_image.height();
    int scaled_x = event->x() * this->source_image.width() / this->image.width() - 175;
    int scaled_y = event->y() * this->source_image.height() / this->image.height() - 175;
    int width = std::min(350, this->source_image.width());
    int height = std::min(350, this->source_image.height());
    if (scaled_x < 0) {
        x = 0;
        scaled_x = 0;
    }
    if (scaled_x + width > this->source_image.width()) {
        x = this->image.width() - width * this->image.width() / this->source_image.width();
        scaled_x = this->source_image.width() - width;
    }
    if (scaled_y < 0) {
        y = 0;
        scaled_y = 0;
    }
    if (scaled_y + height > this->source_image.height()) {
        y = this->image.height() - height * this->image.height() / this->source_image.height();
        scaled_y = this->source_image.height() - height;
    }
    this->selection = {
        .x = x,
        .y = y,
        .width = width * this->image.width() / this->source_image.width(),
        .height = height * this->image.height() / this->source_image.height(),
        .empty = false
    };
    QImage selection(350, 350, this->source_image.format());
    emptyImage(selection);
    for (int i = 0; i < height; ++i) {
        memcpy(selection.bits() + i * selection.bytesPerLine(),
               this->source_image.bits() + (scaled_y + i) * this->source_image.bytesPerLine() + scaled_x * this->source_image.depth() / 8,
               width * this->source_image.depth() / 8);
    }
    this->update();
    emit zoneSelected(selection);
}

void SourceZone::mouseMoveEvent(QMouseEvent *event) {
    this->mousePressEvent(event);
}

