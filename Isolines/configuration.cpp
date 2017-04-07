#include "configuration.h"

#include <QColor>

Configuration::Configuration(QObject *parent) : QObject(parent),
    _start_x(0),
    _start_y(0),
    _width(20),
    _height(20),
    _f_min(0),
    _f_max(0),
    _horizontal_cell_count(40),
    _vertical_cell_count(40),
    _levels({qRgb(255, 0, 0), qRgb(255, 83, 0), qRgb(255, 223, 0), qRgb(0, 255, 0), qRgb(0, 184, 217), qRgb(0, 0, 255), qRgb(128, 0, 255)}),
    _isolines_color(qRgb(0, 0, 0)),
    _interpolate(false),
    _show_grid(false),
    _show_isolines(false)
{

}

double Configuration::startX() const {
    return this->_start_x;
}
void Configuration::setStartX(double start_x) {
    this->_start_x = start_x;
    emit startXChanged(this->_start_x);
}

double Configuration::startY() const {
    return this->_start_y;
}
void Configuration::setStartY(double start_y) {
    this->_start_y = start_y;
    emit startYChanged(this->_start_y);
}

double Configuration::width() const {
    return this->_width;
}

void Configuration::setWidth(double width) {
    this->_width = width;
    emit widthChanged(this->_width);
}

double Configuration::height() const {
    return this->_height;
}
void Configuration::setHeight(double height) {
    this->_height = height;
    emit heightChanged(this->_height);
}

int Configuration::horizontalCellCount() const {
    return this->_horizontal_cell_count;
}

void Configuration::setHorizontalCellCount(int cell_width) {
    this->_horizontal_cell_count = cell_width;
    emit horizontalCellCountChanged(this->_horizontal_cell_count);
}

double Configuration::fMin() const {
    return this->_f_min;
}

void Configuration::setFMin(double min) {
    this->_f_min = min;
    emit fMinChanged(this->_f_min);
}

double Configuration::fMax() const {
    return this->_f_max;
}

void Configuration::setFMax(double max) {
    this->_f_max = max;
    emit fMaxChanged(this->_f_max);
}

double Configuration::fStep() const {
    return this->_f_step;
}

void Configuration::setFStep(double step) {
    this->_f_step = step;
    emit fStepChanged(this->_f_step);
}


int Configuration::verticalCellCount() const {
    return this->_vertical_cell_count;
}

void Configuration::setVerticalCellCount(int cell_height) {
    this->_vertical_cell_count = cell_height;
    emit verticalCellCountChanged(this->_vertical_cell_count);
}

std::vector<QRgb> Configuration::levels() const {
    return this->_levels;
}

void Configuration::setLevels(std::vector<QRgb> &levels) {
    this->_levels = levels;
    emit levelsChanged(this->_levels);
}

QRgb Configuration::isolinesColor() const {
    return this->_isolines_color;
}

void Configuration::setIsolinesColor(QRgb isolines_color) {
    this->_isolines_color = isolines_color;
    emit isolinesColorChanged(this->_isolines_color);
}

bool Configuration::interpolate() const {
    return this->_interpolate;
}

void Configuration::setInterpolate(bool interpolate) {
    this->_interpolate = interpolate;
    emit interpolateChanged(this->_interpolate);
}

bool Configuration::showGrid() const {
    return this->_show_grid;
}

void Configuration::setShowGrid(bool show_grid) {
    this->_show_grid = show_grid;
    emit showGridChanged(this->_show_grid);
}

bool Configuration::showIsolines() const {
    return this->_show_isolines;
}

void Configuration::setShowIsolines(bool show_isolines) {
    this->_show_isolines = show_isolines;
    emit showIsolinesChanged(this->_show_isolines);
}

bool Configuration::load(QTextStream &in) {
    int cell_width, cell_height, n;
    QRgb isolines_color;
    in >> cell_width;
    if (cell_width <= 0) {
        return false;
    }
    in >> cell_height;
    if (cell_height <= 0) {
        return false;
    }
    in.readLine();
    in >> n;
    if (n <= 0) {
        return false;
    }
    in.readLine();
    std::vector<QRgb> levels(n + 1);
    for (int i = 0; i < n + 1; ++i) {
        int r, g, b;
        in >> r >> g >> b;
        levels[i] = qRgb(r, g, b);
        in.readLine();
    }
    int r, g, b;
    in >> r >> g >> b;
    isolines_color = qRgb(r, g, b);
    this->setHorizontalCellCount(cell_width);
    this->setVerticalCellCount(cell_height);
    this->setLevels(levels);
    this->setIsolinesColor(isolines_color);
    this->update();
    return true;
}

void Configuration::update() {
    emit configurationUpdated();
}
