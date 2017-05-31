#include "axis.h"

Axis::Axis(AxisType type, float length) : Axis(QVector3D(0, 0, 0), type, length)
{

}

Axis::Axis(QVector3D center, AxisType type, float length) : WireObject({type == AxisType::OX ?
                                                                           Line3D(HomogeneousPoint3D(0, 0, 0), HomogeneousPoint3D(length, 0, 0))
                                                                     : type == AxisType::OY ?
                                                                           Line3D(HomogeneousPoint3D(0, 0, 0), HomogeneousPoint3D(0, length, 0))
                                                                     :
                                                                           Line3D(HomogeneousPoint3D(0, 0, 0), HomogeneousPoint3D(0, 0, length))
                                                                      },
                                                                       center,
                                                                       (type == AxisType::OX ? QColor(255, 0, 0) : type == AxisType::OY ? QColor(0, 255, 0) : QColor(0, 0, 255))
                                                                       )
{

}
