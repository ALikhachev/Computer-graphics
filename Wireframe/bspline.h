#ifndef BSPLINE_H
#define BSPLINE_H

#include <QMatrix4x4>
#include <QVector4D>
#include <QPoint>

class BSpline
{
public:
    BSpline(std::vector<QPoint> values);

    /**
     * x = T * Mx * G(i)x
     * y = T * My * G(i)y
     */
    QPointF solve(int knot, double t) const;
private:
    void calculateCoefficientsVectors();

    std::vector<QPoint> values;
    QMatrix4x4 spline_matrix;
    std::vector<QVector4D> vectors_x;
    std::vector<QVector4D> vectors_y;
};

#endif // BSPLINE_H