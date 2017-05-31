#ifndef WIRE_OBJECT_H
#define WIRE_OBJECT_H

#include <vector>
#include <QSharedPointer>

#include "transform.h"
#include "line3d.h"

class WireObject
{
public:
    WireObject(std::vector<Line3D> lines);
    WireObject(std::vector<Line3D> lines, QVector3D center);

    virtual const std::vector<Line3D> &getSegments() const;
    QSharedPointer<Transform> getShiftTransform() const;

    virtual ~WireObject() {};

protected:
    WireObject(QVector3D center);

private:
    std::vector<Line3D> _segments;
    QSharedPointer<Transform> _shift_transform;
};

#endif // WIRE_OBJECT_H
