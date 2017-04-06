#include "configuration.h"

#include <QColor>

Configuration::Configuration(QObject *parent) : QObject(parent),
    _width(100),
    _height(20),
    _levels({qRgb(255, 0, 0), qRgb(255, 83, 0), qRgb(255, 223, 0), qRgb(0, 255, 0), qRgb(0, 184, 217), qRgb(0, 0, 255), qRgb(128, 0, 255)}),
    _isolines_color(qRgb(255, 255, 255))
{

}

int Configuration::width() const {
    return this->_width;
}

void Configuration::setWidth(int width) {
    this->_width = width;
    emit widthChanged(this->_width);
}

int Configuration::height() const {
    return this->_height;
}
void Configuration::setHeight(int height) {
    this->_height = height;
    emit heightChanged(this->_height);
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

bool Configuration::load(QTextStream &in) {
    int width, height, n;
    QRgb isolines_color;
    in >> width;
    if (width <= 0) {
        return false;
    }
    in >> height;
    if (height <= 0) {
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
    this->setWidth(width);
    this->setHeight(height);
    this->setLevels(levels);
    this->setIsolinesColor(isolines_color);
    return true;
}
