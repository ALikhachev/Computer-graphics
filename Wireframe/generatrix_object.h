#ifndef GENERATRIX_OBJECT_H
#define GENERATRIX_OBJECT_H

#include <QColor>
#include <QMatrix4x4>
#include <QPoint>
#include <QSharedPointer>

#include "bspline.h"
#include "homogeneous_point3d.h"
#include "line3d.h"
#include "wire_object.h"

class GeneratrixObject : public WireObject
{
public:
    GeneratrixObject(int n, int m, int k, float a, float b, float c, float d,
                     QRgb color, std::vector<QPointF> knots);
    std::vector<std::pair<QPointF, QPointF> > getSegments2D() const;
    QRgb color() const;
    std::vector<QPointF> knots() const;
    const std::vector<Line3D> &getSegments() const override;

private:
    void buildSegments();

    QRgb _color;
    QMatrix4x4 _rotation_matrix;
    std::vector<QPointF> _knots;
    std::vector<Line3D> _segments;
    BSpline _spline;
    constexpr double static const Pi = std::acos(-1);
    float _a;
    float _b;
    float _c;
    float _d;
    int _n;
    int _m;
    int _k;
};

#endif // GENERATRIX_OBJECT_H
