#include "axis.h"

#include <QVector4D>

Axis::Axis(AxisType type, float length) : Axis(QVector3D(0, 0, 0), type, length)
{

}

Axis::Axis(QVector3D center, AxisType type, float length) : WireObject({type == AxisType::OX ?
                                                                           Line3D(QVector4D(0, 0, 0, 1), QVector4D(length, 0, 0, 1))
                                                                     : type == AxisType::OY ?
                                                                           Line3D(QVector4D(0, 0, 0, 1), QVector4D(0, length, 0, 1))
                                                                     :
                                                                           Line3D(QVector4D(0, 0, 0, 1), QVector4D(0, 0, length, 1))
                                                                      },
                                                                       center,
                                                                       (type == AxisType::OX ? QColor(255, 0, 0) : type == AxisType::OY ? QColor(0, 255, 0) : QColor(0, 0, 255))
                                                                       )
{

}
