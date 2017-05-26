#include "transform.h"

QVector4D Transform::apply(HomogeneousPoint3D point) const
{
    return this->_matrix * point.to4D();
}

QSharedPointer<Transform> Transform::compose(Transform *transform)
{
    QSharedPointer<Transform> trans(new Transform);
    trans->_matrix = transform->_matrix * this->_matrix;
    return trans;
}

RotateXTransform::RotateXTransform(float angle)
{
    float cos_angle = std::cos(angle);
    float sin_angle = std::sin(angle);
    this->_matrix = {
        1.0, 0.0,        0.0,       0.0,
        0.0, cos_angle, -sin_angle, 0.0,
        0.0, sin_angle,  cos_angle, 0.0,
        0.0, 0.0,        0.0,       1.0
    };
}

RotateYTransform::RotateYTransform(float angle)
{
    float cos_angle = std::cos(angle);
    float sin_angle = std::sin(angle);
    this->_matrix = {
         cos_angle,  0.0, sin_angle, 0.0,
         0.0,        1.0, 0.0,       0.0,
        -sin_angle, 0.0, cos_angle, 0.0,
         0.0,        0.0, 0.0,       1.0
    };
}

RotateZTransform::RotateZTransform(float angle)
{
    float cos_angle = std::cos(angle);
    float sin_angle = std::sin(angle);
    this->_matrix = {
        cos_angle, -sin_angle, 0.0, 0.0,
        sin_angle,  cos_angle, 0.0, 0.0,
        0.0,        0.0,       1.0, 0.0,
        0.0,        0.0,       0.0, 1.0
    };
}

IdentityTransform::IdentityTransform()
{
    this->_matrix = {
        1.0, 0.0, 0.0, 0.0,
        0.0, 1.0, 0.0, 0.0,
        0.0, 0.0, 1.0, 0.0,
        0.0, 0.0, 0.0, 1.0
    };
}

PerspectiveTransform::PerspectiveTransform(float zn, float zf, float sw, float sh)
{
    this->_matrix = {
        2 * zf / sw, 0, 0, 0,
        0, 2 * zf / sh, 0, 0,
        0, 0, zf / (zn - zf), zn * zf / (zn - zf),
        0, 0, -1, 0
    };
}

CameraTransform::CameraTransform()
{
    QVector3D ref(10, 0, 0);
    QVector3D eye(-10, 0, 0);
    QVector3D up(0, 1, 0);
    QVector3D w(eye - ref);
    w.normalize();
    QVector3D u(QVector3D::crossProduct(up, w));
    u.normalize();
    QVector3D v(QVector3D::crossProduct(w, u));
    this->_matrix =
            QMatrix4x4(
                u[0], u[1], u[2], 0,
                v[0], v[1], v[2], 0,
                w[0], w[1], w[2], 0,
                0   , 0   , 0   , 1
            )
            *
            QMatrix4x4(
                1, 0, 0, -ref[0],
                0, 1, 0, -ref[1],
                0, 0, 1, -ref[2],
                0, 0, 0, 1
            );
}
