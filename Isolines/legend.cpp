#include "legend.h"

#include <QPainter>

Legend::Legend(QSharedPointer<Configuration> config, QWidget *parent) : QWidget(parent),
    config(config),
    image(this->size(), QImage::Format_RGB32),
    step(0)
{
    this->setMinimumHeight(100);
    this->setMaximumHeight(100);
}

void Legend::resizeEvent(QResizeEvent *event) {
    if (this->image.width() != event->size().width()) {
        this->image = QImage(event->size().width(), Legend::LegendHeight, QImage::Format_RGB32);
        this->step = (this->image.width() + 1) / this->config->levels().size();
        this->plot();
    }
}

void Legend::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    painter.drawImage(0, 0, this->image);
}

void Legend::plot() {
    auto levels = this->config->levels();
    QRgb *pixels = reinterpret_cast<QRgb *>(this->image.bits());
    int level_index;
    for (int j = 1; j < this->image.height() - 1; ++j) {
        for (int i = 1; i < this->image.width() - 1; ++i) {
            level_index = i / this->step;
            pixels[j * this->image.width() + i] = levels[level_index];
        }
    }
    this->drawBorder();
}

void Legend::drawBorder() {
    QRgb *pixels = reinterpret_cast<QRgb *>(this->image.bits());
    for (int i = 0; i < this->image.width(); ++i) {
        pixels[i] = qRgb(0, 0, 0);
    }
    for (int i = 0; i < this->image.height(); ++i) {
        pixels[i * this->image.width()] = qRgb(0, 0, 0);
    }
    for (int i = 0; i < this->image.width(); ++i) {
        pixels[(this->image.height() - 1) * this->image.width() + i] = qRgb(0, 0, 0);
    }
    for (int i = 0; i < this->image.height(); ++i) {
        pixels[(i + 1) * this->image.width() - 1] = qRgb(0, 0, 0);
    }
    int levels_count = this->config->levels().size() - 1;
    for (int i = this->step, k = 0; k < levels_count; i += this->step, ++k) {
        for (int j = 0; j < this->image.height(); ++j) {
            pixels[j * this->image.width() + i] = qRgb(0, 0, 0);
        }
    }
}
