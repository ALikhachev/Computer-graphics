#include "wire_object.h"

WireObject::WireObject(std::vector<Line3D> lines) :
    _segments(lines)
{

}

const std::vector<Line3D> &WireObject::getSegments() const
{
    return this->_segments;
}

WireObject::WireObject() :
    _segments()
{

}
