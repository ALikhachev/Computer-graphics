#include "legend.h"

#include <QPainter>
#include <QColor>
#include <QFont>

Legend::Legend(QSharedPointer<Configuration> config, QWidget *parent) : QWidget(parent),
    config(config),
    image(this->size(), QImage::Format_RGB32),
    step(0),
    f_min(-2.0),
    f_max(2.0),
    f_step((this->f_max - this->f_min) / this->config->levels().size())
{
    this->setMinimumHeight(120);
    this->setMaximumHeight(120);
    connect(this->config.data(), &Configuration::interpolateChanged, this, [this] (bool) {
        this->plot();
        this->update();
    });
}

void Legend::resizeEvent(QResizeEvent *event) {
    int width = event->size().width() - HPadding * 2;
    if (this->image.width() != width) {
        this->image = QImage(width, Legend::LegendHeight, QImage::Format_RGB32);
        this->step = (double) this->image.width() / this->config->levels().size();
        this->plot();
    }
}

void Legend::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    painter.setFont(QFont(QString("Courier"), CharWidth, QFont::Bold));
    for (double i = this->f_min, k = HPadding; k <= this->width(); i += this->f_step, k += this->step) {
        const QString str = QString::number(i, 'f', 2);
        int offset = str.length() * CharWidth / 2;
        painter.drawText(k - offset, 18, str);
    }
    painter.drawImage(HPadding, VTopPadding, this->image);
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
                double rel = x - level_index * this->step;
                int sign = (rel > this->step / 2) ? 1 : -1;
                int neighbor_index = level_index + sign;
                if (!(neighbor_index < 0 || static_cast<size_t>(neighbor_index) > levels.size() - 1)) {
                    QColor neighbor_color = QColor(QColor(levels[neighbor_index]));
                    double factor_neighbor;
                    double factor_this;
                    if (sign > 0) {
                        factor_neighbor = (rel - this->step / 2) / this->step;
                        factor_this = 1.0 - factor_neighbor;
                    } else {
                        factor_neighbor = (this->step / 2 - rel) / this->step;
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
        for (double i = this->step, k = 0; k < levels_count; i += this->step, ++k) {
            for (int j = 0; j < this->image.height(); ++j) {
                pixels[j * this->image.width() + (int) std::ceil(i)] = qRgb(0, 0, 0);
            }
        }
    }
}
