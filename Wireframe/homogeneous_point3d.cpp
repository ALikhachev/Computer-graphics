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

QVector4D &HomogeneousPoint3D::to4D()
{
    return this->_coordinates;
}

HomogeneousPoint3D &HomogeneousPoint3D::applyTransform(QSharedPointer<Transform> transform)
{
    this->_coordinates = transform->apply(*this);
    return *this;
}

HomogeneousPoint3D &HomogeneousPoint3D::applyTransform(Transform *transform)
{
    this->_coordinates = transform->apply(*this);
    return *this;
}

void HomogeneousPoint3D::normalize() {
    for (int i = 0; i < 3; ++i) {
        this->_coordinates[i] = this->_coordinates[i] / this->_coordinates[3];
    }
    this->_coordinates[3] = 1.0;
}
