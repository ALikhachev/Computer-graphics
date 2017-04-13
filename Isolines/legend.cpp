#include "legend.h"

#include <QPainter>
#include <QColor>
#include <QFont>

Legend::Legend(QSharedPointer<Configuration> config, QWidget *parent) : QWidget(parent),
    config(config),
    image(this->size(), QImage::Format_RGB32),
    step(0),
    has_current_value(false)
{
    this->setMinimumHeight(LegendHeight + VTopPadding + VBottomPadding);
    this->setMaximumHeight(LegendHeight + VTopPadding + VBottomPadding);
    connect(this->config.data(), &Configuration::interpolateChanged, this, [this] (bool) {
        this->plot();
        this->update();
    });
    connect(this->config.data(), &Configuration::levelsChanged, this, [this] (const std::vector<QRgb> &levels) {
        this->step = (double) this->image.width() / levels.size();
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
    double f_step = this->config->fStep();
    for (double i = this->config->fMin(), k = HPadding; k <= this->width(); i += f_step, k += this->step) {
        const QString str = QString::number(i, 'f', 2);
        int offset = str.length() * CharWidth / 2;
        painter.drawText(k - offset, 18, str);
    }
    if (this->has_current_value) {
        double range = this->config->fMax() - this->config->fMin();
        int x_offset = HPadding - 5 + std::round((this->current_value - this->config->fMin()) * this->image.width() / range);
        int y_offset = this->height() - VBottomPadding;
        QVector<QPoint> arrow{QPoint(x_offset, y_offset + 5), QPoint(x_offset + 10, y_offset + 5), QPoint(x_offset + 5, y_offset + 0)};
        painter.drawPolygon(arrow);
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

void Legend::pointerFunctionValueUpdated(IsolinesMousePosition position) {
    this->has_current_value = !position.out_of_screen;
    this->current_value = position.value;
    this->update();
}
