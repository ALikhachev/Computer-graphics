#include "wire_object.h"

WireObject::WireObject(std::vector<Line3D> lines, QColor color) : WireObject(lines, QVector3D(0, 0, 0), color)
{

}

WireObject::WireObject(std::vector<Line3D> lines, QVector3D center, QColor color) :
    _segments(lines),
    _shift_transform(new ShiftTransform(center.x(), center.y(), center.z())),
    _center(center),
    _color(color),
    _rotation(new IdentityTransform())
{

}

const std::vector<Line3D> &WireObject::getSegments() const
{
    return this->_segments;
}

QSharedPointer<Transform> WireObject::getShiftTransform() const
{
    return _shift_transform;
}

QVector3D WireObject::getCenter() const
{
    return _center;
}

void WireObject::setCenter(QVector3D center)
{
    this->_center = center;
    _shift_transform.reset(new ShiftTransform(center.x(), center.y(), center.z()));
}

QColor WireObject::getColor() const
{
    return _color;
}

QSharedPointer<Transform> WireObject::getRotation() const
{
    return _rotation;
}

void WireObject::setRotation(QSharedPointer<Transform> transform)
{
    this->_rotation = transform;
}

WireObject::WireObject(QVector3D center, QColor color) : WireObject(center, color, QSharedPointer<Transform>(new IdentityTransform()))
{

}

WireObject::WireObject(QVector3D center, QColor color, QSharedPointer<Transform> rotation) :
    _shift_transform(new ShiftTransform(center.x(), center.y(), center.z())),
    _center(center),
    _color(color),
    _rotation(rotation)
{

}
