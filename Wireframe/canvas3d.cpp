#include "canvas3d.h"

#include <QPainter>
#include "drawing.h"
#include "axis.h"

Canvas3D::Canvas3D(QSharedPointer<Configuration> config, QWidget *parent) : QWidget(parent),
    _config(config),
    _image(this->size(), QImage::Format_RGB32),
    _rotation(new IdentityTransform)
{

}

void Canvas3D::resizeEvent(QResizeEvent *)
{
    this->_image = QImage(this->size(), QImage::Format_RGB32);
    this->plot();
}

void Canvas3D::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.drawImage(0, 0, this->_image);
}

void Canvas3D::mousePressEvent(QMouseEvent *event)
{
    this->_rotation_tracking.setX(event->x());
    this->_rotation_tracking.setY(event->y());
}

void Canvas3D::mouseMoveEvent(QMouseEvent *event)
{
    float diff_x = (float) (event->x() - this->_rotation_tracking.x()) * 2 * Pi / this->_image.width();
    float diff_y = (float) (event->y() - this->_rotation_tracking.y()) * 2 * Pi / this->_image.height();
    RotateYTransform y_transform = RotateYTransform(diff_x);
    RotateXTransform x_transform = RotateXTransform(diff_y);
    this->_rotation->compose((Transform *)&y_transform)->compose((Transform *)&x_transform);
    this->_rotation_tracking.setX(event->x());
    this->_rotation_tracking.setY(event->y());
    this->plot();
    this->update();
}

void Canvas3D::drawObject(WireObject &object, QColor color)
{
    auto &segments = object.getSegments();
    for (auto it = segments.begin(); it < segments.end(); ++it) {
        HomogeneousPoint3D from_point = it->from();
        HomogeneousPoint3D to_point = it->to();
        from_point.applyTransform(this->_rotation);
        to_point.applyTransform(this->_rotation);
        Drawing::drawLine3D(this->_image, from_point.to3D(), to_point.to3D(), color);
    }
}

void Canvas3D::plot()
{
    memset(this->_image.bits(), 0xFF, this->_image.bytesPerLine() * this->_image.height());
    Axis axis1 = Axis(AxisType::OX, 100);
    Axis axis2 = Axis(AxisType::OY, 100);
    Axis axis3 = Axis(AxisType::OZ, 100);
    WireObject cube = WireObject({
                                     Line3D(HomogeneousPoint3D(-75, -75, -75), HomogeneousPoint3D(75, -75, -75)),
                                     Line3D(HomogeneousPoint3D(-75, -75, -75), HomogeneousPoint3D(-75, 75, -75)),
                                     Line3D(HomogeneousPoint3D(-75, 75, -75), HomogeneousPoint3D(75, 75, -75)),
                                     Line3D(HomogeneousPoint3D(75, -75, -75), HomogeneousPoint3D(75, 75, -75)),
                                     Line3D(HomogeneousPoint3D(-75, -75, 75), HomogeneousPoint3D(75, -75, 75)),
                                     Line3D(HomogeneousPoint3D(-75, -75, 75), HomogeneousPoint3D(-75, 75, 75)),
                                     Line3D(HomogeneousPoint3D(-75, 75, 75), HomogeneousPoint3D(75, 75, 75)),
                                     Line3D(HomogeneousPoint3D(75, -75, 75), HomogeneousPoint3D(75, 75, 75)),
                                     Line3D(HomogeneousPoint3D(-75, -75, -75), HomogeneousPoint3D(-75, -75, 75)),
                                     Line3D(HomogeneousPoint3D(-75, 75, -75), HomogeneousPoint3D(-75, 75, 75)),
                                     Line3D(HomogeneousPoint3D(75, -75, -75), HomogeneousPoint3D(75, -75, 75)),
                                     Line3D(HomogeneousPoint3D(75, 75, -75), HomogeneousPoint3D(75, 75, 75)),
                                 });
    this->drawObject(cube, QColor(0, 0, 0));
    this->drawObject(axis1, QColor(255, 0, 0));
    this->drawObject(axis2, QColor(0, 255, 0));
    this->drawObject(axis3, QColor(0, 0, 255));
}
