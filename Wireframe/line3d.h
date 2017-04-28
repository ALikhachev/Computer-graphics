#ifndef LINE3D_H
#define LINE3D_H

#include <QVector3D>
#include "homogeneous_point3D.h"

class Line3D
{
public:
    Line3D(HomogeneousPoint3D from, HomogeneousPoint3D to);
    HomogeneousPoint3D from();
    HomogeneousPoint3D to();
    QVector3D from3D() const;
    QVector3D to3D() const;

private:
    HomogeneousPoint3D _from;
    HomogeneousPoint3D _to;
};

#endif // LINE3D_H
