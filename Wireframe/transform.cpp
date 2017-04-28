#include "transform.h"

QVector4D Transform::apply(HomogeneousPoint3D point)
{
    return this->_matrix * point.to4D();
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
