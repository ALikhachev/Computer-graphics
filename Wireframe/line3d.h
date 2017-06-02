#ifndef LINE3D_H
#define LINE3D_H

#include <QVector3D>
#include <QVector4D>

class Line3D
{
public:
    Line3D(QVector4D from, QVector4D to);
    QVector4D from() const;
    QVector4D to() const;
    QVector3D from3D() const;
    QVector3D to3D() const;
    bool clip();

private:
    bool clipCoordinate(int pos, float min, float max);

    QVector4D _from;
    QVector4D _to;
};

#endif // LINE3D_H
