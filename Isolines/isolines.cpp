#include "isolines.h"

#include <QPainter>
#include <QDebug>
#include <QMouseEvent>

#include "utils.h"

Isolines::Isolines(QSharedPointer<Configuration> config, QWidget *parent) : QWidget(parent),
    config(config),
    image(this->size(), QImage::Format_RGB32),
    has_user_isoline(false),
    continious_isoline_mode(false),
    need_replot(true)
{
    this->setAttribute(Qt::WA_Hover);
    this->setMouseTracking(true);
    connect(this->config.data(), &Configuration::isolinesColorChanged, this, Isolines::repaint);
    connect(this->config.data(), &Configuration::verticalCellCountChanged, this, Isolines::repaint);
    connect(this->config.data(), &Configuration::horizontalCellCountChanged, this, Isolines::repaint);
    connect(this->config.data(), &Configuration::startXChanged, this, Isolines::replot);
    connect(this->config.data(), &Configuration::startYChanged, this, Isolines::replot);
    connect(this->config.data(), &Configuration::widthChanged, this, Isolines::resizeImage);
    connect(this->config.data(), &Configuration::heightChanged, this, Isolines::resizeImage);
    connect(this->config.data(), &Configuration::interpolateChanged, this, Isolines::replot);
    connect(this->config.data(), &Configuration::showGridChanged, this, Isolines::repaint);
    connect(this->config.data(), &Configuration::showIsolinesChanged, this, Isolines::repaint);
    connect(this->config.data(), &Configuration::showEntriesChanged, this, Isolines::repaint);
    connect(this->config.data(), &Configuration::levelsChanged, this, [this] (const std::vector<QRgb> &levels) {
        this->config->setFStep((this->config->fMax() - this->config->fMin()) / (double) levels.size());
        this->replot();
    });
}

void Isolines::paint() {
    if (this->need_replot) {
        this->plot();
        this->need_replot = false;
    }
    this->image = this->image_plot;
    if (this->config->showGrid()) {
        this->drawGrid();
    }
    if (this->config->showIsolines()) {
        this->drawIsolines();
    }
}

void Isolines::plot() {
    QRgb *pixels = (QRgb *) this->image_plot.bits();
    auto levels = this->config->levels();
    double min = this->config->fMin();
    double max = this->config->fMax();
    double x_offset = this->config->startX();
    double y_offset = this->config->startY();
    double height = this->config->height();
    // find min and max of function, count level step
    if (min - std::numeric_limits<double>::max() < 1e-15) {
        for (int j = 0; j < this->image.height(); ++j) {
            for (int i = 0; i < this->image.width(); ++i) {
                double x = ((double) i) / this->scale_factor_x + x_offset;
                double y = ((double) j / this->scale_factor_y) + y_offset;
                double val = Isolines::f(x, y);
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
    for (int j = 0; j < this->image_plot.height(); ++j) {
        for (int i = 0; i < this->image_plot.width(); ++i) {
            double x = ((double) i) / this->scale_factor_x + x_offset;
            double y = ((double) j / this->scale_factor_y) + y_offset;
            double val = Isolines::f(x, y);
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
}

void Isolines::drawGrid() {
    double cell_width = (double) this->config->width() / this->config->horizontalCellCount();
    double cell_height = (double) this->config->height() / this->config->verticalCellCount();
    double scaled_cell_width = cell_width * this->scale_factor_x;
    double scaled_cell_height = cell_height * this->scale_factor_y;
    QRgb *pixels = (QRgb *) this->image.bits();
    for (int j = 0; j < this->image.height(); ++j) {
        for (double i = scaled_cell_width; i < this->image.width(); i += scaled_cell_width) {
            pixels[j * this->image.width() + (int) std::round(i)] = qRgb(0, 0, 0);
        }
    }
    for (double j = 0; j < this->image.height(); j += scaled_cell_height) {
        for (int i = 0; i < this->image.width(); ++i) {
            pixels[(this->image.height() - 1 - (int) std::round(j)) * this->image.width() + i] = qRgb(0, 0, 0);
        }
    }
}

void Isolines::drawIsolines() {
    QRgb color = this->config->isolinesColor();
    int horizontal_cells = this->config->horizontalCellCount();
    int vertical_cells = this->config->verticalCellCount();
    double cell_width = (double) this->config->width() / horizontal_cells;
    double cell_height = (double) this->config->height() / vertical_cells;
    double scaled_cell_width = cell_width * this->scale_factor_x;
    double scaled_cell_height = cell_height * this->scale_factor_y;
    int default_isolines = this->config->levels().size() - 1;
    double min = this->config->fMin();
    double step = this->config->fStep();
    bool show_entries = this->config->showEntries();
    for (int j = 0; j < vertical_cells; ++j) {
        for (int i = 0; i < horizontal_cells; ++i) {
            std::vector<std::pair<QPoint, double>> cell{
                        std::make_pair(QPoint((int) std::round(i * scaled_cell_width),       this->image.height() - 1 - (int) std::round(j * scaled_cell_height)),
                                       f( i * cell_width + this->config->startX(),      (j * cell_height) + this->config->startY())),

                        std::make_pair(QPoint((int) std::round((i + 1) * scaled_cell_width), this->image.height() - 1 - (int) std::round(j * scaled_cell_height)),
                                       f((i + 1) * cell_width + this->config->startX(), (j * cell_height) + this->config->startY())),

                        std::make_pair(QPoint((int) std::round(i * scaled_cell_width),       this->image.height() - 1 - (int) std::round((j + 1) * scaled_cell_height)),
                                       f( i * cell_width + this->config->startX(),      ((j + 1) * cell_height) + this->config->startY())),

                        std::make_pair(QPoint((int) std::round((i + 1) * scaled_cell_width), this->image.height() - 1 - (int) std::round((j + 1) * scaled_cell_height)),
                                       f((i + 1) * cell_width + this->config->startX(), ((j + 1) * cell_height) + this->config->startY()))
            };
            double middle_value = f(i * cell_width / 2 + this->config->startX(), (j * cell_height / 2) + this->config->startY());
            for (int k = 0; k < default_isolines; ++k) {
                double isoline_level = min + (k + 1) * step;
                std::vector<std::pair<QPoint, QPoint>> isolines = IsolinesUtils::handleCell(cell, isoline_level, middle_value);
                for (auto it = isolines.begin(); it < isolines.end(); ++it) {
                    if (show_entries) {
                        IsolinesUtils::drawCircle(this->image, it->first, CircleRadius, qRgb(0, 0, 0));
                        IsolinesUtils::drawCircle(this->image, it->second, CircleRadius, qRgb(0, 0, 0));
                    }
                    IsolinesUtils::drawLine(this->image, it->first, it->second, color);
                }
            }
            if (this->has_user_isoline) {
                std::vector<std::pair<QPoint, QPoint>> isolines = IsolinesUtils::handleCell(cell, this->user_isoline, middle_value);
                for (auto it = isolines.begin(); it < isolines.end(); ++it) {
                    if (show_entries) {
                        IsolinesUtils::drawCircle(this->image, it->first, CircleRadius, qRgb(0, 0, 0));
                        IsolinesUtils::drawCircle(this->image, it->second, CircleRadius, qRgb(0, 0, 0));
                    }
                    IsolinesUtils::drawLine(this->image, it->first, it->second, color);
                }
            }
        }
    }
}

void Isolines::resizeEvent(QResizeEvent *) {
    this->resizeImage();
}

bool Isolines::event(QEvent *event) {
    if (event->type() == QEvent::HoverLeave) {
        this->position.out_of_screen = true;
        emit pointerFunctionValueUpdated(this->position);
        return true;
    }
    return QWidget::event(event);
}
#include <QDebug>
void Isolines::mouseMoveEvent(QMouseEvent *event) {
    double x = (double) event->x() / this->scale_factor_x + this->config->startX();
    double y = (double) (this->image.height() - 1 - event->y()) / this->scale_factor_y + this->config->startY();
    double val = Isolines::f(x, y);
    this->position = {
        .x = x,
        .y = y,
        .value = val,
        .out_of_screen = false
    };
    emit pointerFunctionValueUpdated(this->position);
    if (this->continious_isoline_mode && this->config->showIsolines()) {
        this->user_isoline = val;
        this->repaint();
    }
}

void Isolines::mousePressEvent(QMouseEvent *event) {
    if (!this->config->showIsolines()) {
        return;
    }
    if (event->button() == Qt::LeftButton) {
        this->has_user_isoline = true;
        double x = (double) event->x() / this->scale_factor_x + this->config->startX();
        double y = (double) (this->image.height() - 1 - event->y()) / this->scale_factor_y + this->config->startY();
        this->user_isoline = Isolines::f(x, y);
        this->continious_isoline_mode = true;
    } else {
        this->has_user_isoline = false;
    }
    this->repaint();
}

void Isolines::mouseReleaseEvent(QMouseEvent *event) {
    if (!this->config->showIsolines()) {
        return;
    }
    if (event->button() == Qt::LeftButton) {
        this->continious_isoline_mode = false;
    }
}

void Isolines::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    painter.drawImage(0, 0, this->image);
}

void Isolines::resizeImage() {
    double f_x = (double) this->width() / (double) this->config->width();
    double f_y = (double) this->height() / (double) this->config->height();
    if (std::fabs(this->scale_factor_x - f_x) > 1e-15 || std::fabs(this->scale_factor_y - f_y) > 1e-15) {
        this->scale_factor_x =  f_x;
        this->scale_factor_y = f_y;
        this->image = QImage(this->config->width() * this->scale_factor_x, this->config->height() * this->scale_factor_y, QImage::Format_RGB32);
        this->image_plot = this->image;
        this->config->setFMin(std::numeric_limits<double>::max());
        this->replot();
    }
}

void Isolines::repaint() {
    this->paint();
    this->update();
}

void Isolines::replot() {
    this->need_replot = true;
    this->repaint();
}
