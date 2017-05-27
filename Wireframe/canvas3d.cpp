#include "canvas3d.h"

#include <QPainter>
#include "drawing.h"
#include "axis.h"

Canvas3D::Canvas3D(QSharedPointer<Configuration> config, QWidget *parent) : QWidget(parent),
    _config(config),
    _image(this->size(), QImage::Format_RGB32),
    _rotation(new IdentityTransform),
    _camera(new CameraTransform())
{

}

void Canvas3D::resizeEvent(QResizeEvent *)
{
    this->_image = QImage(this->size(), QImage::Format_RGB32);
    this->_perspective.reset(new PerspectiveTransform(this->_config->clippingNearDistance(),
                                          this->_config->clippingFarDistance(),
                                          this->width(),
                                          this->height()));
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
    RotateZTransform x_transform = RotateZTransform(diff_y);
    this->_rotation = this->_rotation->compose((Transform *)&y_transform)->compose((Transform *)&x_transform);
    this->_rotation_tracking.setX(event->x());
    this->_rotation_tracking.setY(event->y());
    this->plot();
    this->update();
}

void Canvas3D::wheelEvent(QWheelEvent *event)
{
    float k = event->delta() > 0 ? 1.1 : 0.9;
    this->_config->setClippingFarDistance(this->_config->clippingFarDistance() * k);
    this->_perspective.reset(new PerspectiveTransform(this->_config->clippingNearDistance(),
                                          this->_config->clippingFarDistance(),
                                          this->width(),
                                          this->height()));
    this->plot();
    this->update();
}

void Canvas3D::drawObject(WireObject *object, QColor color)
{
    auto &segments = object->getSegments();
    for (auto it = segments.begin(); it < segments.end(); ++it) {
        HomogeneousPoint3D from_point = it->from();
        HomogeneousPoint3D to_point = it->to();
        from_point.applyTransform(this->_rotation);
        to_point.applyTransform(this->_rotation);
        from_point.applyTransform(this->_camera);
        to_point.applyTransform(this->_camera);
        from_point.applyTransform(this->_perspective);
        to_point.applyTransform(this->_perspective);
        Line3D res_line(from_point, to_point);
        if (res_line.clip()) {
            Drawing::drawLine3D(this->_image, res_line.from3D() * 100, res_line.to3D()* 100, color);
        }
    }
}

void Canvas3D::drawBoundingBox()
{
    WireObject cube = WireObject({
                                     Line3D(HomogeneousPoint3D(-1, -1, -1), HomogeneousPoint3D(1, -1, -1)),
                                     Line3D(HomogeneousPoint3D(-1, -1, -1), HomogeneousPoint3D(-1, 1, -1)),
                                     Line3D(HomogeneousPoint3D(-1, 1, -1), HomogeneousPoint3D(1, 1, -1)),
                                     Line3D(HomogeneousPoint3D(1, -1, -1), HomogeneousPoint3D(1, 1, -1)),
                                     Line3D(HomogeneousPoint3D(-1, -1, 1), HomogeneousPoint3D(1, -1, 1)),
                                     Line3D(HomogeneousPoint3D(-1, -1, 1), HomogeneousPoint3D(-1, 1, 1)),
                                     Line3D(HomogeneousPoint3D(-1, 1, 1), HomogeneousPoint3D(1, 1, 1)),
                                     Line3D(HomogeneousPoint3D(1, -1, 1), HomogeneousPoint3D(1, 1, 1)),
                                     Line3D(HomogeneousPoint3D(-1, -1, -1), HomogeneousPoint3D(-1, -1, 1)),
                                     Line3D(HomogeneousPoint3D(-1, 1, -1), HomogeneousPoint3D(-1, 1, 1)),
                                     Line3D(HomogeneousPoint3D(1, -1, -1), HomogeneousPoint3D(1, -1, 1)),
                                     Line3D(HomogeneousPoint3D(1, 1, -1), HomogeneousPoint3D(1, 1, 1)),
                                 });
    this->drawObject(&cube, QColor(0, 0, 0));
}

void Canvas3D::plot()
{
    memset(this->_image.bits(), 0xFF, this->_image.bytesPerLine() * this->_image.height());
    Axis axis1 = Axis(AxisType::OX, 0.5);
    Axis axis2 = Axis(AxisType::OY, 0.5);
    Axis axis3 = Axis(AxisType::OZ, 0.5);
    this->drawObject(&axis1, QColor(255, 0, 0));
    this->drawObject(&axis2, QColor(0, 255, 0));
    this->drawObject(&axis3, QColor(0, 0, 255));
    for (auto obj : this->_config->objects()) {
        this->drawObject(obj.data(), obj->color());
    }
    this->drawBoundingBox();
}
