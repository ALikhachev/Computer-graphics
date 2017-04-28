#include "axis.h"

Axis::Axis(AxisType type, float length) : WireObject({type == AxisType::OX ?
               Line3D(HomogeneousPoint3D(0, 0, 0), HomogeneousPoint3D(length, 0, 0))
         : type == AxisType::OY ?
               Line3D(HomogeneousPoint3D(0, 0, 0), HomogeneousPoint3D(0, length, 0))
         :
               Line3D(HomogeneousPoint3D(0, 0, 0), HomogeneousPoint3D(0, 0, length))
          })
{

}
