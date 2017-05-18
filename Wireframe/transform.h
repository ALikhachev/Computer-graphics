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
    PerspectiveTransform(float clipping_near, float clipping_far,
                         float clipping_rect_width, float clipping_rect_height);
};

class CameraTransform : public Transform
{
public:
    CameraTransform(float x, float y, float z);
};

#endif // TRANSFORM_H
