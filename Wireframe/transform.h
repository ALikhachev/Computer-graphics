#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <QMatrix4x4>

#include "homogeneous_point3D.h"

class Transform
{
public:
    QVector4D apply(HomogeneousPoint3D point);
protected:
    QMatrix4x4 _matrix;
};

class RotateXTransform : Transform
{
public:
    RotateXTransform(float angle);
};

class RotateYTransform : Transform
{
public:
    RotateYTransform(float angle);
};

class RotateZTransform : Transform
{
public:
    RotateZTransform(float angle);
};

#endif // TRANSFORM_H
