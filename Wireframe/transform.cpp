#include "transform.h"

QVector4D Transform::apply(const HomogeneousPoint3D point) const
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

PerspectiveTransform::PerspectiveTransform(float n, float f, float w, float h)
{
    this->_matrix = {
        (2 / w) * n,  0,          0,            0,
        0,           (2 / h) * n, 0,            0,
        0,            0,          f / (f - n), -f * n / (f - n),
        0,            0,          1,            0
    };
}
