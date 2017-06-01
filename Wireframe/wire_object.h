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
    void setCenter(QVector3D center);
    QColor getColor() const;
    QSharedPointer<Transform> getRotation() const;
    void setRotation(QSharedPointer<Transform> transform);

    virtual ~WireObject() {}

protected:
    WireObject(QVector3D center, QColor color);
    WireObject(QVector3D center, QColor color, QSharedPointer<Transform> rotation);

private:
    std::vector<Line3D> _segments;
    QSharedPointer<Transform> _shift_transform;
    QVector3D _center;
    QColor _color;
    QSharedPointer<Transform> _rotation;
};

#endif // WIRE_OBJECT_H
