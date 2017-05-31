#ifndef AXIS_H
#define AXIS_H

#include "wire_object.h"
#include "homogeneous_point3D.h"

enum AxisType {
    OX,
    OY,
    OZ
};

class Axis : public WireObject
{
public:
    Axis(AxisType type, float length);
    Axis(QVector3D center, AxisType type, float length);
};

#endif // AXIS_H
