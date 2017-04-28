#ifndef HOMOGENEOUS_POINT3D_H
#define HOMOGENEOUS_POINT3D_H

#include <QVector3D>
#include <QVector4D>

class HomogeneousPoint3D
{
public:
    HomogeneousPoint3D(float x, float y, float z);
    QVector3D to3D() const;

private:
    QVector4D _coordinates;
};

#endif // HOMOGENEOUS_POINT3D_H
