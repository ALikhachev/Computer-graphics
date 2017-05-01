#include "line3d.h"

Line3D::Line3D(HomogeneousPoint3D from, HomogeneousPoint3D to) :
    _from(from),
    _to(to)
{

}

HomogeneousPoint3D Line3D::from() const
{
    return this->_from;
}

HomogeneousPoint3D Line3D::to() const
{
    return this->_to;
}

QVector3D Line3D::from3D() const
{
    return this->_from.to3D();
}

QVector3D Line3D::to3D() const
{
    return this->_to.to3D();
}
