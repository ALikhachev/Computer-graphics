#include "configuration.h"

#include <QColor>

Configuration::Configuration(QObject *parent) : QObject(parent),
    _width(100),
    _height(20),
    _levels({qRgb(255, 0, 0), qRgb(0, 255, 0), qRgb(0, 0, 255)}),
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
