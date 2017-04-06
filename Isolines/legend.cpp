#include "legend.h"

#include <QPainter>
#include <QColor>

Legend::Legend(QSharedPointer<Configuration> config, QWidget *parent) : QWidget(parent),
    config(config),
    image(this->size(), QImage::Format_RGB32),
    step(0)
{
    this->setMinimumHeight(100);
    this->setMaximumHeight(100);
    connect(this->config.data(), &Configuration::interpolateChanged, this, [this] (bool) {
        this->plot();
        this->update();
    });
}

void Legend::resizeEvent(QResizeEvent *event) {
    if (this->image.width() != event->size().width()) {
        this->image = QImage(event->size().width(), Legend::LegendHeight, QImage::Format_RGB32);
        this->step = this->image.width() / this->config->levels().size();
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
            int x = i - 1;
            level_index = x / this->step;
            QColor color = QColor(levels[level_index]);
            if (this->config->interpolate()) {
                int rel = x - level_index * this->step;
                int sign = (rel > this->step / 2) ? 1 : -1;
                int neighbor_index = level_index + sign;
                if (!(neighbor_index < 0 || static_cast<size_t>(neighbor_index) > levels.size() - 1)) {
                    QColor neighbor_color = QColor(QColor(levels[neighbor_index]));
                    double factor_neighbor;
                    double factor_this;
                    if (sign > 0) {
                        factor_neighbor = (double) (rel - this->step / 2) / this->step;
                        factor_this = 1.0 - factor_neighbor;
                    } else {
                        factor_neighbor = (double) (this->step / 2 - rel) / this->step;
                        factor_this = 1.0 - factor_neighbor;
                    }
                    color.setRed(color.red() * factor_this + neighbor_color.red() * factor_neighbor);
                    color.setGreen(color.green() * factor_this + neighbor_color.green() * factor_neighbor);
                    color.setBlue(color.blue() * factor_this + neighbor_color.blue() * factor_neighbor);
                }
            }
            pixels[j * this->image.width() + i] = color.rgb();
        }
    }
    this->drawBorders();
}

void Legend::drawBorders() {
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
    if (!this->config->interpolate()) {
        int levels_count = this->config->levels().size() - 1;
        for (int i = this->step, k = 0; k < levels_count; i += this->step, ++k) {
            for (int j = 0; j < this->image.height(); ++j) {
                pixels[j * this->image.width() + i] = qRgb(0, 0, 0);
            }
        }
    }
}
