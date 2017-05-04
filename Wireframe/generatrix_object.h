#ifndef GENERATRIX_OBJECT_H
#define GENERATRIX_OBJECT_H

#include <QColor>
#include <QMatrix4x4>
#include <QPoint>
#include <QSharedPointer>

#include "bspline.h"
#include "homogeneous_point3d.h"
#include "line3d.h"

class GeneratrixObject
{
public:
    GeneratrixObject(QRgb color, std::vector<QPoint> knots);
    std::vector<std::pair<QPointF, QPointF> > getSegments2D() const;
    QRgb color() const;
    std::vector<QPoint> knots() const;
    std::vector<Line3D> getObjectSegments() const;

private:
    QRgb _color;
    QMatrix4x4 _rotation_matrix;
    std::vector<QPoint> _knots;
    BSpline _spline;
    constexpr double static const Pi = std::acos(-1);
};

#endif // GENERATRIX_OBJECT_H
