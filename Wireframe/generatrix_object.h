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
    GeneratrixObject(int n, int m, int k, float a, float b, float c, float d,
                     QRgb color, QVector3D center, std::vector<QPointF> knots);
    const BSpline &getBSpline() const;
    QRgb color() const;
    std::vector<QPointF> knots() const;
    const std::vector<Line3D> &getSegments() const override;
    void setN(int n);
    void setM(int m);
    void setK(int k);
    void setA(float a);
    void setB(float b);
    void setC(float c);
    void setD(float d);

private:
    void buildSegments();

    QRgb _color;
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
