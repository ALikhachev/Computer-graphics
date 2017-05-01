#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <QMatrix4x4>
#include <QVector4D>

#include "homogeneous_point3d.h"

class HomogeneousPoint3D;

class Transform
{
public:
    QVector4D apply(const HomogeneousPoint3D point) const;
    Transform *compose(Transform *transform);
protected:
    QMatrix4x4 _matrix;
};

class RotateXTransform : public Transform
{
public:
    RotateXTransform(float angle);
};

class RotateYTransform : public Transform
{
public:
    RotateYTransform(float angle);
};

class RotateZTransform : public Transform
{
public:
    RotateZTransform(float angle);
};

class IdentityTransform : public Transform
{
public:
    IdentityTransform();
};

#endif // TRANSFORM_H
