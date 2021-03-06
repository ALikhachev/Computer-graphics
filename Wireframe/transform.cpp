#include "transform.h"

QVector4D Transform::apply(const QVector4D point) const
{
    return this->_matrix * point;
}

QSharedPointer<Transform> Transform::compose(Transform *transform)
{
    QSharedPointer<Transform> trans(new Transform);
    trans->_matrix = transform->_matrix * this->_matrix;
    return trans;
}

void Transform::save3D(QTextStream &stream) const
{
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            stream << _matrix(i, j) << " ";
        }
        stream << '\n';
    }
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
                1, 0, 0, -eye[0],
                0, 1, 0, -eye[1],
                0, 0, 1, -eye[2],
                0, 0, 0, 1
            );
}

ScaleTransform::ScaleTransform(float scale_factor)
{
    this->_matrix = {
        1.0f / scale_factor, 0.0,                 0.0,                 0.0,
        0.0,                 1.0f / scale_factor, 0.0,                 0.0,
        0.0,                 0.0,                 1.0f / scale_factor, 0.0,
        0.0,                 0.0,                 0.0,                 1.0f
    };
}

ShiftTransform::ShiftTransform(float x, float y, float z)
{
    this->_matrix = {
        1.0, 0.0, 0.0, x,
        0.0, 1.0, 0.0, y,
        0.0, 0.0, 1.0, z,
        0.0, 0.0, 0.0, 1.0
    };
}

MatrixTransform::MatrixTransform(float a1, float a2, float a3, float a4, float a5, float a6, float a7, float a8, float a9)
{
    this->_matrix = {
        a1 , a2 , a3 , 0,
        a4 , a5 , a6 , 0,
        a7 , a8 , a9 , 0,
        0.0, 0.0, 0.0, 1.0
    };
}
