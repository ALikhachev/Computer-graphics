#ifndef WIRE_OBJECT_H
#define WIRE_OBJECT_H

#include <vector>
#include <QSharedPointer>
#include <QColor>

#include "transform.h"
#include "line3d.h"

class WireObject
{
public:
    WireObject(std::vector<Line3D> lines, QColor color);
    WireObject(std::vector<Line3D> lines, QVector3D center, QColor color);

    virtual const std::vector<Line3D> &getSegments() const;
    QSharedPointer<Transform> getShiftTransform() const;
    QVector3D getCenter() const;
    QColor getColor() const;

    virtual ~WireObject() {}

protected:
    WireObject(QVector3D center, QColor color);

private:
    std::vector<Line3D> _segments;
    QSharedPointer<Transform> _shift_transform;
    QVector3D _center;
    QColor _color;
};

#endif // WIRE_OBJECT_H
