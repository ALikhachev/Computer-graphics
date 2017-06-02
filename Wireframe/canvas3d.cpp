#include "canvas3d.h"

#include <QPainter>
#include "drawing.h"
#include "axis.h"

Canvas3D::Canvas3D(QSharedPointer<Configuration> config, QWidget *parent) : QWidget(parent),
    _config(config),
    _image(this->size(), QImage::Format_RGB32),
    _perspective(new PerspectiveTransform(this->_config->clippingNearDistance(),
                                          this->_config->clippingFarDistance(),
                                          this->_config->sw(),
                                          this->_config->sh())),
    _camera(new CameraTransform())
{
    connect(this->_config.data(), &Configuration::updated, this, [this] {
        this->_perspective.reset(new PerspectiveTransform(this->_config->clippingNearDistance(),
                                              this->_config->clippingFarDistance(),
                                              this->_config->sw(),
                                              this->_config->sh()));
        this->plot();
        this->update();
    });
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
    this->_button_clicked = event->button();
}

void Canvas3D::mouseMoveEvent(QMouseEvent *event)
{
    float diff_x = (float) (event->x() - this->_rotation_tracking.x()) * 2 * Pi / this->_image.width();
    float diff_y = (float) (event->y() - this->_rotation_tracking.y()) * 2 * Pi / this->_image.height();
    RotateYTransform y_transform = RotateYTransform(diff_x);
    RotateZTransform x_transform = RotateZTransform(diff_y);
    if (this->_button_clicked == Qt::LeftButton) {
        this->_config->setRotationTransform(this->_config->rotationTransform()->compose((Transform *)&y_transform)->compose((Transform *)&x_transform));
        this->_rotation_tracking.setX(event->x());
        this->_rotation_tracking.setY(event->y());
        this->plot();
        this->update();
    } else if (this->_button_clicked == Qt::RightButton) {
        this->_config->rotateCurrentObject(((Transform *)&y_transform)->compose((Transform *)&x_transform));
        this->plot();
        this->update();
    }
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
    QSharedPointer<Transform> object_transform = object->getRotation()->compose(object->getShiftTransform().data());
    if (dynamic_cast<GeneratrixObject *>(object) != nullptr) {
        Axis axis1(object->getCenter(), AxisType::OX, 0.5);
        Axis axis2(object->getCenter(), AxisType::OY, 0.5);
        Axis axis3(object->getCenter(), AxisType::OZ, 0.5);
        this->drawObject(&axis1, scale_transform);
        this->drawObject(&axis2, scale_transform);
        this->drawObject(&axis3, scale_transform);
    }
    for (auto it = segments.begin(); it < segments.end(); ++it) {
        QVector4D from_point = it->from();
        QVector4D to_point = it->to();
        from_point = object_transform->apply(from_point);
        to_point = object_transform->apply(to_point);
        if (scale_transform != NULL) {
            from_point = scale_transform->apply(from_point);
            to_point = scale_transform->apply(to_point);
        }
        from_point = this->_config->rotationTransform()->apply(from_point);
        to_point = this->_config->rotationTransform()->apply(to_point);
        from_point = this->_camera->apply(from_point);
        to_point = this->_camera->apply(to_point);
        from_point = this->_perspective->apply(from_point);
        to_point = this->_perspective->apply(to_point);
        Line3D res_line(from_point, to_point);
        if (res_line.clip()) {
            Drawing::drawLine3D(this->_image, res_line.from3D() * _scale, res_line.to3D() * _scale, object->getColor(),
                                this->_config->sw(), this->_config->sh());
        }
    }
}

void Canvas3D::drawBoundingBox()
{
    WireObject cube(std::vector<Line3D>{
                         Line3D(QVector4D(-1, -1, -1, 1), QVector4D(1, -1, -1, 1)),
                         Line3D(QVector4D(-1, -1, -1, 1), QVector4D(-1, 1, -1, 1)),
                         Line3D(QVector4D(-1, 1, -1, 1), QVector4D(1, 1, -1, 1)),
                         Line3D(QVector4D(1, -1, -1, 1), QVector4D(1, 1, -1, 1)),
                         Line3D(QVector4D(-1, -1, 1, 1), QVector4D(1, -1, 1, 1)),
                         Line3D(QVector4D(-1, -1, 1, 1), QVector4D(-1, 1, 1, 1)),
                         Line3D(QVector4D(-1, 1, 1, 1), QVector4D(1, 1, 1, 1)),
                         Line3D(QVector4D(1, -1, 1, 1), QVector4D(1, 1, 1, 1)),
                         Line3D(QVector4D(-1, -1, -1, 1), QVector4D(-1, -1, 1, 1)),
                         Line3D(QVector4D(-1, 1, -1, 1), QVector4D(-1, 1, 1, 1)),
                         Line3D(QVector4D(1, -1, -1, 1), QVector4D(1, -1, 1, 1)),
                         Line3D(QVector4D(1, 1, -1, 1), QVector4D(1, 1, 1, 1)),
                     }, QColor(0, 0, 0));
    this->drawObject(&cube, NULL);
}

float Canvas3D::findAbsMax()
{
    float max = 0;
    for (auto obj : this->_config->objects()) {
        auto &segments = obj->getSegments();
        QSharedPointer<Transform> object_transform = obj->getRotation()->compose(obj->getShiftTransform().data());
        for (auto it = segments.begin(); it < segments.end(); ++it) {
            QVector4D from_point = it->from();
            QVector4D to_point = it->to();
            from_point = object_transform->apply(from_point);
            to_point = object_transform->apply(to_point);
            QVector3D from_3D = from_point.toVector3D();
            QVector3D to_3D = to_point.toVector3D();
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
    this->_scale = std::min(this->width() / this->_config->sw(), this->height() / this->_config->sh());
    Drawing::fill(this->_image, this->_config->backgroundColor());
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
    this->drawBorders();
}

void Canvas3D::drawBorders()
{
    // vertical
    Drawing::drawLine(this->_image, QPoint(this->width() / 2 - this->_config->sw() * _scale / 2, this->height() / 2 - this->_config->sh() * _scale / 2),
                                    QPoint(this->width() / 2 - this->_config->sw() * _scale / 2, this->height() / 2 + this->_config->sh() * _scale / 2), qRgb(0, 0, 0));
    Drawing::drawLine(this->_image, QPoint(this->width() / 2 + this->_config->sw() * _scale / 2, this->height() / 2 - this->_config->sh() * _scale / 2),
                                    QPoint(this->width() / 2 + this->_config->sw() * _scale / 2, this->height() / 2 + this->_config->sh() * _scale / 2), qRgb(0, 0, 0));
    // horizontal
    Drawing::drawLine(this->_image, QPoint(this->width() / 2 - this->_config->sw() * _scale / 2, this->height() / 2 - this->_config->sh() * _scale / 2),
                                    QPoint(this->width() / 2 + this->_config->sw() * _scale / 2, this->height() / 2 - this->_config->sh() * _scale / 2), qRgb(0, 0, 0));
    Drawing::drawLine(this->_image, QPoint(this->width() / 2 - this->_config->sw() * _scale / 2, this->height() / 2 + this->_config->sh() * _scale / 2),
                                    QPoint(this->width() / 2 + this->_config->sw() * _scale / 2, this->height() / 2 + this->_config->sh() * _scale / 2), qRgb(0, 0, 0));

}
