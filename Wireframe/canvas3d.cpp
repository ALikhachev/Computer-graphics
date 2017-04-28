#include "canvas3d.h"

#include <QPainter>
#include "drawing.h"
#include "axis.h"

Canvas3D::Canvas3D(QSharedPointer<Configuration> config, QWidget *parent) : QWidget(parent),
    _config(config),
    _image(this->size(), QImage::Format_RGB32)
{

}

void Canvas3D::resizeEvent(QResizeEvent *)
{
    this->_image = QImage(this->size(), QImage::Format_RGB32);
}

void Canvas3D::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    this->_image.fill(QColor(255, 255, 255));
    Axis axis = Axis(AxisType::OX, 30);
    Drawing::drawObject(this->_image, axis, QColor(255, 0, 0));
    painter.drawImage(0, 0, this->_image);
}
