#ifndef WIRE_OBJECT_H
#define WIRE_OBJECT_H

#include <vector>

#include "line3d.h"

class WireObject
{
public:
    WireObject(std::vector<Line3D> lines);
    const std::vector<Line3D> &getSegments() const;

private:
    std::vector<Line3D> _segments;
};

#endif // WIRE_OBJECT_H
