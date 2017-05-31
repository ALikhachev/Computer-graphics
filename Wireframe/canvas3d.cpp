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
    connect(this->_config.data(), &Configuration::updated, this, [this] {
        this->_perspective.reset(new PerspectiveTransform(this->_config->clippingNearDistance(),
                                              this->_config->clippingFarDistance(),
                                              this->width(),
                                              this->height()));
        this->plot();
        this->update();
    });
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

//void Canvas3D::wheelEvent(QWheelEvent *event)
//{
//    float k = event->delta() > 0 ? 0.95f : 1.05f;
//    this->_config->setClippingNearDistance(this->_config->clippingNearDistance() * k);
//    this->_config->setClippingFarDistance(this->_config->clippingFarDistance() * k);
//    this->_perspective.reset(new PerspectiveTransform(this->_config->clippingNearDistance(),
//                                          this->_config->clippingFarDistance(),
//                                          this->width(),
//                                          this->height()));
//    this->plot();
//    this->update();
//}

void Canvas3D::drawObject(WireObject *object, Transform *scale_transform)
{
    auto &segments = object->getSegments();
    QSharedPointer<Transform> shift = object->getShiftTransform();
    if (dynamic_cast<GeneratrixObject *>(object) != nullptr) {
        Axis axis1(object->getCenter(), AxisType::OX, 0.5);
        Axis axis2(object->getCenter(), AxisType::OY, 0.5);
        Axis axis3(object->getCenter(), AxisType::OZ, 0.5);
        this->drawObject(&axis1, scale_transform);
        this->drawObject(&axis2, scale_transform);
        this->drawObject(&axis3, scale_transform);
    }
    for (auto it = segments.begin(); it < segments.end(); ++it) {
        HomogeneousPoint3D from_point = it->from();
        HomogeneousPoint3D to_point = it->to();
        from_point.applyTransform(shift);
        to_point.applyTransform(shift);
        if (scale_transform != NULL) {
            from_point.applyTransform(scale_transform);
            to_point.applyTransform(scale_transform);
        }
        from_point.applyTransform(this->_rotation);
        to_point.applyTransform(this->_rotation);
        from_point.applyTransform(this->_camera);
        to_point.applyTransform(this->_camera);
        from_point.applyTransform(this->_perspective);
        to_point.applyTransform(this->_perspective);
        Line3D res_line(from_point, to_point);
        if (res_line.clip()) {
            Drawing::drawLine3D(this->_image, res_line.from3D() * 80, res_line.to3D() * 80, object->getColor());
        }
    }
}

void Canvas3D::drawBoundingBox()
{
    WireObject cube(std::vector<Line3D>{
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
                     }, QColor(0, 0, 0));
    this->drawObject(&cube, NULL);
}

float Canvas3D::findAbsMax()
{
    float max = 0;
    for (auto obj : this->_config->objects()) {
        auto &segments = obj->getSegments();
        QSharedPointer<Transform> shift = obj->getShiftTransform();
        for (auto it = segments.begin(); it < segments.end(); ++it) {
            HomogeneousPoint3D from_point = it->from();
            HomogeneousPoint3D to_point = it->to();
            from_point.applyTransform(shift);
            to_point.applyTransform(shift);
            QVector3D from_3D = from_point.to3D();
            QVector3D to_3D = to_point.to3D();
            std::vector<float> points {
                std::fabs(from_3D.x()),
                std::fabs(from_3D.y()),
                std::fabs(from_3D.z()),
                std::fabs(to_3D.x()),
                std::fabs(to_3D.y()),
                std::fabs(to_3D.z())
            };
            max = std::max(max, *std::max_element(points.begin(), points.end()));
        }
    }
    return max;
}

void Canvas3D::plot()
{
    memset(this->_image.bits(), 0xFF, this->_image.bytesPerLine() * this->_image.height());
    if (this->_config->objects().size() > 1) {
        Axis axis1(AxisType::OX, 0.5);
        Axis axis2(AxisType::OY, 0.5);
        Axis axis3(AxisType::OZ, 0.5);
        this->drawObject(&axis1, NULL);
        this->drawObject(&axis2, NULL);
        this->drawObject(&axis3, NULL);
    }
    this->drawBoundingBox();
    float max = this->findAbsMax();
    ScaleTransform scale_transform = ScaleTransform(max);
    for (auto obj : this->_config->objects()) {
        this->drawObject(obj.data(), &scale_transform);
    }
}
