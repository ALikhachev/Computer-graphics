#include "homogeneous_point3d.h"

HomogeneousPoint3D::HomogeneousPoint3D(float x, float y, float z) :
    _coordinates(x, y, z, 1.0F)
{

}

const QVector3D HomogeneousPoint3D::to3D() const
{
    return QVector3D(_coordinates.x() / _coordinates.w(),
                     _coordinates.y() / _coordinates.w(),
                     _coordinates.z() / _coordinates.w());
}

const QVector4D &HomogeneousPoint3D::to4D() const
{
    return this->_coordinates;
}
