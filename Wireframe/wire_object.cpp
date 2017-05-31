#include "wire_object.h"

WireObject::WireObject(std::vector<Line3D> lines) : WireObject(lines, QVector3D(0, 0, 0))
{

}

WireObject::WireObject(std::vector<Line3D> lines, QVector3D center) :
    _segments(lines),
    _shift_transform(new ShiftTransform(center.x(), center.y(), center.z()))
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

WireObject::WireObject(QVector3D center) :
    _shift_transform(new ShiftTransform(center.x(), center.y(), center.z()))
{

}
