#include <QImage>
#include <QPainter>
#include <QDebug>

#include "filter_zone.h"
#include "utils.h"

using FilterUtils::emptyImage;
using FilterUtils::drawDashedRect;
using FilterUtils::RgbaDepth;

FilterZone::FilterZone(QWidget *parent) : QWidget(parent),
    image(0, 0, QImage::Format_RGBA8888),
    canvas(352, 352, QImage::Format_RGBA8888),
    has_image(false)
{
    this->setFixedSize(352, 352);
    this->drawBorder();
}

void FilterZone::clear() {
    emptyImage(this->canvas);
    this->has_image = false;
    this->drawBorder();
    this->update();
}

void FilterZone::setImage(QImage image) {
    this->image = image;
    this->clear();
    this->has_image = true;
    for (int i = 0; i < image.height(); ++i) {
        memcpy(this->canvas.bits() + (i + 1) * this->canvas.bytesPerLine() + RgbaDepth, image.bits() + i * image.bytesPerLine(), image.width() * RgbaDepth);
    }
    this->update();
}

QImage &FilterZone::getImage() {
    return this->image;
}

void FilterZone::drawBorder() {
    drawDashedRect(this->canvas, 0, 0, this->canvas.width(), this->canvas.height());
}

void FilterZone::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    painter.drawImage(0, 0, this->canvas);
}

SourceZone::SourceZone(QWidget *parent) : FilterZone(parent)
{
}

void SourceZone::clear() {
    FilterZone::clear();
}

void SourceZone::setImage(QImage image) {
    this->image = image;
    this->clear();
    this->has_image = true;
    QImage scaled = (image.width() > 350 || image.height() > 350) ?
                    FilterUtils::scaleImageToFit(image, 350, 350) :
                    image;
    for (int i = 0; i < scaled.height(); ++i) {
        memcpy(this->canvas.bits() + (i + 1) * this->canvas.bytesPerLine() + RgbaDepth, scaled.bits() + i * scaled.bytesPerLine(), scaled.width() * RgbaDepth);
    }
    this->saveCanvas();
    this->scaled_width = scaled.width();
    this->scaled_height = scaled.height();
    this->update();
}

void SourceZone::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    painter.drawImage(0, 0, this->canvas);
}

void SourceZone::mousePressEvent(QMouseEvent *event) {
    if (!this->has_image) {
        return;
    }
    int x = event->x() - 175 * this->scaled_width / this->image.width();
    int y = event->y() - 175 * this->scaled_height / this->image.height();
    int scaled_x = event->x() * this->image.width() / this->scaled_width - 175;
    int scaled_y = event->y() * this->image.height() / this->scaled_height - 175;
    int width = std::min(350, this->image.width());
    int height = std::min(350, this->image.height());
    if (scaled_x < 0) {
        x = 0;
        scaled_x = 0;
    }
    if (scaled_x + width > this->image.width()) {
        x = this->scaled_width - width * this->scaled_width / this->image.width();
        scaled_x = this->image.width() - width;
    }
    if (scaled_y < 0) {
        y = 0;
        scaled_y = 0;
    }
    if (scaled_y + height > this->image.height()) {
        y = this->scaled_height - height * this->scaled_height / this->image.height();
        scaled_y = this->image.height() - height;
    }
    QImage selection(width, height, this->image.format());
    emptyImage(selection);
    for (int i = 0; i < height; ++i) {
        memcpy(selection.bits() + i * selection.bytesPerLine(),
               this->image.bits() + (scaled_y + i) * this->image.bytesPerLine() + scaled_x * this->image.depth() / 8,
               width * this->image.depth() / 8);
    }
    drawSelectionBox(x, y, width * this->scaled_width / this->image.width(), height * this->scaled_height / this->image.height());
    emit zoneSelected(selection);
}

void SourceZone::drawSelectionBox(int x, int y, int width, int height) {
    this->restoreCanvas();
    drawDashedRect(this->canvas, x + 1, y + 1, width, height);
    this->update();
}

void SourceZone::mouseMoveEvent(QMouseEvent *event) {
    this->mousePressEvent(event);
}

void SourceZone::saveCanvas() {
    this->canvas_without_selection = this->canvas;
}

void SourceZone::restoreCanvas() {
    this->canvas = this->canvas_without_selection;
}

