#include "isolines.h"

#include <QPainter>
#include <QDebug>
#include <QMouseEvent>

Isolines::Isolines(QSharedPointer<Configuration> config, QWidget *parent) : QWidget(parent),
    config(config),
    image(this->size(), QImage::Format_RGB32)
{
    this->setAttribute(Qt::WA_Hover);
    this->setMouseTracking(true);
    connect(this->config.data(), &Configuration::interpolateChanged, this, [this] (bool) {
        this->plot();
        this->update();
    });
    connect(this->config.data(), &Configuration::showGridChanged, this, [this] (bool) {
        this->plot();
        this->update();
    });
    connect(this->config.data(), &Configuration::levelsChanged, this, [this] (const std::vector<QRgb> &levels) {
        this->config->setFStep((this->config->fMax() - this->config->fMin()) / (double) levels.size());
        this->plot();
        this->update();
    });
}

void Isolines::plot() {
    QRgb *pixels = (QRgb *) this->image.bits();
    auto levels = this->config->levels();
    double min = this->config->fMin();
    double max = this->config->fMax();
    double x_offset = this->config->startX();
    double y_offset = this->config->startY();
    // find min and max of function, count level step
    if (min - std::numeric_limits<double>::max() < 1e-20) {
        for (int j = 0; j < this->image.height(); ++j) {
            for (int i = 0; i < this->image.width(); ++i) {
                double val = Isolines::f(((double) i) / this->scale_factor_x + x_offset, ((double) j) / this->scale_factor_y - y_offset);
                if (val < min) {
                    min = val;
                }
                if (val > max) {
                    max = val;
                }
            }
        }
        this->config->setFMin(min);
        this->config->setFMax(max);
        this->config->setFStep((max - min) / (double) levels.size());
    }
    double step = this->config->fStep();
    for (int j = 0; j < this->image.height(); ++j) {
        for (int i = 0; i < this->image.width(); ++i) {
            double val = Isolines::f((double) i / this->scale_factor_x + x_offset, (double) j / this->scale_factor_y - y_offset);
            int level_index = ((val - min) / step);
            QColor color = QColor(levels[level_index]);
            if (this->config->interpolate()) {
                double rel = val - min - level_index * step;
                int sign = (rel > step / 2) ? 1 : -1;
                int neighbor_index = level_index + sign;
                if (!(neighbor_index < 0 || static_cast<size_t>(neighbor_index) > levels.size() - 1)) {
                    QColor neighbor_color = QColor(QColor(levels[neighbor_index]));
                    double factor_neighbor;
                    double factor_this;
                    if (sign > 0) {
                        factor_neighbor = (double) (rel - step / 2) / step;
                        factor_this = 1.0 - factor_neighbor;
                    } else {
                        factor_neighbor = (double) (step / 2 - rel) / step;
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
    if (this->config->showGrid()) {
        this->drawGrid();
    }
}

void Isolines::drawGrid() {
    double cell_width = (double) this->image.width() / this->config->horizontalCellCount();
    double cell_height = (double) this->image.height() / this->config->horizontalCellCount();
    QRgb *pixels = (QRgb *) this->image.bits();
    for (int j = 0; j < this->image.height(); ++j) {
        for (double i = cell_width; i < this->image.width(); i += cell_width) {
            pixels[j * this->image.width() + (int) i] = qRgb(0, 0, 0);
        }
    }
    for (double j = cell_height; j < this->image.height(); j += cell_height) {
        for (int i = 0; i < this->image.width(); ++i) {
            pixels[(int) j * this->image.width() + i] = qRgb(0, 0, 0);
        }
    }
}

void Isolines::resizeEvent(QResizeEvent *event) {
    this->scale_factor_x =  (double) event->size().width() / (double) this->config->width();
    this->scale_factor_y = (double) event->size().height() / (double) this->config->height();
    this->image = QImage(this->config->width() * this->scale_factor_x, this->config->height() * this->scale_factor_y, QImage::Format_RGB32);
    this->config->setFMin(std::numeric_limits<double>::max());
    plot();
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
        double x = (double) event->x() / this->scale_factor_x + this->config->startX();
        double y = (double) (this->image.height() - 1 - event->y()) / this->scale_factor_y + this->config->startY();
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
