#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <QMatrix4x4>
#include <QVector4D>
#include <QSharedPointer>

#include "homogeneous_point3d.h"

class HomogeneousPoint3D;

class Transform
{
public:
    QVector4D apply(const HomogeneousPoint3D point) const;
    QSharedPointer<Transform> compose(Transform *transform);
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

class PerspectiveTransform : public Transform
{
public:
    PerspectiveTransform(float zn, float zf, float sw, float sh);
};

class CameraTransform : public Transform
{
public:
    CameraTransform();
};

class ScaleTransform : public Transform
{
public:
    ScaleTransform(float scale_factor);
};

#endif // TRANSFORM_H
