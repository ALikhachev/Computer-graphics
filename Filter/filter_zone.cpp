#include <QImage>
#include <QPainter>
#include <QDebug>

#include "filter_zone.h"
#include "utils.h"

using FilterUtils::emptyImage;

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

QImage FilterZone::getImage() {
    return this->image;
}

void FilterZone::drawBorder() {
    int periods = this->canvas.width() / BorderDashLength;
    for (int i = 0; i < periods; i += 2) {
        // top border
        memset(this->canvas.bits() + i * BorderDashLength * RgbaDepth, 0, BorderDashLength * RgbaDepth);
        // bottom border
        memset(this->canvas.bits() + (this->canvas.height() - 1) * this->canvas.bytesPerLine() + i * BorderDashLength * RgbaDepth, 0, BorderDashLength * RgbaDepth);
        for (int j = i * BorderDashLength; j < (i + 1) * BorderDashLength; ++j) {
            this->canvas.bits()[j * RgbaDepth + 3] = 0xFF; // top
            this->canvas.bits()[(this->canvas.height() - 1) * this->canvas.bytesPerLine() + j * RgbaDepth + 3] = 0xFF; // bottom
            // right border
            memset(this->canvas.bits() + j * this->canvas.bytesPerLine() + (this->canvas.width() - 1) * RgbaDepth, 0, RgbaDepth - 1);
            this->canvas.bits()[j * this->canvas.bytesPerLine() + (this->canvas.width() - 1) * RgbaDepth + 3] = 0xFF;
            // left border
            memset(this->canvas.bits() + j * this->canvas.bytesPerLine(), 0, RgbaDepth - 1);
            this->canvas.bits()[j * this->canvas.bytesPerLine() + 3] = 0xFF;
        }
    }
}

void FilterZone::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    painter.drawImage(0, 0, this->canvas);
}

SourceZone::SourceZone(QWidget *parent) : FilterZone(parent),
    selection({0, 0, 0, 0, true})
{
    this->setFixedSize(352, 352);
}

void SourceZone::clear() {
    this->selection.empty = true;
    FilterZone::clear();
}

void SourceZone::setImage(QImage image) {
    this->image = image;
    this->clear();
    this->has_image = true;
    // TODO: own scale function
    QImage scaled = (image.width() > 350 || image.height() > 350) ?
                    image.scaled(350, 350, Qt::KeepAspectRatio) :
                    image;
    for (int i = 0; i < scaled.height(); ++i) {
        memcpy(this->canvas.bits() + (i + 1) * this->canvas.bytesPerLine() + RgbaDepth, scaled.bits() + i * scaled.bytesPerLine(), scaled.width() * RgbaDepth);
    }
    this->scaled_width = scaled.width();
    this->scaled_height = scaled.height();
    this->update();
}

void SourceZone::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    painter.drawImage(0, 0, this->canvas);
    if (this->selection.empty) {
        return;
    }
    painter.drawRect(this->selection.x + 1, this->selection.y + 1, this->selection.width, this->selection.height); // selection
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
    this->selection = {
        .x = x,
        .y = y,
        .width = width * this->scaled_width / this->image.width(),
        .height = height * this->scaled_height / this->image.height(),
        .empty = false
    };
    QImage selection(350, 350, this->image.format());
    emptyImage(selection);
    for (int i = 0; i < height; ++i) {
        memcpy(selection.bits() + i * selection.bytesPerLine(),
               this->image.bits() + (scaled_y + i) * this->image.bytesPerLine() + scaled_x * this->image.depth() / 8,
               width * this->image.depth() / 8);
    }
    this->update();
    emit zoneSelected(selection);
}

void SourceZone::mouseMoveEvent(QMouseEvent *event) {
    this->mousePressEvent(event);
}

