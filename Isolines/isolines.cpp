#include "isolines.h"

#include <QPainter>

Isolines::Isolines(QSharedPointer<Configuration> config, QWidget *parent) : QWidget(parent),
    config(config),
    image(this->size(), QImage::Format_RGB32)
{
    this->min = -2.0;
    this->max = 2.0;
    this->step = (this->max - this->min) / (double) this->config->levels().size();
    plot();
}

void Isolines::plot() {
    QRgb *pixels = (QRgb *) this->image.bits();
    auto levels = this->config->levels();
    for (int j = 0; j < this->image.height(); ++j) {
        for (int i = 0; i < this->image.width(); ++i) {
            double val = Isolines::f(((double) i) / mul, ((double) j) / mul);
            int color_index = ((val - this->min) / this->step);
            pixels[(this->image.height() - 1 - j) * this->image.width() + i] = levels[color_index];
        }
    }
}

void Isolines::resizeEvent(QResizeEvent *event) {
    double x_mul =  (double) event->size().width() / (double) this->config->width();
    double y_mul = (double) event->size().height() / (double) this->config->height();
    double mul = std::min(x_mul, y_mul);
    if (mul != this->mul) {
        this->mul = mul;
        this->image = QImage(this->config->width() * this->mul, this->config->height() * this->mul, QImage::Format_RGB32);
        plot();
    }
}

void Isolines::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.drawImage(0, 0, this->image);
}
