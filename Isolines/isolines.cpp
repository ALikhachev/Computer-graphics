#include "isolines.h"

#include <QPainter>
#include <QDebug>
#include <QMouseEvent>

Isolines::Isolines(QSharedPointer<Configuration> config, QWidget *parent) : QWidget(parent),
    config(config),
    image(this->size(), QImage::Format_RGB32),
    min(std::numeric_limits<double>::max())
{
    this->setAttribute(Qt::WA_Hover);
    this->setMouseTracking(true);
    plot();
    connect(this->config.data(), &Configuration::interpolateChanged, this, [this] (bool) {
        this->plot();
        this->update();
    });
    connect(this->config.data(), &Configuration::levelsChanged, this, [this] (const std::vector<QRgb> &levels) {
        this->step = (this->max - this->min) / (double) levels.size();
        this->plot();
        this->update();
    });
    connect(this->config.data(), &Configuration::widthChanged, this, [this] (int width) {
        double x_mul =  (double) this->width() / (double) width;
        double y_mul = (double) this->height() / (double) this->config->height();
        this->mul = std::min(x_mul, y_mul);
        this->image = QImage(width * this->mul, this->config->height() * this->mul, QImage::Format_RGB32);
        this->min = std::numeric_limits<double>::max();
        this->plot();
        this->update();
    });
    connect(this->config.data(), &Configuration::heightChanged, this, [this] (int height) {
        double x_mul =  (double) this->width() / (double) this->config->width();
        double y_mul = (double) this->height() / (double) height;
        this->mul = std::min(x_mul, y_mul);
        this->image = QImage(this->config->width() * this->mul, height * this->mul, QImage::Format_RGB32);
        this->min = std::numeric_limits<double>::max();
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
        this->min = std::numeric_limits<double>::max();
        plot();
    }
}

bool Isolines::event(QEvent *event) {
    if (event->type() == QEvent::HoverLeave) {
        this->position.out_of_screen = true;
        emit pointerFunctionValueUpdated(this->position);
        return true;
    }
    return QWidget::event(event);
}

void Isolines::mouseMoveEvent(QMouseEvent *event) {
    bool out_of_screen = event->x() < 0 || event->y() < 0 || event->x() >= this->image.width() || event->y() >= this->image.height();
    if (!out_of_screen) {
        double x = (double) event->x() / mul;
        double y = (double) (this->image.height() - event->y()) / mul;
        double val = Isolines::f(x, y);
        this->position = {
            .x = x,
            .y = y,
            .value = val,
            .out_of_screen = false
        };
    }
    // do not emit new signals
    if (out_of_screen && this->position.out_of_screen) {
        return;
    }
    this->position.out_of_screen = out_of_screen;
    emit pointerFunctionValueUpdated(this->position);
}

void Isolines::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    painter.drawImage(0, 0, this->image);
}
