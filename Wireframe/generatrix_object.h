#ifndef GENERATRIX_OBJECT_H
#define GENERATRIX_OBJECT_H

#include <QColor>
#include <QMatrix3x3>
#include <QPoint>

#include "bspline.h"

class GeneratrixObject
{
public:
    GeneratrixObject(QRgb color, std::vector<QPoint> knots);
    std::vector<std::pair<QPointF, QPointF> > getSegments() const;
    QRgb color() const;
    std::vector<QPoint> knots() const;

private:
    QRgb _color;
    QMatrix3x3 _rotation_matrix;
    std::vector<QPoint> _knots;
    BSpline _spline;
};

#endif // GENERATRIX_OBJECT_H
