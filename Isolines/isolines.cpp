#include "isolines.h"

#include <QPainter>
#include <QDebug>

Isolines::Isolines(QSharedPointer<Configuration> config, QWidget *parent) : QWidget(parent),
    config(config),
    image(this->size(), QImage::Format_RGB32),
    min(std::numeric_limits<double>::max())
{
    plot();
    connect(this->config.data(), &Configuration::interpolateChanged, this, [this] (bool) {
        this->plot();
        this->update();
    });
}

void Isolines::plot() {
    QRgb *pixels = (QRgb *) this->image.bits();
    auto levels = this->config->levels();
    // find min and max of function, count level step
    if (this->min - std::numeric_limits<double>::max() < 1e-20) {
        for (int j = 0; j < this->image.height(); ++j) {
            for (int i = 0; i < this->image.width(); ++i) {
                double val = Isolines::f(((double) i) / mul, ((double) j) / mul);
                if (val < this->min) {
                    this->min = val;
                }
                if (val > this->max) {
                    this->max = val;
                }
            }
        }
        this->step = (this->max - this->min) / (double) this->config->levels().size();
    }
    for (int j = 0; j < this->image.height(); ++j) {
        for (int i = 0; i < this->image.width(); ++i) {
            double val = Isolines::f(((double) i) / mul, ((double) j) / mul);
            int level_index = ((val - this->min) / this->step);
            QColor color = QColor(levels[level_index]);
            if (this->config->interpolate()) {
                double rel = val - min - level_index * this->step;
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
            pixels[(this->image.height() - 1 - j) * this->image.width() + i] = color.rgb();
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

void Isolines::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    painter.drawImage(0, 0, this->image);
}
